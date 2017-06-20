#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_musicplayer.h"

class MusicPlayer : public QMainWindow
{
	Q_OBJECT

public:
	MusicPlayer(QWidget *parent = Q_NULLPTR);

private:
	Ui::MusicPlayerClass ui;
};
