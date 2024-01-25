#include "thermalcamera.h"
#include "video4linuxwrappergeneric.h"
#include "logger/log.h"
ThermalCamera::ThermalCamera(QObject *parent)
    : QObject{parent}
{
    statusTimer = new QTimer(this);
    statusTimer->start(1000);
    connect(statusTimer, &QTimer::timeout, this, &ThermalCamera::updateStatus);
    started = false;
    cameraRotation = ROTATION::DEG0;
}

void ThermalCamera::start(QString videoDevice)
{
    started = true;
    v4lcam = std::make_shared<Video4LinuxWrapperGeneric>(videoDevice, V4L2_PIX_FMT_YUYV, 256, 384, std::bind(&ThermalCamera::imgCb, this, std::placeholders::_1, std::placeholders::_2));
    v4lcam->start();
}

void ThermalCamera::start(QString videoDevice, ROTATION newRotation)
{
    cameraRotation = newRotation;
    start(videoDevice);
}

void ThermalCamera::stop()
{
    if(!started)return;
    started = false;
    v4lcam->stop();
}

void ThermalCamera::rotateCam(int idx){
    cameraRotation = ROTATION(idx);
}

bool ThermalCamera::isStarted()
{
    return started;
}

bool ThermalCamera::hasError()
{
    if(!started)return false;
    return v4lcam->hasError;
}

void ThermalCamera::imgCb(void * data, int nbytes)
{
    if(nbytes != 256*384*2){
        LOG << "Bad byte count: "<<nbytes;
        return;
    }


    if (cameraRotation == ROTATION::DEG0){
        auto nextImg = std::make_shared<ThermalImage>(256, 192);
        uint16_t * dp = (uint16_t*)data;
        float * tp = nextImg->data;
        int k = 0;
        for(int y = 0; y < 192; y++){
            for(int x = 0; x < 256; x++){
                uint16_t v = dp[x + (y+192)*256];
                /*                      ^
                 * InfiRay sends Raw Data and Some kind of calibrated data.
                 * To extract the Raw Data 192 as Offset is used. (y+192) */
                float t = (float(v >> 2) / 16.0) - 273.15;
                tp[k++] = t;
            }
        }
        nextImg->computeMetrics();
        lastImage = nextImg;
    }
    if (cameraRotation == ROTATION::DEG180){
        auto nextImg = std::make_shared<ThermalImage>(256, 192);
        uint16_t * dp = (uint16_t*)data;
        float * tp = nextImg->data;
        //int k = 0;
        int k = 256*192-1;
        for(int y = 0; y < 192; y++){
            for(int x = 0; x < 256; x++){
                uint16_t v = dp[x + (y+192)*256];
                float t = (float(v >> 2) / 16.0) - 273.15;
                tp[k--] = t;
            }
        }
        nextImg->computeMetrics();
        lastImage = nextImg;
    }
    else if (cameraRotation == ROTATION::DEG90){
        auto nextImg = std::make_shared<ThermalImage>(192, 256);
        uint16_t * dp = (uint16_t*)data;
        float * tp = nextImg->data;
        int k = 0;
        for(int x = 0; x < 256; x++){
            for(int y = 191; y >= 0; y--){
                uint16_t v = dp[x + (y+192)*256];
                float t = (float(v >> 2) / 16.0) - 273.15;
                tp[k++] = t;
            }
        }
        nextImg->computeMetrics();
        lastImage = nextImg;
    }
    else if (cameraRotation == ROTATION::DEG270){
        auto nextImg = std::make_shared<ThermalImage>(192, 256);
        uint16_t * dp = (uint16_t*)data;
        float * tp = nextImg->data;
        int k = 0;
        for(int x = 255; x>=0; x--){
            for(int y = 0; y < 192; y++){
                uint16_t v = dp[x + (y+192)*256];
                float t = (float(v >> 2) / 16.0) - 273.15;
                tp[k++] = t;
            }
        }
        nextImg->computeMetrics();
        lastImage = nextImg;
    }

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
