#include "mainwindow.h"
#include "ui_mainwindow.h"


// 以下常数禁止修改
static const int timeConst = 125; // 采样时间间隔(ms)
static const int barConst = 32;	  // 频谱条个数


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setWindowTitle(QStringLiteral("MusicPlayer"));
    srcFile = "./Picture/123.txt";
    Position = 0;

    QFile fp(srcFile);
	l = new playList;
    l->initialize(&fp);

    setFixedSize(this->width(), this->height());//不允许窗口放大缩小

    QIcon icon("./Picture/icon.png");	//临时（后期用ps设计png格式的图标替换）
    setWindowIcon(icon);				//设置窗口图标

    QPixmap pixmap("./Picture/background.jpg");
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Active,QPalette::Window,QBrush(pixmap));
    setPalette(palette);			//添加背景图片（考虑添加背景图片自定义功能——平铺模式，拉伸模式，固定大小）
    setAutoFillBackground(true);	//BUG：出现弹窗时背景图片会消失
	 

	ui->button_Play->setStyleSheet("background-color:transparent");
	ui->button_Import->setStyleSheet("background-color:transparent");
	ui->button_Next->setStyleSheet("background-color:transparent");
	ui->button_Previous->setStyleSheet("background-color:transparent");
	ui->button_Return->setStyleSheet("background-color:transparent");
	ui->uiSongList->setStyleSheet("background-color:transparent");
	ui->Loop->setStyleSheet("background-color:transparent");
	ui->button_Pause->setStyleSheet("background-color:transparent");
	ui->Sequential->setStyleSheet("background-color:transparent");
	ui->CurrentItemInLoop->setStyleSheet("background-color:transparent");
	ui->CurrentItemOnce->setStyleSheet("background-color:transparent");
	ui->Random->setStyleSheet("background-color:transparent");
	ui->pushButton->setStyleSheet("background-color:transparent");
	ui->Random->setToolTip(tr("Random"));
	ui->Sequential->setToolTip("Sequential");
	ui->Loop->setToolTip(tr("Loop"));
	ui->CurrentItemInLoop->setToolTip("CurrentItemInLoop");
	ui->CurrentItemOnce->setToolTip(tr("CurrentItemOnce"));
	ui->button_Play->hide();


    MyPlayer = new QMediaPlayer(this);
	MyplayList = new myPlayList;
	playBackMode = Loop;//单曲播放模式
	ui->CurrentItemInLoop->hide();
	ui->CurrentItemOnce->hide();
	ui->Random->hide();
	ui->Sequential->hide();

    ui->uiSongList->setColumnCount(1); //表格初始化只有1行
    ui->uiSongList->setRowCount(0); //表格初始化不分列
    ui->uiSongList->setStyleSheet("background-color:rgba(209,238,238,50)"); //最后一位调节widget的透明度
    ui->uiSongList->horizontalHeader()->setStretchLastSection(true);
    ui->uiSongList->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许直接修改表中的内容
    ui->uiSongList->setSelectionBehavior(QAbstractItemView::SelectRows);//一列一列选择
    ui->uiSongList->verticalHeader()->setHidden(true);//隐藏行表头
	ui->uiSongList->setShowGrid(false);
	ui->uiSongList->setWindowFlags(Qt::FramelessWindowHint);


    ui->slider_volme->setRange(0, 100); //音量大小范围
    QObject::connect(ui->slider_volme, SIGNAL(valueChanged(int)), this, SLOT(setVolume())); //根据slider调节音量
    ui->slider_volme->setValue(50); //初始音量大小
	QObject::connect(ui->slider_song, SIGNAL(sliderPressed()), this, SLOT(setPosition())); //点击滑条改变歌曲进度
	QObject::connect(ui->slider_song, SIGNAL(sliderReleased()), this, SLOT(setPosition())); //拖动滑块改变歌曲进度

    QObject::connect(MyPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChange())); //根据slider调节进度条
    QObject::connect(MyPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChange())); //根据进度条调节slider
	MyPlayer->duration();


	lrc = new MyLrc(this);				//歌词

    createAction(); //初始化Action
    createTray(); //初始化托盘菜单
    createMenu(); //初始化右键菜单
	showplayList();
	///////////////// 频谱分析部分 ///////////////////
	//  初始化所用到的对象
	rawData = nullptr;
	wav = new Wave();
	timer = new QTimer();
	SpecGraph = new Spectrum(this);
	this->SpecGraph->move(425, 0);
	// 频谱分析部分额外需要的槽函数
	connect(MyPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(sampling()));
	connect(timer, SIGNAL(timeout()), this, SLOT(slottimedone()));
	this->SpecGraph->show();
	//////////////////负责人： 江志锋/////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
	delete MyPlayer;
	delete  MyplayList;
	delete lrc;
	///////////////// 频谱分析部分 ///////////////////
	delete wav;
	delete timer;
	delete[] rawData;
	delete SpecGraph;
	//////////////////负责人： 江志锋/////////////////
}

