/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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


#include<BookMarkServerTestui.h>
#include <bookmarkclient.h>
//#include<bookmarks.h>
BookMarkServerTestui::BookMarkServerTestui()
    {
    int sessionId=0;
    obj=new BookmarksClientSide();
    obj->connectToServer();
    bool res=obj->getSessionId(sessionId);
    }
BookMarkServerTestui::~BookMarkServerTestui()
    {
    obj->closeServer();
    delete(obj);
    obj=NULL;
    }
void BookMarkServerTestui::buttonclicked()
{
 getop();

}
void BookMarkServerTestui::getop()
    {
    BookmarkLeaf* col=new BookmarkLeaf();
  /*  col->setTitle("TestT1");
    col->setUrl("http://www.google.com");
    QDate x(2010,01,19);
    col->setDate(x);
    col->setTag("GOOGLY");*/
    QDate x(2010,01,19);
    col->setUrl("http://www.google.com");
    col->setTitle("TestT1");
    col->setDate(x);
    col->setTag("GOOGLY");
    obj->AddBookmark(col);


    }
void BookMarkServerTestui::delbutton()
    {
    
    obj->DeleteBookmark("TestT1");
    }
void BookMarkServerTestui::fetchbutton()
    {
    
    //BookmarkLeaf* fin=obj->FetchBookmark("first");
    }
void BookMarkServerTestui::closeAll()
    {
     //obj->closeServer();
     
    }
void BookMarkServerTestui::yahooclicked()
    {
    /*BookmarkLeaf* BookmarkContent=new BookmarkLeaf();
    BookmarkContent->setBookmarkTitle("second");
    BookmarkContent->setBookmarkUrl("www.yahoo.com");
    BookmarkContent->setLastUpdatedTime("11:10");
    BookmarkContent->setBookmarkTag("HHHHHHO");*/
    BookmarkLeaf* col=new BookmarkLeaf();
    QDate x(2010,01,19);
    col->setUrl("http://www.yahoo.com");
    col->setTitle("TestT2");
    col->setDate(x);
    col->setTag("YAH");
    obj->AddBookmark(col);
    
    }
void BookMarkServerTestui::nokiaclicked()
    {
    BookmarkLeaf* col=new BookmarkLeaf();
       QDate x(2010,01,19);
       col->setUrl("http://www.NOKIA.com");
       col->setTitle("TestT3");
       col->setDate(x);
       col->setTag("NOK");
    obj->AddBookmark(col);
    
    }
void BookMarkServerTestui::delyahoo()
    {
    obj->DeleteBookmark("TestT2");
    }
void BookMarkServerTestui::delnokia()
    {
    obj->DeleteBookmark("TestT3");
    }
