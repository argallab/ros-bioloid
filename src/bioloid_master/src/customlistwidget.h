#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <qt5/QtCore/QString>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QListView>
#include <qt5/QtWidgets/QPushButton>
#include "robotposeslistmodel.h"

class CustomListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomListWidget(QList<RobotPose> posesList, QString title = "List title",
                              bool allowDuplNames = true, QWidget *parent = 0);
    RobotPosesListModel* getRobotPosesListModel() const { return mRobotPosesListModel; }
    QListView* getListView() const { return mListView; }

signals:

public slots:
    void add(RobotPose robotPose);
    void addFrom(CustomListWidget* customListWidget);
    void remove();
    void moveUp();
    void moveDown();
    void savePosesFile();
    void loadPosesFile();

private:
    void customiseLayout();
    QPushButton* moveUpButton;
    QPushButton* moveDownButton;
    RobotPosesListModel* mRobotPosesListModel;
    QListView* mListView;
};

#endif // CUSTOMLISTWIDGET_H
