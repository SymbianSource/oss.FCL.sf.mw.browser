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


#ifndef BOOKMARKSCOMMONENGINE_H
#define BOOKMARKSCOMMONENGINE_H


#define BOOKMARKSENGINESERVER "BookMarkServer"

#ifdef Q_OS_LINUX 
#define BOOKMARKSENGINESERVEREXE "./BookMarkServer"
#else
#define BOOKMARKSENGINESERVEREXE "BookMarkServer.exe"
#endif

#define BOOKMARKSENGINESEPARATOR  ";"

    enum BookmarksRequestTypes
        {
        EDeleteBookmark,
        EAddBookmark,
        EFetchBookmark,
        EFetchAllBookmark,
        EUnknownrequest
        };
    enum EServerResponseTypes
        {
        EServerMsgBookmarkDeleted,
        EServerMsgBookmarkAdded,
        EServerMsgBookmarkFetched,
        EServerMsgBookmarkAllFetched,
        EServerMsgUnknown
        };
    enum EBookmarkErrorTypes
        {
        ErrNone,
        ErrGeneral=-2
        };
#endif //BOOKMARKSCOMMONENGINE_H