void MainWindow::createAction()
{
    action_playStop = new QAction("Play/Pause",this);
    action_playStop->setShortcut(tr("ctrl+p"));
    connect(action_playStop, SIGNAL(triggered()), this, SLOT(PlayOrStop()));
    connect(ui->button_Play, SIGNAL(clicked()), this, SLOT(PlayOrStop())); //开始播放或暂停播放
	connect(ui->button_Pause, SIGNAL(clicked()), this, SLOT(PlayOrStop())); //开始播放或暂停播放


    action_playChoosen = new QAction("PlayChoosen",this);
    action_playChoosen->setShortcut(tr("ctrl+shift+p"));
    connect(action_playChoosen, SIGNAL(triggered()), this, SLOT(playChoosen()));
    //播放选择中的音乐（可以考虑与playStop功能合并）

    action_Import = new QAction("ImportSongs",this);
    action_Import->setShortcut(tr("ctrl+i"));
    connect(action_Import, SIGNAL(triggered()), this, SLOT(Import()));
    connect(ui->button_Import, SIGNAL(clicked()), this, SLOT(Import()));
    //加载mp3文件进播放列表

    action_Next = new QAction("NextSong",this);
    action_Next->setShortcut(tr("ctrl+n"));
    connect(action_Next, SIGNAL(triggered()), this, SLOT(nextSong()));
    connect(ui->button_Next, SIGNAL(clicked()), this, SLOT(nextSong()));
    //下一首

    action_Previous = new QAction("PreSong",this);
    action_Previous->setShortcut(tr("ctrl+l"));
    connect(action_Previous, SIGNAL(triggered()), this, SLOT(preSong()));
    connect(ui->button_Previous, SIGNAL(clicked()), this, SLOT(preSong()));
    //上一首
    connect(ui->uiSongList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemClicked()));
    //双击播放列表播放选中歌单

    action_Exit = new QAction("Exit",this);
    action_Exit->setShortcut(tr("ctrl+e"));
    connect(action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    //退出文件（在窗口缩小或正常显示时变为后台运行，只有托盘能看见时点击才是退出）

    action_Minimize = new QAction("Minimize",this);
    action_Minimize->setShortcut(tr("ctrl+m"));
    connect(action_Minimize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    //最小化窗口

    action_VolumeUp = new QAction("Volume+10%",this);
    action_VolumeUp->setShortcut(tr("up"));
    connect(action_VolumeUp, SIGNAL(triggered()), this, SLOT(VolumeUp()));
    //提高音量（菜单）

    action_VolumeDown = new QAction("Volume-10%",this);
    action_VolumeDown->setShortcut(tr("down"));
    connect(action_VolumeDown, SIGNAL(triggered()), this, SLOT(VolumeDown()));
    //降低音量（菜单）

    action_ChangeMode = new QAction("ChangeMode",this);
    action_ChangeMode->setShortcut(tr("ctrl+c"));
    connect(action_ChangeMode, SIGNAL(triggered()), this, SLOT(ChangeMode()));
    connect(ui->Loop, SIGNAL(clicked()), this, SLOT(ChangeMode()));
    //改变播放模式

	connect(ui->Random, SIGNAL(clicked()), this, SLOT(ChangeMode()));
	//改变播放模式

	connect(ui->Sequential, SIGNAL(clicked()), this, SLOT(ChangeMode()));
	//改变播放模式

	connect(ui->CurrentItemInLoop, SIGNAL(clicked()), this, SLOT(ChangeMode()));
	//改变播放模式

	connect(ui->CurrentItemOnce, SIGNAL(clicked()), this, SLOT(ChangeMode()));
	//改变播放模式


    action_Remove = new QAction("Remove",this);
    action_Remove->setShortcut(tr("delete"));
    connect(action_Remove, SIGNAL(triggered()), this, SLOT(Remove()));

    action_Return = new QAction("Return",this);
    action_Return->setShortcut(tr("ctrl+r"));
    connect(action_Return, SIGNAL(triggered()), this, SLOT(showplayList()));
    connect(ui->button_Return, SIGNAL(clicked()), this, SLOT(showplayList()));

    action_createList = new QAction("createList",this);
    action_createList->setShortcut(tr("ctrl+o"));
    connect(action_createList, SIGNAL(triggered()), this, SLOT(createList()));

	action_LRC = new QAction(("Lrc"), this);
	action_LRC->setShortcut(tr("F2"));//设置开启桌面歌词的播放列表快捷键为F2
	connect(action_LRC, SIGNAL(triggered()), this, SLOT(LrcShow()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LrcShow()));
	connect(MyPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateTime(qint64)));
	//显示歌词

	connect(MyPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(ChangeLrc()));
	//判断歌曲是否改变

}

