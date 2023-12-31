#include "thermalcamera.h"
#include "video4linuxwrappergeneric.h"
#include "logger/log.h"
ThermalCamera::ThermalCamera(QObject *parent)
    : QObject{parent}
{
    v4lcam = std::make_shared<Video4LinuxWrapperGeneric>("/dev/video0", V4L2_PIX_FMT_YUYV, 256, 384, std::bind(&ThermalCamera::imgCb, this, std::placeholders::_1, std::placeholders::_2));
    statusTimer = new QTimer(this);
    statusTimer->start(1000);
    connect(statusTimer, &QTimer::timeout, this, &ThermalCamera::updateStatus);
    started = false;
}

void ThermalCamera::start()
{
    started = true;
    v4lcam->start();
}

void ThermalCamera::stop()
{
    started = false;
    v4lcam->stop();
}

void ThermalCamera::imgCb(void * data, int nbytes)
{
    if(nbytes != 256*384*2){
        LOG << "Bad byte count: "<<nbytes;
        return;
    }

    auto nextImg = std::make_shared<ThermalImage>(256, 192);

    uint16_t * dp = (uint16_t*)data;
    float * tp = nextImg->data;
    int k = 0;
    for(int y = 0; y < 192; y++){
        for(int x = 0; x < 256; x++){
            uint16_t v = dp[x + (y+192)*256];
            float t = (float(v >> 2) / 16.0) - 273.15;
            tp[k++] = t;
        }
    }

    nextImg->computeMetrics();

    lastImage = nextImg;

    QMetaObject::invokeMethod(this, [this](){
            frameCnt++;
            emit newThermalImage();
        }, Qt::QueuedConnection);
}

void ThermalCamera::updateStatus()
{
    if(!started){
        emit statusUpdate("IDLE");
        return;
    }

    int fps = frameCnt;
    frameCnt = 0;
    if(v4lcam->hasError){
        emit statusUpdate("Failed: "+v4lcam->errorString);
        return;
    }

    emit statusUpdate("Running ... "+QString::number(fps)+" FPS");

}

ThermalImage::ThermalImage(int w, int h){
    this->width = w;
    this->height = h;
    this->data = new float[width*height];
}

ThermalImage::~ThermalImage(){
    delete [] data;
}

void ThermalImage::computeMetrics(){
    minX = minY = maxX = maxY = 0;
    max = data[0];
    min = data[0];
    int i = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            float v = data[i];
            if(v > max){
                max = v;
                maxX = x;
                maxY = y;
            }
            if(v < min){
                min = v;
                minX = x;
                minY = y;
            }
            i++;
        }
    }
}
