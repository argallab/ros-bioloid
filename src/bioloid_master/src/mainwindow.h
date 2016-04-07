#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt5/QtCore/QVector>
#include <qt5/QtCore/QThread>
#include <qt5/QtCore/QMutex>
#include <qt5/QtWidgets/QMainWindow>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QDockWidget>
#include <qt5/QtWidgets/QMenu>
#include <qt5/QtWidgets/QAction>
#include <qt5/QtWidgets/QPushButton>
#include <qt5/QtWidgets/QLineEdit>
//
#include "sensor_msgs/JointState.h"
#include <moveit/move_group_interface/move_group.h>
//
#include "rosworker.h"
#include "customlistwidget.h"
#include "outputlog.h"
#include "doubleslider.h"
#include "motorvalueeditor.h"
#include "motoraddresseditor.h"
#include "motordials.h"
#include "moveithandler.h"
#include "sensorgrapher.h"

class PlanAndExecuteChainWorker : public QObject
{
    Q_OBJECT

public:
    explicit PlanAndExecuteChainWorker(QList<RobotPose> poses, RosWorker* rw, QMutex* mutex);

public slots:
    void doWork();

signals:
    void finished();

private:
    QList<RobotPose> poses;
    RosWorker* rw;
    QMutex* mutex;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char* argv[], QWidget* parent = 0);
    ~MainWindow();

signals:

public slots:
    void initRosNode();
    void initMoveItHandler();
    void addPose();
    void removePose();

    void planAndExecuteChain();

    void addToQueue();
    void removeFromQueue();

    void nodeConnectedToRosMaster();
    void nodeDisconnectedFromRosMaster();

    void enableMotionButtons();
    void disableMotionButtons();

    void updateJointStateValues(sensor_msgs::JointState js);
    void updateSecondaryRobotValues(sensor_msgs::JointState js);
    void updateJointStateValuesFromPose(const QModelIndex &modelIndex);

    void aboutQt();
    void about();
    void quit();

private:
    void setUpLayout();
    void customiseLayout();
    void connectSignalsAndSlots();

    RosWorker* rosWorker;
    MotorValueEditor* motorValueEditor;
    MotorAddressEditor* motorAddressEditor;
    MotorDials* motorDials;
    MoveItHandler* moveItHandler;
    OutputLog* outputLog;
    SensorGrapher* sensorGrapher;

    QAction* exitAct;
    QAction* aboutQtAct;
    QAction* aboutAct;

    QPushButton* initRosNodeButton;
    QPushButton* initMoveItHandlerButton;
    QPushButton* setCurrentAsStartStateButton;
    QPushButton* setCurrentAsGoalStateButton;
    QPushButton* planMotionButton;
    QPushButton* executeMotionButton;

    QPushButton* addPoseButton;
    QPushButton* removePoseButton;
    QPushButton* addToQueueButton;
    QPushButton* removeFromQueueButton;
    QPushButton* planAndExecuteChainButton;
    QPushButton* testButton;

    QPushButton* homeAllMotorsButton;
    QPushButton* setAllMotorTorquesOffButton;

    QPushButton* saveAvailablePosesFileButton;
    QPushButton* saveQueuedPosesFileButton;
    QPushButton* loadAvailablePosesFileButton;
    QPushButton* loadQueuedPosesFileButton;

    CustomListWidget* availablePosesCustomListWidget;
    CustomListWidget* queuedPosesCustomListWidget;

    QVector<DoubleSlider*> presentPosSliders;
    QVector<QLineEdit*> presentPosLineEdits;
    QVector<QLineEdit*> goalPosLineEdits;
    QVector<QLineEdit*> presentSpeedLineEdits;
    QVector<QLineEdit*> movingSpeedLineEdits;

    QDockWidget* motorFeedbackDockWidget;
    QDockWidget* motorCommandsDockWidget;
    QDockWidget* poseControlDockWidget;
    QDockWidget* fileIoDockWidget;
    QDockWidget* outputLogDockWidget;
    QDockWidget* motorValueEditorDockWidget;
    QDockWidget* motorAddressEditorDockWidget;
    QDockWidget* motorDialsDockWidget;    
    QDockWidget* sensorGrapherDockWidget;

    QThread* workerThread;
    QMutex moveMutex;
};

#endif // MAINWINDOW_H