void MainWindow::createMenu()
{
    ui->uiSongList->addAction(action_playStop);
    ui->uiSongList->addAction(action_playChoosen);
    ui->uiSongList->addAction(action_Import);
    ui->uiSongList->addAction(action_Remove);
    ui->uiSongList->addAction(action_Next);
    ui->uiSongList->addAction(action_Previous);
    ui->uiSongList->addAction(action_VolumeUp);
    ui->uiSongList->addAction(action_VolumeDown);
    ui->uiSongList->addAction(action_ChangeMode);
    ui->uiSongList->addAction(action_Minimize);
    ui->uiSongList->addAction(action_Exit);
    ui->uiSongList->addAction(action_createList);
	ui->uiSongList->addAction(action_LRC);
    ui->uiSongList->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->addAction(action_LRC);
    this->addAction(action_playStop);
    this->addAction(action_playChoosen);
    this->addAction(action_Import);
    this->addAction(action_Next);
    this->addAction(action_Previous);
    this->addAction(action_VolumeUp);
    this->addAction(action_VolumeDown);
    this->addAction(action_ChangeMode);
    this->addAction(action_Minimize);
    this->addAction(action_Exit);
    this->addAction(action_createList);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createTray()
{
    QIcon icon("./Picture/icon.png");
    SystemTray = new QSystemTrayIcon();
    connect(SystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActived(QSystemTrayIcon::ActivationReason)));
    //在双击托盘时显示窗口（单击左键显示托盘菜单会影响灵敏度，考虑将菜单改为单击右键显示）
    SystemTray->setIcon(icon);
    SystemTray->setToolTip("Tray");
    TrayMenu = new QMenu(this);
    action_TrayShow = new QAction("Show", this);
    action_TrayShow->setShortcut(tr("ctrl+s"));
    TrayMenu->addAction(action_TrayShow);
    connect(action_TrayShow, SIGNAL(triggered()), this, SLOT(showNormal()));
    TrayMenu->addAction(action_playStop);
    TrayMenu->addAction(action_Import);
    TrayMenu->addAction(action_Next);
    TrayMenu->addAction(action_Previous);
    TrayMenu->addAction(action_VolumeUp);
    TrayMenu->addAction(action_VolumeDown);
    TrayMenu->addAction(action_ChangeMode);
    TrayMenu->addAction(action_Exit);
    SystemTray->setContextMenu(TrayMenu);
    SystemTray->show();
}

