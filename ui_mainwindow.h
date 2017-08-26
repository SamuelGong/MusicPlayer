/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *button_Previous;
    QPushButton *button_Next;
    QPushButton *button_Import;
    QTableWidget *uiSongList;
    QLabel *label;
    QSlider *slider_volme;
    QSlider *slider_song;
    QPushButton *Random;
    QPushButton *button_Return;
    QPushButton *button_Play;
    QPushButton *button_Pause;
    QPushButton *Sequential;
    QPushButton *CurrentItemInLoop;
    QPushButton *Loop;
    QPushButton *CurrentItemOnce;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(823, 455);
        MainWindow->setStyleSheet(QStringLiteral("border-top-color: rgb(85, 255, 0);"));
        MainWindow->setInputMethodHints(Qt::ImhNone);
        MainWindow->setIconSize(QSize(45, 45));
        MainWindow->setAnimated(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setAutoFillBackground(false);
        centralWidget->setStyleSheet(QStringLiteral(""));
        button_Previous = new QPushButton(centralWidget);
        button_Previous->setObjectName(QStringLiteral("button_Previous"));
        button_Previous->setGeometry(QRect(20, 370, 71, 71));
        button_Previous->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QStringLiteral("./Picture/pre.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_Previous->setIcon(icon);
        button_Previous->setIconSize(QSize(50, 50));
        button_Next = new QPushButton(centralWidget);
        button_Next->setObjectName(QStringLiteral("button_Next"));
        button_Next->setGeometry(QRect(260, 370, 71, 71));
        button_Next->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QStringLiteral("./Picture/next.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_Next->setIcon(icon1);
        button_Next->setIconSize(QSize(50, 50));
        button_Import = new QPushButton(centralWidget);
        button_Import->setObjectName(QStringLiteral("button_Import"));
        button_Import->setGeometry(QRect(390, 270, 31, 27));
        button_Import->setCursor(QCursor(Qt::CrossCursor));
        button_Import->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral("./Picture/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_Import->setIcon(icon2);
        button_Import->setIconSize(QSize(27, 27));
        uiSongList = new QTableWidget(centralWidget);
        uiSongList->setObjectName(QStringLiteral("uiSongList"));
        uiSongList->setEnabled(true);
        uiSongList->setGeometry(QRect(0, 0, 421, 271));
        uiSongList->setMinimumSize(QSize(0, 241));
        uiSongList->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        uiSongList->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 240);"));
        uiSongList->setRowCount(0);
        uiSongList->setColumnCount(0);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(410, 410, 36, 33));
        label->setPixmap(QPixmap(QString::fromUtf8("./Picture/valume.png")));
        label->setScaledContents(true);
        slider_volme = new QSlider(centralWidget);
        slider_volme->setObjectName(QStringLiteral("slider_volme"));
        slider_volme->setGeometry(QRect(460, 418, 91, 19));
        slider_volme->setMinimumSize(QSize(91, 0));
        slider_volme->setMaximumSize(QSize(91, 16777215));
        slider_volme->setCursor(QCursor(Qt::OpenHandCursor));
        slider_volme->setOrientation(Qt::Horizontal);
        slider_song = new QSlider(centralWidget);
        slider_song->setObjectName(QStringLiteral("slider_song"));
        slider_song->setGeometry(QRect(30, 319, 331, 21));
        slider_song->setCursor(QCursor(Qt::OpenHandCursor));
        slider_song->setOrientation(Qt::Horizontal);
        Random = new QPushButton(centralWidget);
        Random->setObjectName(QStringLiteral("Random"));
        Random->setGeometry(QRect(360, 400, 40, 40));
        Random->setCursor(QCursor(Qt::PointingHandCursor));
        Random->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral("./Picture/random.png"), QSize(), QIcon::Normal, QIcon::Off);
        Random->setIcon(icon3);
        Random->setIconSize(QSize(33, 33));
        Random->setCheckable(false);
        button_Return = new QPushButton(centralWidget);
        button_Return->setObjectName(QStringLiteral("button_Return"));
        button_Return->setGeometry(QRect(390, 0, 31, 23));
        button_Return->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon4;
        icon4.addFile(QStringLiteral("./Picture/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_Return->setIcon(icon4);
        button_Return->setIconSize(QSize(60, 60));
        button_Play = new QPushButton(centralWidget);
        button_Play->setObjectName(QStringLiteral("button_Play"));
        button_Play->setEnabled(true);
        button_Play->setGeometry(QRect(137, 360, 91, 81));
        button_Play->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon5;
        icon5.addFile(QStringLiteral("./Picture/pause.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_Play->setIcon(icon5);
        button_Play->setIconSize(QSize(60, 60));
        button_Pause = new QPushButton(centralWidget);
        button_Pause->setObjectName(QStringLiteral("button_Pause"));
        button_Pause->setEnabled(true);
        button_Pause->setGeometry(QRect(137, 360, 91, 81));
        QIcon icon6;
        icon6.addFile(QStringLiteral("./Picture/Play.gif"), QSize(), QIcon::Normal, QIcon::Off);
        button_Pause->setIcon(icon6);
        button_Pause->setIconSize(QSize(60, 60));
        Sequential = new QPushButton(centralWidget);
        Sequential->setObjectName(QStringLiteral("Sequential"));
        Sequential->setGeometry(QRect(360, 400, 40, 40));
        Sequential->setCursor(QCursor(Qt::PointingHandCursor));
        Sequential->setStyleSheet(QStringLiteral(""));
        QIcon icon7;
        icon7.addFile(QStringLiteral("./Picture/sequential.png"), QSize(), QIcon::Normal, QIcon::Off);
        Sequential->setIcon(icon7);
        Sequential->setIconSize(QSize(33, 33));
        Sequential->setCheckable(false);
        CurrentItemInLoop = new QPushButton(centralWidget);
        CurrentItemInLoop->setObjectName(QStringLiteral("CurrentItemInLoop"));
        CurrentItemInLoop->setGeometry(QRect(360, 400, 40, 40));
        CurrentItemInLoop->setCursor(QCursor(Qt::PointingHandCursor));
        CurrentItemInLoop->setStyleSheet(QStringLiteral(""));
        QIcon icon8;
        icon8.addFile(QStringLiteral("./Picture/CurrentItemInLoop.gif"), QSize(), QIcon::Normal, QIcon::Off);
        CurrentItemInLoop->setIcon(icon8);
        CurrentItemInLoop->setIconSize(QSize(33, 33));
        CurrentItemInLoop->setCheckable(false);
        Loop = new QPushButton(centralWidget);
        Loop->setObjectName(QStringLiteral("Loop"));
        Loop->setGeometry(QRect(360, 400, 40, 40));
        Loop->setCursor(QCursor(Qt::PointingHandCursor));
        Loop->setStyleSheet(QStringLiteral(""));
        QIcon icon9;
        icon9.addFile(QStringLiteral("./Picture/loop.png"), QSize(), QIcon::Normal, QIcon::Off);
        Loop->setIcon(icon9);
        Loop->setIconSize(QSize(33, 33));
        Loop->setCheckable(false);
        CurrentItemOnce = new QPushButton(centralWidget);
        CurrentItemOnce->setObjectName(QStringLiteral("CurrentItemOnce"));
        CurrentItemOnce->setGeometry(QRect(360, 400, 40, 40));
        CurrentItemOnce->setCursor(QCursor(Qt::PointingHandCursor));
        CurrentItemOnce->setStyleSheet(QStringLiteral(""));
        QIcon icon10;
        icon10.addFile(QStringLiteral("./Picture/CurrentItemOnce.png"), QSize(), QIcon::Normal, QIcon::Off);
        CurrentItemOnce->setIcon(icon10);
        CurrentItemOnce->setIconSize(QSize(33, 33));
        CurrentItemOnce->setCheckable(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(560, 412, 41, 31));
        pushButton->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon11;
        icon11.addFile(QStringLiteral("./Picture/LRC2.gif"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon11);
        pushButton->setIconSize(QSize(40, 40));
        MainWindow->setCentralWidget(centralWidget);
        button_Previous->raise();
        button_Next->raise();
        button_Import->raise();
        uiSongList->raise();
        label->raise();
        slider_volme->raise();
        slider_song->raise();
        Random->raise();
        button_Return->raise();
        button_Pause->raise();
        button_Play->raise();
        Sequential->raise();
        CurrentItemInLoop->raise();
        Loop->raise();
        CurrentItemOnce->raise();
        pushButton->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        button_Previous->setText(QString());
        button_Next->setText(QString());
        button_Import->setText(QString());
        label->setText(QString());
        Random->setText(QString());
        button_Return->setText(QString());
        button_Play->setText(QString());
        button_Pause->setText(QString());
        Sequential->setText(QString());
        CurrentItemInLoop->setText(QString());
        Loop->setText(QString());
        CurrentItemOnce->setText(QString());
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
