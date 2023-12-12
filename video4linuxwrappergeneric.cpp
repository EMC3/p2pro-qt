#include "video4linuxwrappergeneric.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <string.h>
#include <logger/log.h>
#include <sys/mman.h>


Video4LinuxWrapperGeneric::Video4LinuxWrapperGeneric(const char* device, uint32_t format, const int width, const int height, std::function<void(void*, int)> frame_cb)
    : width(width), height(height), format(format), cb(frame_cb), dev(device)
{
    run = false;
}


Video4LinuxWrapperGeneric::~Video4LinuxWrapperGeneric() {
    // Close the video device
    stop();
}

void Video4LinuxWrapperGeneric::stop()
{
    if(run){
        run = false;
        thd.join();
    }
}

void Video4LinuxWrapperGeneric::start()
{
    if(run && hasError)stop();
    if(run){
        ERR << "Ignoring repeated start";
        return;
    }
    hasError = false;
    run = true;
    thd = std::thread(std::bind(&Video4LinuxWrapperGeneric::worker, this));
}

void Video4LinuxWrapperGeneric::worker()
{
    // Open the video device
    fd = open(dev, O_RDWR | O_NONBLOCK, 0);
    if (fd == -1) {
        ERR << "Error opening " << dev;
        errorString = "Error opening "+QString(dev)+" "+strerror(errno);
        hasError = true;
        close(fd);
        return;
    }
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = format;
    fmt.fmt.pix.field = V4L2_FIELD_ANY;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        ERR << "Error setting format";
        errorString = QString("Error setting format: ")+strerror(errno);
        hasError = true;
        close(fd);
        return;
    }

    int nBuffers = 4;
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(v4l2_requestbuffers));
    req.count = nBuffers;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        ERR << "Requesting Buffer";
        errorString = QString("Error requesting buffer: ")+strerror(errno);
        hasError = true;
        close(fd);
        return;
    }
    nBuffers = req.count;
    buffers.resize(nBuffers);
    for(int i = 0; i < nBuffers; i++){
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        int res = ioctl(fd, VIDIOC_QUERYBUF, &buf);
        if(res == -1) {
            ERR << "Could not query buffer";
            errorString = QString("Error query buffer: ")+strerror(errno);
            hasError = true;
            close(fd);
            return;
        }

        buffers[i] = (char*)mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if(buffers[i] == MAP_FAILED){
            errorString = QString("Error MMAP: ")+strerror(errno);
            hasError = true;
            close(fd);
            return;
        }
    }

    for(int i = 0; i < nBuffers; i++){
        struct v4l2_buffer bufd;
        memset(&bufd, 0, sizeof(v4l2_buffer));
        bufd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufd.memory = V4L2_MEMORY_MMAP;
        bufd.index = i;
        if(-1 == ioctl(fd, VIDIOC_QBUF, &bufd))
        {
            ERR << "Queue Buffer";
            errorString = QString("Error queue buffer: ")+strerror(errno);
            hasError = true;
            close(fd);
            return;
        }
    }

    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMON, &type) == -1){
        ERR << "VIDIOC_STREAMON";
        errorString = QString("Error streamon: ")+strerror(errno);
        hasError = true;
        close(fd);
        return;
    }

    while(run){
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval tv = {0};
        tv.tv_sec = 5;
        int r = select(fd+1, &fds, NULL, NULL, &tv);
        if(-1 == r){
            ERR << "Waiting for Frame";
            errorString = QString("Error waiting for frame: ")+strerror(errno);
            hasError = true;
            break;
        }

        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        // Request a frame from the camera
        if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
            ERR << "Error dequeuing buffer";
            errorString = QString("Error dequeueing buffer: ")+strerror(errno);
            hasError = true;
            // Handle the error or throw an exception
            break;
        }

        cb(buffers[buf.index], buf.bytesused);


        // Release the buffer back to the camera
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            ERR << "Error queuing buffer";
            errorString = QString("Error requeueing buffer: ")+strerror(errno);
            hasError = true;
            // Handle the error or throw an exception
            break;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMOFF, &type) == -1){
        ERR << "VIDIOC_STREAMOFF";
    }

    close(fd);
}