void MainWindow::PlayOrStop()
{
    if(MyPlayer->state() == QMediaPlayer::PausedState)
    {
		ui->button_Pause->hide();
		ui->button_Play->show();
        MyPlayer->setPosition(Position);
        MyPlayer->play();
		resolve_lrc();
    }
    else if(MyPlayer->state() == QMediaPlayer::PlayingState)
    {
		ui->button_Pause->show();
		ui->button_Play->hide();
        Position = MyPlayer->position();
        MyPlayer->pause();
		if (!lrc_map.isEmpty()) 
		{
			lrc->stop_lrc_mask();
		}
    }
	else
	{
		if (l->getviewList() != 0)
		{
			ui->button_Pause->hide();
			ui->button_Play->show();
			playChoosen();
		}
	}
}

void MainWindow::playChoosen()
{
    QModelIndex indexlist = ui->uiSongList->currentIndex();
    int row = indexlist.row();
    if(MyPlayer->state() == QMediaPlayer::StoppedState||MyplayList->currentIndex() != row)
    {
		if (l->getviewList() != l->getcurList())
		{
			MyplayList->clear();
			l->setcurList(l->getviewList());
			songList *tmpList = l->findList(l->getcurList());
			int num = tmpList->getNum();
			song *tmpSong = tmpList->getHead()->getNext();
			for (int j = 0; j<num; j++)
			{
				MyplayList->addMedia(tmpSong->getName());
				tmpSong = tmpSong->getNext();
			}
		}
		MyplayList->setCurrentIndex(row);
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
		preprocess();
		ui->button_Pause->hide();
		ui->button_Play->show();
		MyPlayer->play();
		Position = 0;
    }
    else if(MyPlayer->state() == QMediaPlayer::PausedState)
    {
        MyPlayer->setPosition(Position);
//		preprocess();		// 为了频谱显示的播放前预处理
		ui->button_Pause->hide();
		ui->button_Play->show();
        MyPlayer->play();
    }
}





void MainWindow::Import()//每次开启都进行自动读取（将当前list中的文件路径全部放到一个txt文件中，下次打开自动进行读取）
{
    QModelIndex indexlist = ui->uiSongList->currentIndex();
    int row = indexlist.row(), tarList;
    if(l->getviewList()!=0)
        tarList = l->getviewList();//目标歌单为当前打开个单
    else if(row>=0 && row<l->getNum())
        tarList = row+1;			//目标歌单为当前打开个单
    else
        return;
    int i, num;
    QStringList path=QFileDialog::getOpenFileNames(this,tr("Choose wav files"),".","*.wav");
    songList *tmpList = l->findList(tarList);
    song *tmpSong;
    num = tmpList->getNum();
    for(i=0; i<path.size(); i++)
    {
        if(tmpList->findSong(path[i]) != NULL)
            continue;
        ui->uiSongList->setRowCount(ui->uiSongList->rowCount()+1);
        tmpSong = new song(path[i]);
        tmpList->insertSong(tmpSong);
        if(tarList == l->getviewList())
        {
            QFileInfo fileInfo;
            fileInfo = QFileInfo(path[i]);
            QString fileName = fileInfo.baseName();
            ui->uiSongList->setItem(tmpList->getNum()-1,0, new QTableWidgetItem(fileName));
        }
        if(l->getcurList() == l->getviewList())
			MyplayList->addMedia(path[i]); //如果插入的是正在播放的歌单，则将新歌加入当前播放列表
    }
}

void MainWindow::showplayList()//每次开启都进行自动读取（将当前list中的文件路径全部放到一个txt文件中，下次打开自动进行读取）
{
    int i, num;
    l->setviewList(0);
    num = l->getNum();
    songList *tmpList = l->getHead()->getNext();
    ui->uiSongList->clear();
    QStringList header;
    header<<"Songlist";
    ui->uiSongList->setHorizontalHeaderLabels(header);//设置列表头
    ui->uiSongList->setRowCount(num);
    for(i=0;i<num;i++)
    {
        ui->uiSongList->setItem(i,0, new QTableWidgetItem(tmpList->getName()));
        tmpList = tmpList->getNext();
    }
}

