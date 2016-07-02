#ifndef PIDBALANCERWIDGET_H
#define PIDBALANCERWIDGET_H

#include <qt5/QtCore/QObject>
#include <qt5/QtCore/QThread>
#include <qt5/QtCore/QMutex>
#include <qt5/QtCore/QElapsedTimer>
#include <qt5/QtCore/QWaitCondition>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QGroupBox>
#include <qt5/QtWidgets/QPushButton>
#include "rosworker.h"
#include "pidwidget.h"
#include "sensorgraph.h"

class PidWorker : public QObject
{
    Q_OBJECT

public:
    explicit PidWorker(RosWorker* rw, SimplePid* pid, QMutex* mutex);

public slots:
    void doWork();
    void pause();
    void resume();
    void stop();
    void stepPid();

signals:
    void ioGraphDataUpdated(float SP, float PV);
    void finished();

private:
    RosWorker* rw;
    SimplePid* pid;
    QMutexLocker locker;
    bool running;
    bool paused;
    QWaitCondition pauseCondition;

    float SP;
    float SMA;
    float SMA_n;
    std::deque<float> SMA_window;

    usb2ax_controller::SetMotorParam setMotorParamSrv;
    std_srvs::Empty emptySrv;

    tf::StampedTransform transform;

    tf::Quaternion q;
    float position, PV, output;

    float SMA_newValue, SMA_oldValue;

};

class PidBalancerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PidBalancerWidget(RosWorker* rosWorker, QWidget* parent = 0);
    ~PidBalancerWidget();

signals:

public slots:
    void setBalancingActive(bool activate);
    void updateIoGraphData(float SP, float PV);

private:
    QGroupBox* ankleBalancingGroupBox;
    RosWorker* mRosWorker;
    QThread* workerThread;
    QMutex moveMutex;
    SimplePid* pid;
    PidWidget* pidWidget;
    PidWorker* pidWorker;
    SensorGraph* ioGraph;
    QPushButton* toggleBalancingButton;
    QElapsedTimer* elapsedTimer;
};

#endif // PIDBALANCERWIDGET_H
