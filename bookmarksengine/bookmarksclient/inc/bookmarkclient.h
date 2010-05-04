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

#ifndef BOOKMARKCLIENT_H
#define BOOKMARKCLIENT_H

#include<QObject>
#include<QString>
#include<bookmarkclientdefs.h>
#include<QtGui>
//using namespace WRT;

class BookmarksClientSidePrivate;

class BOOKMARKSENGINECLIENT_EXPORT BookmarkLeaf
{
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    QString getTag(){ return m_tag;}
    int getIndex(){ return m_index;}

    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setTag(QString atag){m_tag=atag;}
    void setIndex(int index){m_index=index;}
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

class BOOKMARKSENGINECLIENT_EXPORT BookmarksClientSide : public QObject
    {
Q_OBJECT
BOOKMARKSCLIENT_PRIVATE(BookmarksClientSide)
public:

    BookmarksClientSide();
    ~BookmarksClientSide();
    int connectToServer();
    void closeServer();
    int AddBookmark(BookmarkLeaf* BookmarkContent);
    BookmarkLeaf* FetchBookmark(QString title);
    int DeleteBookmark(QString title);
    int getSessionId(int& BookmarkId);
    QList<BookmarkLeaf*> FetchAllBookmarks();
signals:
    void BookmarkAdded(QString title);
    void BookmarkDeleted(QString title);
private slots:
    void handleServerResponse();
    };
#endif //BOOKMARKCLIENT_H