void MainWindow::showsongList(int viewList)
{
    int i, num;
    l->setviewList(viewList);
    ui->uiSongList->clear();
    songList *tmpList = l->findList(viewList);
    num = tmpList->getNum();
    QStringList header;
    header<<tmpList->getName();
    ui->uiSongList->setHorizontalHeaderLabels(header);//设置列表头
    ui->uiSongList->setRowCount(num);
    song *tmpSong = tmpList->getHead()->getNext();
    QFileInfo fileInfo;
    QString fileName;
    for(i=0;i<num;i++)
    {
        fileInfo=QFileInfo(tmpSong->getName());
        fileName=fileInfo.baseName();
        ui->uiSongList->setItem(i,0, new QTableWidgetItem(fileName));
        tmpSong = tmpSong->getNext();
    }
}

void MainWindow::nextSong() //播放下一首歌
{
	QString tmpStr = MyplayList->getNext();
	if (tmpStr != NULL)
	{
		MyPlayer->setMedia(QUrl::fromLocalFile(tmpStr));
		MyPlayer->play();
	}
}

void MainWindow::preSong() //播放上一首歌
{
	QString tmpStr = MyplayList->getPre();
	if (tmpStr != NULL)
	{
		MyPlayer->setMedia(QUrl::fromLocalFile(tmpStr));
		MyPlayer->play();
	}
}

void MainWindow::itemClicked()
{
	qDebug() << l->getviewList();
    if(l->getviewList() == 0)
        listClicked();
	else
		songClicked(); //如果处于歌单界面，被双击的是歌曲，调用函数播放当前歌曲
}

void MainWindow::listClicked()
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int i = indexlist.row();
	l->setviewList(i + 1);
	showsongList(i + 1);
}


void MainWindow::setVolume()
{
    Volume = ui->slider_volme->value();
    MyPlayer->setVolume(Volume);
}

void MainWindow::setPosition()
{
    MyPlayer->setPosition(ui->slider_song->value());
}

void MainWindow::positionChange()
{
	ui->slider_song->setSliderPosition(MyPlayer->position());
	if (MyPlayer->position() != 0 && MyPlayer->position() == MyPlayer->duration())
		checkMode();
}

void MainWindow::durationChange() //一首歌播完以后读取下一首歌的长度
{
    ui->slider_song->setRange(0,MyPlayer->duration());
}

void MainWindow::iconActived(QSystemTrayIcon::ActivationReason reason) //双击托盘
{
    if(reason == QSystemTrayIcon::DoubleClick)
       showNormal();
} //show the window then double-clicking the tray icon

void MainWindow::closeEvent(QCloseEvent *event) //拦截close信号，使部分情况由关闭变为后台运行
{
    if(this->isVisible())
   {
        event->ignore();
        SystemTray->showMessage(tr("hint"), tr("Background operation"));
        this->hide();
    }
    else
    {
        QFile fp(srcFile);
        l->outputList(&fp);
		exit(1);
        event->accept();
    }
}

void MainWindow::VolumeUp() //菜单音量提升
{
    if(Volume > 90)
    {
        Volume = 100;
        SystemTray->showMessage(tr("hint"), tr("Volume can't be greater"));
    }
    else
        Volume += 10;
    ui->slider_volme->setValue(Volume);
    MyPlayer->setVolume(Volume);
}

void MainWindow::VolumeDown() //菜单音量降低
{
    if(Volume < 10)
    {
        Volume = 0;
        SystemTray->showMessage(tr("hint"), tr("Volume can't be smaller"));
    }
    else
        Volume -= 10;
    ui->slider_volme->setValue(Volume);
    MyPlayer->setVolume(Volume);
}

void MainWindow::ChangeMode() //点击按钮或菜单循环改变播放模式
{
    switch(playBackMode)
    {
        case CurrentItemOnce: playBackMode = CurrentItemInLoop; 
			ui->CurrentItemInLoop->show();
			ui->CurrentItemOnce->hide();
		break;
        case CurrentItemInLoop: playBackMode = Sequential;
			ui->Sequential->show();
			ui->CurrentItemInLoop->hide();
		break;
        case Sequential: playBackMode = Loop;
			ui->Loop->show();
			ui->Sequential->hide();
		break;
        case Loop: playBackMode = CurrentItemOnce;
			ui->CurrentItemOnce->show();
			ui->Loop->hide();
		break;
    }

}


