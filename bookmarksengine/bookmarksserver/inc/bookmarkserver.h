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

#ifndef BOOKMARKSERVER_H
#define BOOKMARKSERVER_H

#include <QHash>
#include <serviceipcobserver.h>
#include <serviceipcserver.h>
#define BOOKMARKDATAMODELSERVER "BookMarkServer"

const QString dbLocation="BookMarkDatabase.db";



//using namespace WRT;
class BookMarkServer: public QObject, public WRT::MServiceIPCObserver
    {
    Q_OBJECT
    public:
        BookMarkServer();
        virtual ~BookMarkServer();
        enum BookmarkRequestTypes
            {
            EDeleteBookMark,
            EAddBookMark,
            EFetchBookmark,
            EFetchAllBookmarks,
            EUnknownrequest
            };
        enum BookmarkResponseTypes
            {
            EDeleteBookMarkDone,
            EAddBookMarkDone,
            EFetchBookmarkDone,
            EFetchAllBookmarksDone,
            EUnknownresponse
            };


    private:
        bool handleRequest(WRT::ServiceIPCRequest *aRequest);
        void handleCancelRequest(WRT::ServiceIPCRequest *aRequest);
        void handleClientConnect(WRT::ClientInfo *aNewClient);
        void handleClientDisconnect(WRT::ClientInfo *aClient);
    private:
        void fetchBookMarkData(WRT::ServiceIPCRequest *request);
        int createDatabase();
        void addBookMark(WRT::ServiceIPCRequest *request);
        void deleteBookMark(WRT::ServiceIPCRequest *request);
    private slots:
    void closeServer();
    private:
        WRT::ServiceFwIPCServer* m_server;
        QString m_connectionName;
        QHash<int,QString>m_clients;
        int m_sessionCount;
    };
#endif //BOOKMARKSERVER_H
