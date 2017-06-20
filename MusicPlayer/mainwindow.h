#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAction>
#include <QMenu>
#include <QPalette>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QLineEdit>
#include <QInputDialog>
#include "songList.h"
#include "myplaylist.h"
#include <QAudioFormat>
#include "qaudiodecoder.h"
#include <QCloseEvent>
#include <QAudioBuffer>
#include <QByteArray>
#include <QTimer>
#include "spectrum.h"
#include "wave.h"
#include <QTextCodec>
#include <QTime>
#include "mylrc.h"
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *action_TrayShow;
    QAction *action_playStop;
    QAction *action_playChoosen;
    QAction *action_Import;
    QAction *action_Next;
    QAction *action_Previous;
    QAction *action_Exit;
    QAction *action_Minimize;
    QAction *action_VolumeUp;
    QAction *action_VolumeDown;
    QAction *action_ChangeMode;
    QAction *action_Remove;
    QAction *action_Return;
    QAction *action_createList;


	QAction *action_LRC;

    /*QWinThumbnailToolButton *PlayButton;*/

    QSystemTrayIcon *SystemTray;
    QMenu *TrayMenu;
    QMenu *Menu;
    /*QWinThumbnailToolBar *ToolBar;*/
    QMediaPlayer *MyPlayer;
	myPlayList *MyplayList;
    QString srcFile;
    playList *l;
    int Volume;
	qint64 Position;
	int index;				//歌词
	int songNum;

	MyLrc *lrc;
	QString file_Name;
	QMap<qint64, QString> lrc_map;
	void resolve_lrc();

	int playBackMode;
    void createMenu();
    void createAction();
    void createTray();
    void closeEvent(QCloseEvent *event);
    void listClicked();
//  void changeList(int viewList);
	void checkMode();
	void songClicked();

	///////////////// 频谱绘制模块用到的成员 ///////////////////
	Wave* wav;						// 当前音频文件的元数据
	QTimer* timer;					// 定时器，用于刷新频谱
	unsigned char* rawData;			// 一个文件的所有音频数据
	quint64 currentTime;			// 当前播放时间（ms），为了同步频谱
	Spectrum *SpecGraph;			// 用于处理数据并且绘制频谱图形
	//////////////////////负责人：　江志锋///////////////////////


private slots:
    void PlayOrStop();
    void playChoosen();
    void Import();
    void nextSong();
    void preSong();
    void itemClicked();
    void setVolume();
    void setPosition();
    void positionChange();
    void durationChange();
    void iconActived(QSystemTrayIcon::ActivationReason reason);
    void VolumeUp();
    void VolumeDown();
    void ChangeMode();
    void Remove();
    void showplayList();
    void showsongList(int viewList);
	void createList();

	void LrcShow();
	void UpdateTime(qint64 time);
	void ChangeLrc();


	///////////////// 频谱绘制模块用到的成员函数 ///////////////
	void preprocess();		// 获取音频文件的元数据，并且读取所有音频数据
	void sampling();		// 采样器接口，采样直到播放暂停或停止
	void slottimedone();    // 定时器超时动作，即完成一次采样并且进行频谱分析和显示
	//////////////////// 负责人：江志锋 ///////////////////////
};

#endif // MAINWINDOW_H
