#include "myplaylist.h"

myPlayList:: ~myPlayList()
{
    if(head->next != NULL)
        clear();
    delete(head);
}

QString myPlayList::getTail()
{
    return tail->curSong;
}

QString myPlayList::getNext()
{
	if (head->next == NULL)
		return NULL;
	if (curNode != NULL)
		curNode = curNode->next;
	if (curNode == NULL)
		curNode = head->next;
	return curNode->curSong;
}

QString myPlayList::getCur()
{
	if (curNode == NULL)
		return NULL;
	return curNode->curSong;
}

QString myPlayList::getPre()
{
	if (head->next == NULL)
		return NULL;
	if (curNode != head)
		curNode = curNode->previous;
	if (curNode == head)
		curNode = tail;
	return curNode->curSong;
}


void myPlayList::addMedia(QString path)
{
    node *tmpNode = new node(path);
    tmpNode->previous = tail;
    tail->next = tmpNode;
    tail = tmpNode;
    num++;
}

int myPlayList::currentIndex()
{
    int i;
    node *tmpNode = head->next;
    for(i=0; tmpNode!=curNode && tmpNode != NULL; i++)
        tmpNode = tmpNode->next;
    return i;
}

void myPlayList::setCurrentIndex(int index)
{
    int i;
    node *tmpNode = head->next;
    if(index > num)
        return;
    for(i=0; i<index; i++)
        tmpNode = tmpNode->next;
    curNode = tmpNode;
}

void myPlayList::clear()
{
    int i;
    node *tmpNode = head->next, *nxtNode;
    for(i=1; tmpNode!=NULL; i++)
    {
        nxtNode = tmpNode->next;
        delete tmpNode;
        tmpNode = nxtNode;
    }
    head->next = NULL;
    num = 0;
}

void myPlayList::removeMedia(int index)
{
	int i;
	node *tmpNode = head->next, *preNode = head;
	if (index > num)
		return;
	cout << index << endl;
	for (i = 0; i<index; i++)
	{
		preNode = tmpNode;
		tmpNode = tmpNode->next;
	}
	if (tail == tmpNode)
		tail = preNode;
	preNode->next = tmpNode->next;
	if (tmpNode->next != NULL)
		tmpNode->next->previous = preNode;
	delete tmpNode;
	num--;
}
