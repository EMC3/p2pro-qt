#ifndef VIDEO4LINUXWRAPPERGENERIC_H
#define VIDEO4LINUXWRAPPERGENERIC_H
#include <cstdint>
#include <thread>
#include <vector>
#include <functional>
#include <QString>
#include <linux/videodev2.h>

class Video4LinuxWrapperGeneric
{
public:
    Video4LinuxWrapperGeneric(QString device, uint32_t format, const int width, const int height, std::function<void(void*, int)> frame_cb);

    ~Video4LinuxWrapperGeneric();
    QString errorString;
    bool hasError;
    void stop();
    void start();
private:
    int fd;  // File descriptor for the video device
    int width;
    int height;
    uint32_t format;
    std::vector<char*> buffers;
    std::function<void(void*, int)> cb;
    QString dev;

    void worker();

    std::thread thd;
    volatile bool run;
};

#endif // VIDEO4LINUXWRAPPERGENERIC_H