void MainWindow::checkMode() //根据当前模式决定下一首播放什么歌
{
	switch (playBackMode)
	{
	case CurrentItemOnce:
		ui->slider_song->setSliderPosition(0); break;
	case CurrentItemInLoop:
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
		preprocess();		// 为了频谱显示的播放前预处理
		MyPlayer->play(); 
		ui->button_Pause->hide();
		ui->button_Play->show(); 
		break;
	case Sequential:
		if (MyplayList->getCur() != MyplayList->getTail()) {
			MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getNext()));
			preprocess();		// 为了频谱显示的播放前预处理
			ui->button_Pause->hide();
			ui->button_Play->show();
			MyPlayer->play();
		}
		else
			ui->slider_song->setSliderPosition(0);
		break;
	case Loop:
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getNext()));
		preprocess();					// 为了频谱显示的播放前预处理
		ui->button_Pause->hide();
		ui->button_Play->show();
		MyPlayer->play(); break;
	}
}

void MainWindow::songClicked() //播放选中音乐
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int row = indexlist.row(), j, num;
	if (l->getviewList() != l->getcurList())
	{
		MyplayList->clear();
		l->setcurList(l->getviewList());
		songList *tmpList = l->findList(l->getcurList());
		num = tmpList->getNum();
		song *tmpSong = tmpList->getHead()->getNext();
		for (j = 0; j<num; j++)
		{
			MyplayList->addMedia(tmpSong->getName());
			tmpSong = tmpSong->getNext();
		}
	}
	MyplayList->setCurrentIndex(row);
	MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
	preprocess();
	ui->button_Pause->hide();
	ui->button_Play->show();
	MyPlayer->play();
	Position = 0;
}

void MainWindow::createList()
{
    bool ok = false;
    QString tmpStr = QInputDialog::getText(NULL,
                tr( "Create a new songlist" ),
                tr( "Please enter the name:" ),
                QLineEdit::Normal, QString::null, &ok);
    if(tmpStr.isEmpty())
    {
        QMessageBox::warning(NULL, "Create a new songlist", "No input", QMessageBox::Ok);
        return;
    }
    songList *tmpList = new songList(tmpStr);
    l->insertList(tmpList);
    showplayList();
}

void MainWindow::Remove() //移除选中的歌单或歌单中的歌曲
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int row = indexlist.row();
	if (l->getviewList() != 0 && row >= 0)
	{
		ui->uiSongList->removeRow(row);
		songList *tmpList = l->findList(l->getviewList());
		tmpList->deleteSong(row + 1);
		if (l->getcurList() == l->getviewList())
		{
			int curIndex = MyplayList->currentIndex();
			if (curIndex == row)
			{
				ui->button_Play->hide();
				ui->button_Pause->show();
				MyPlayer->stop();
			}
			MyplayList->removeMedia(row);
		}
	}
	else if (row > 0)
	{
		ui->uiSongList->removeRow(row);
		cout << row << endl;
		QTableWidgetItem *item = ui->uiSongList->currentItem();
		QString tmpStr = item->text();
		l->deleteList(row + 1);
		if (l->getcurList() == l->getviewList())
			MyplayList->clear();
	}
}


