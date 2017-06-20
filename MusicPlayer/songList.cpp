//
//  songList.cpp
//  songList
//
//  Created by apple on 17/6/10.
//  Copyright © 2017年 apple. All rights reserved.
//

#include "songList.h"

QString song::getName()
{
    return Name;
}

song *song::getNext()
{
    return next;
}

song *songList::findSong(QString &Name)
{
    song *tmpSong = head->next;
    while(tmpSong != NULL && tmpSong->Name != Name)
        tmpSong = tmpSong->next;
    return tmpSong;
}

QString songList::getName()
{
    return Name;
}

songList *songList::getNext()
{
    return next;
}

song *songList::getHead()
{
    return head;
}

int songList::getNum()
{
    return num;
}

void songList::insertSong(song *s)
{
    tail->next = s;
    tail = s;
    num++;
}

void songList::deleteSong(int index)
{
	song *tmp = head, *pre = NULL;
	int i;
	if (index>num || index <= 0)
		return;
	num--;
	for (i = 0; i<index; i++)
	{
		pre = tmp;
		tmp = tmp->next;
	}
	if (tmp == tail)
		tail = pre;
	pre->next = tmp->next;
	delete(tmp);
}

void songList::disposeList()
{
    song *tmp = head->next, *nxt;
    while(tmp != NULL)
    {
        nxt = tmp->next;
        delete(tmp);
        tmp = nxt;
    }
    head->next = NULL;
//    if(tmp != NULL)
//        delete(tmp);
}

void songList::outputList(QFile *fp)
{
    song *tmp = head;
    QTextStream txtOutput(fp);
    while(tmp != NULL)
    {
        txtOutput<<tmp->Name<<'@';
        tmp = tmp->next;
    }
}

songList *playList::getHead()
{
    return head;
}

songList *playList::findList(int index)
{
    songList *tmpList = head->next;
    int i;
    if(index > num)
        return NULL;
    for(i=1; i<index; i++)
        tmpList = tmpList->next;
    return tmpList;
}

void playList::setNum(int num)
{
    this->num = num;
}

int playList::getNum()
{
    return num;
}

int playList::getcurList()
{
    return curList;
}

void playList::setcurList(int curList)
{
    this->curList = curList;
}

int playList::getviewList()
{
    return viewList;
}

void playList::setviewList(int viewList)
{
    this->viewList = viewList;
}

void playList::insertList(songList *s)
{
    tail->next = s;
    tail = s;
    num++;
}

void playList::deleteList(int index)
{
	songList *tmp = head, *pre = NULL;
	int i;
	if (index>num || index <= 0)
		return;
	num--;
	for (i = 0; i<index; i++)
	{
		pre = tmp;
		tmp = tmp->next;
	}
	if (tmp == tail)
		tail = pre;
	pre->next = tmp->next;
	tmp->disposeList();
}

void playList::outputList(QFile *fp)
{
    songList *tmpList = head->next;
    QTextStream txtOutput(fp);
    if(!fp->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }
    while(tmpList != NULL)
    {
        tmpList->outputList(fp);
        txtOutput<<endl;
        tmpList = tmpList->next;
    }
}

void playList::initialize(QFile *fp)
{
    QTextStream txtInput(fp);
    QString lineStr, tmpSection;
    songList *tmpList;
    song *tmpSong;
    int i;
    if(!fp->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        tmpSection = lineStr.section('@',0,0);
        if(!tmpSection.isEmpty())
        {
            tmpList = new songList(tmpSection);
            this->insertList(tmpList);
            i = 1;
            tmpSection = lineStr.section('@',i,i);
            while(!tmpSection.isEmpty())
            {
                tmpSong = new song(tmpSection);
                tmpList->insertSong(tmpSong);
                i++;
                tmpSection = lineStr.section('@',i,i);
            }
        }
    }
}
