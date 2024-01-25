#ifndef THERMALCAMERA_H
#define THERMALCAMERA_H

#include <QObject>
#include <memory>
#include <QTimer>

class ThermalImage{
public:
    ThermalImage(int w, int h);
    ~ThermalImage();
    void computeMetrics();
    inline float getValue(int x, int y){
        return data[x + y*width];
    }

    int maxX, maxY;
    int minX, minY;
    float max, min;
    float * data;
    int width;
    int height;
};

class Video4LinuxWrapperGeneric;

class ThermalCamera : public QObject
{
    Q_OBJECT
public:
    enum ROTATION{
        DEG0 = 0,
        DEG90 = 1,
        DEG180 = 2,
        DEG270 = 3,
    };

    explicit ThermalCamera(QObject *parent = nullptr);
    void start(QString videoDevice);
    void start(QString videoDevice,ThermalCamera::ROTATION newRotation);
    void stop();
    void rotateCam(int idx);
    std::shared_ptr<ThermalImage> lastImage;

    bool isStarted();
    bool hasError();


signals:
    void newThermalImage();
    void statusUpdate(QString statusStr);

private:
    void imgCb(void *data, int nbytes);
    void updateStatus();

    ROTATION cameraRotation;

    std::shared_ptr<Video4LinuxWrapperGeneric> v4lcam;
    int frameCnt;
    QTimer * statusTimer;
    bool started;
};

#endif // THERMALCAMERA_H

