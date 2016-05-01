#include "customlistwidget.h"
#include <qt5/QtCore/QDir>
#include <qt5/QtCore/QFile>
#include <qt5/QtCore/QString>
#include <qt5/QtWidgets/QGridLayout>
#include <qt5/QtWidgets/QLabel>
#include <qt5/QtWidgets/QAbstractItemView>
#include <qt5/QtWidgets/QFileDialog>
#include <qt5/QtWidgets/QMessageBox>
#include <qt5/QtWidgets/QAbstractButton>


CustomListWidget::CustomListWidget(QList<RobotPose> posesList, QString title,
                                   bool allowDuplNames, QWidget *parent) :
    QWidget(parent)
{
    QGridLayout* layout = new QGridLayout;
    setLayout(layout);

    QLabel* label = new QLabel(title);

    mRobotPosesListModel = new RobotPosesListModel(posesList, allowDuplNames);

    mListView = new QListView(this);
    mListView->setModel(mRobotPosesListModel);
    mListView->setSelectionMode(QAbstractItemView::SingleSelection);

    moveUpButton = new QPushButton("Move up");
    moveDownButton = new QPushButton("Move down");

    int row = 0;
    layout->addWidget(label, row++, 0, 1, -1);
    layout->addWidget(mListView, row++, 0, 1, -1);
    layout->addWidget(moveUpButton, row, 0);
    layout->addWidget(moveDownButton, row++, 1);

    customiseLayout();

    connect( moveUpButton, SIGNAL(clicked()), this, SLOT(moveUp()) );
    connect( moveDownButton, SIGNAL(clicked()), this, SLOT(moveDown()) );
}


void CustomListWidget::add(RobotPose robotPose)
{
    QModelIndex index = mListView->currentIndex();
    int err = mRobotPosesListModel->add(robotPose, index);
    if (err == 1)
    {
        QMessageBox::warning(this, "Failed to add pose",
                             "Cannot add pose with empty name.");
        return;
    }
    else if (err == 2)
    {
        QMessageBox::warning(this, "Failed to add pose",
                             "Pose name must be unique.");
        return;
    }
    mListView->setCurrentIndex(index);
}


void CustomListWidget::addFrom(CustomListWidget* otherCustomListWidget)
{
    QModelIndex index = mListView->currentIndex();
    QModelIndex otherIndex = otherCustomListWidget->getListView()->currentIndex();
    mRobotPosesListModel->add(otherCustomListWidget->getRobotPosesListModel()->getCurrentPose(otherIndex), index);
    mListView->setCurrentIndex(index);
}


void CustomListWidget::remove()
{
    QModelIndex index = mListView->currentIndex();
    mRobotPosesListModel->remove(index);
    mListView->setCurrentIndex(index);
}


void CustomListWidget::moveUp()
{
    QModelIndex index = mListView->currentIndex();
    mRobotPosesListModel->moveUp(index);
    mListView->setCurrentIndex(index);
}


void CustomListWidget::moveDown()
{
    QModelIndex index = mListView->currentIndex();
    mRobotPosesListModel->moveDown(index);
    mListView->setCurrentIndex(index);
}


void CustomListWidget::savePosesFile()
{
    //QString fileName = QFileDialog::getSaveFileName(this, "Save poses", QDir::homePath(), "Pose files (*.txt)");
    QFileDialog dialog(this, "Save poses", QDir::homePath());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Pose files (*.txt)");
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("txt");
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    mRobotPosesListModel->savePosesFile(fileName);
}


void CustomListWidget::loadPosesFile()
{
    //QString fileName = QFileDialog::getOpenFileName(this, "Load poses", QDir::homePath(), "Pose files (*.txt)");
    QFileDialog dialog(this, "Load poses", QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Pose files (*.txt)");
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    QModelIndex index;
    mRobotPosesListModel->loadPosesFile(fileName, index);
    mListView->setCurrentIndex(index);
}


void CustomListWidget::customiseLayout()
{
    moveUpButton->setIcon( QIcon("assets/images/ionicons-2.0.1/src/arrow-up-b.svg") );
    moveDownButton->setIcon( QIcon("assets/images/ionicons-2.0.1/src/arrow-down-b.svg") );

//    QFile file;
//    QString customPushButtonUpIconStyleSheet;
//    QString customPushButtonDownIconStyleSheet;

//    file.setFileName("assets/qss/custompushbuttonupicon.qss");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;
//    customPushButtonUpIconStyleSheet.append( QLatin1String(file.readAll()) );
//    file.close();

//    file.setFileName("assets/qss/custompushbuttondownicon.qss");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;
//    customPushButtonDownIconStyleSheet.append( QLatin1String(file.readAll()) );
//    file.close();

//    moveUpButton->setStyleSheet(customPushButtonUpIconStyleSheet);
//    moveDownButton->setStyleSheet(customPushButtonDownIconStyleSheet);
}
