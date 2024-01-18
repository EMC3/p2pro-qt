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
    explicit ThermalCamera(QObject *parent = nullptr);
    void start(QString videoDevice);
    void stop();
    std::shared_ptr<ThermalImage> lastImage;

    bool isStarted();
    bool hasError();
signals:
    void newThermalImage();
    void statusUpdate(QString statusStr);
private:
    void imgCb(void *data, int nbytes);
    void updateStatus();
    std::shared_ptr<Video4LinuxWrapperGeneric> v4lcam;
    int frameCnt;
    QTimer * statusTimer;
    bool started;
};

#endif // THERMALCAMERA_H

