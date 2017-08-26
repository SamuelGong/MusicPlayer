#ifndef MYPLAYLIST_H
#define MYPLAYLIST_H
#include <QString>
#include <iostream>
using namespace std;
enum{CurrentItemOnce, CurrentItemInLoop, Sequential, Loop};

class node
{
    friend class myPlayList;
private:
    QString curSong;
    node *previous;
    node *next;
public:
    node(QString path):curSong(path), previous(NULL), next(NULL){}
};

class myPlayList
{
private:
    node *curNode;
    node *head;
    node *tail;
    int num;
public:
	myPlayList() { head = tail = new node(""); head->previous = NULL; head->next = NULL; num = 0; curNode = NULL; }
    virtual ~myPlayList();
    QString getTail();
    QString getNext();
    QString getPre();
    QString getCur();
    void addMedia(QString path);
    int currentIndex();
    void setCurrentIndex(int index);
    void clear();
    void removeMedia(int index);
};

#endif // MYPLAYLIST_H