void MainWindow::preprocess() {

	qDebug() << "haha";
	timer->stop();
	ifstream fs;

	QString filePath = MyplayList->getCur();

//	QTextStream cout(stdout, QIODevice::WriteOnly);
	string str;
	str = filePath.toStdString();
	cout << str << endl;

//	/*
	QByteArray temp = filePath.toLatin1();
	fs.open(temp.data(), ios::binary | ios::in);
//	/*	
	// 文件大小
	fs.seekg(0, ios::end);
	wav->file_size = fs.tellg();

	// 数据头是否有附加信息
	bool appExist;
	unsigned int filter;
	fs.seekg(0x10);
	fs.read((char*)&filter, sizeof(unsigned int));
	appExist = filter == 32;

	// 通道数
	fs.seekg(0x16);
	fs.read((char*)&wav->channel, sizeof(wav->channel));

	// 采样频率
	fs.seekg(0x18);
	fs.read((char*)&wav->frequency, sizeof(wav->frequency));

	// 比特率（每秒平均字节数）
	fs.seekg(0x1c);
	fs.read((char*)&wav->Bps, sizeof(wav->Bps));

	// 样本位数
	fs.seekg(0x22);
	fs.read((char*)&wav->sample_num_bit, sizeof(wav->sample_num_bit));

	unsigned long int addr = 0x24;
	// 附加信息
	if (appExist)
		addr += 2;

	// 判断有没有可选信息，没有的话就是数据的开始了
	char flag[5];
	fs.seekg(addr);
	fs.read(flag, 4 * sizeof(char));
	flag[4] = '\0';

	bool isBegin;
	if (flag == "data")
		isBegin = true;
	else
		isBegin = false;

	// 如果有可选信息，寻找真正数据的开头
	if (isBegin == false) {
		addr += 4;
		unsigned int interval;
		fs.seekg(addr);
		fs.read((char*)&interval, sizeof(unsigned int));
		addr += (4 + interval);
	}

	addr += 4;
	fs.seekg(addr);
	fs.read((char*)&wav->data_size, sizeof(wav->data_size));

	if (rawData != nullptr) {
		qDebug() << "delete!";
		delete [] rawData;
	}
	rawData = new unsigned char[wav->data_size];

	addr += 4;
	fs.seekg(addr);
	fs.read((char *)rawData, sizeof(char)*wav->data_size);

	fs.close();

	// 计算时长
	wav->wav_time = double(wav->data_size) / wav->Bps * 1000;
}


void MainWindow::sampling() {
	if (MyPlayer->state() == QMediaPlayer::PlayingState) {
		timer->start(timeConst);
	}
	else if (MyPlayer->state() == QMediaPlayer::PausedState
		|| QMediaPlayer::StoppedState) {
		timer->stop();
	}
	
}

// 定时器超时动作，即完成一次采样并且进行频谱分析和显示
void MainWindow::slottimedone() {
	timer->stop();
	// 为计算待分析的数据地址做准备
	quint64 current = MyPlayer->position();
	unsigned long startFrame = current * wav->data_size / wav->wav_time + wav->Bps / 10;
	short* sampleData;
	unsigned long sampleCount;

	// 根据声道数， 确定每次的采样数
	if (wav->channel == 1) {
		sampleCount = barConst;
		sampleData = new short[barConst];
	}
	else {
		sampleCount = barConst * 2;
		sampleData = new short[2 * barConst];
	}

	// 采样转码，根据大小端规则
	double* power[barConst];	// 结果存放数组，稍后将对这个数据
								// 里面的数据进行分析

								// 如果声道数为1
	if (wav->channel == 1) {
		for (int i = 0; i < sampleCount; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 1;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			//取大端的最高位（符号位），转成补码
			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[i] = (short)data_complement;
		}
	}

	// 如果声道数为2
	else {
		// 通道1
		for (int i = 0; i < sampleCount / 2; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 1;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			//取大端的最高位（符号位），转成补码
			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[2 * i] = (short)data_complement;
		}
		// 通道2
		for (int i = 0; i < sampleCount / 2; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 2;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 3;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			//取大端的最高位（符号位），转成补码
			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[2 * i + 1] = (short)data_complement;
		}
	}
	
	// 进行功率谱分析，最后更新图形
	SpecGraph->powerSpectrum(sampleData, sampleCount, wav->channel, SpecGraph->FFT_result);
	SpecGraph->update();
	delete[] sampleData;
	timer->start(timeConst);
}

void MainWindow::LrcShow() //设置歌词界面
{
	if (lrc->isHidden())
		lrc->show();
	else
		lrc->hide();
}

