/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#ifndef BROWSERCONTENT_H
#define BROWSERCONTENT_H

#include<QObject>
#include<QString>
#include<browsercontentdllclientdefs.h>
#include<QtGui>


class BrowserContentPrivate;

class BOOKMARKSCONTENTDLL_EXPORT BookmarkLeaf
{
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    QString getTag(){ return m_tag;}
    int getIndex() {return m_index;}
    
    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setTag(QString atag){m_tag=atag;}
    void setIndex(int aIndex){m_index=aIndex;}
private:    
    QString url;
    QString title;

    /*The last date on which URL is visited*/
    QDate date;
    /*The last time on which URL is visited*/
    QTime lastVisited;

    QString desc;
    QIcon favicon;
    bool expanded;
    QString m_tag;
    int m_index;
    

};

class BOOKMARKSCONTENTDLL_EXPORT HistoryLeaf
{
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    int getIndex() {return m_index;}
    
    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setIndex(int aIndex){m_index=aIndex;}
private:    
    QString url;
    QString title;

    /*The last date on which URL is visited*/
    QDate date;
    /*The last time on which URL is visited*/
    QTime lastVisited;

    QString desc;
    QIcon favicon;
    bool expanded;
    int m_index;
   
};

class BOOKMARKSCONTENTDLL_EXPORT BrowserContent:public QObject
    {
Q_OBJECT
BOOKMARKSCLIENT_PRIVATE(BrowserContent)
public:

    BrowserContent(QString aClientName);
    ~BrowserContent();
    int AddBookmark(BookmarkLeaf* BookmarkContent);
    int DeleteBookmark(QString title);
    QList<BookmarkLeaf*> FetchAllBookmarks();
    int reorderBokmarks(QString title,int new_index);

	int AddHistory(HistoryLeaf* HistoryContent);
	QList<HistoryLeaf*> FetchHistory();
	int clearHistory();

private:
	int createDatabase();
    };
#endif //BROWSERCONTENT_H