// 解析LRC歌词，在stateChanged()函数的Phonon::PlayingState处和aboutToFinish()函数中调用了该函数
void MainWindow::resolve_lrc()
{
	lrc_map.clear();
//	index = MyplayList->currentIndex();
//	if (l.getviewList() != 0)
//		file_Name = ui->uiSongList->item(index, 0)->text();

	QString lrc_file_name = file_Name + ".lrc";

	// 打开歌词文件
	QFile file(lrc_file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		lrc->setText(/*QFileInfo(media_object->currentSource().fileName()).baseName()
					 + */tr("LRC NOT EXISTED"));
		return;
	}
	// 设置字符串编码
	QTextStream in(&file);
	QString all_text = QString(in.readAll());
	file.close();
	// 将歌词按行分解为歌词列表
	QStringList lines = all_text.split("\n");

	//这个是时间标签的格式[00:05.54]
	//正则表达式d{2}表示匹配2个数字
	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
	foreach(QString oneline, lines) {
		QString temp = oneline;
		temp.replace(rx, "");//用空字符串替换正则表达式中所匹配的地方,这样就获得了歌词文本
							 // 然后依次获取当前行中的所有时间标签，并分别与歌词文本存入QMap中
							 //indexIn()为返回第一个匹配的位置，如果返回为-1，则表示没有匹配成功
							 //正常情况下pos后面应该对应的是歌词文件
		int pos = rx.indexIn(oneline, 0);
		while (pos != -1) { //表示匹配成功
			QString cap = rx.cap(0);//返回第0个表达式匹配的内容
									// 将时间标签转换为时间数值，以毫秒为单位
			QRegExp regexp;
			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int millisecond = regexp.cap(0).toInt();
			qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
			// 插入到lrc_map中
			lrc_map.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneline, pos);//匹配全部
		}
	}
	// 如果lrc_map为空
	if (lrc_map.isEmpty()) {
		lrc->setText(/*QFileInfo(media_object->currentSource().fileName()).baseName()
					 + */tr("  LRC NOT EXISTED"));
		return;
	}
	//lrc->setText("...");
}

void MainWindow::UpdateTime(qint64 time)
{
	//qint64 total_time_value = MyPlayer-->totalTime();//直接获取该音频文件的总时长参数，单位为毫秒
	qint64 total_time_value = MyPlayer->duration();
	//这3个参数分别代表了时，分，秒；60000毫秒为1分钟，所以分钟第二个参数是先除6000,第3个参数是直接除1s
	//QTime total_time(0, (total_time_value/60000)%60, (total_time_value/1000)%60);
	//QTime current_time(0, (time/60000)%60, (time/1000)%60);//传进来的time参数代表了当前的时间
	//QString str = current_time.toString("mm:ss") + "/" + total_time.toString("mm:ss");
	//time_label->setText(str);


	// 获取当期时间对应的歌词
	if (!lrc_map.isEmpty()) {
		// 获取当前时间在歌词中的前后两个时间点
		qint64 previous = 0;
		qint64 later = 0;
		//keys()方法返回lrc_map列表
		foreach(qint64 value, lrc_map.keys()) {
			if (time >= value) {
				previous = value;
			}
			else {
				later = value;
				break;
			}
		}

		// 达到最后一行,将later设置为歌曲总时间的值
		if (later == 0)
			later = total_time_value;

		// 获取当前时间所对应的歌词内容
		QString current_lrc = lrc_map.value(previous);

		//        // 没有内容时
		//        if(current_lrc.length() < 2)
		//            current_lrc = tr("简易音乐播放器");

		// 如果是新的一行歌词，那么重新开始显示歌词遮罩
		if (current_lrc != lrc->text()) {
			lrc->setText(current_lrc);
			//top_label->setText(current_lrc);
			qint64 interval_time = later - previous;
			lrc->start_lrc_mask(interval_time);
		}
	}
	else 
	{  // 如果没有歌词文件，则在顶部标签中显示歌曲标题
			/*top_label->setText(QFileInfo(media_object->
			currentSource().fileName()).baseName());*/

	}
}

void MainWindow::ChangeLrc()
{
	if (MyPlayer->duration() != 0) {
		index = MyplayList->currentIndex();
		file_Name = ui->uiSongList->item(index, 0)->text();
		resolve_lrc();
	}
}
