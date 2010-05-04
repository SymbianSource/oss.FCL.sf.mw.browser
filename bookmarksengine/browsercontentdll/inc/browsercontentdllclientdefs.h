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


#ifndef BOOKMARKSCLIENTDEFS_H
#define BOOKMARKSCLIENTDEFS_H

#define BOOKMARKSCLIENT_PRIVATE(Class) \
                 private: \
                 inline Class##Private* priv_func() { return reinterpret_cast<Class##Private*>(priv_ptr); } \
                 inline const Class##Private* priv_func() const{ return static_cast<Class##Private*>(priv_ptr);} \
                 friend class Class##Private; \
                 void* priv_ptr;
#define BOOKMARKSCLIENT_PUBLIC(Class) \
        public: \
                inline Class* pub_func() { return static_cast<Class *>(pub_ptr); } \
                inline const Class* pub_func() const { return static_cast<const Class *>(pub_ptr); } \
        private: \
                friend class Class; \
                void* pub_ptr;

#define BOOKMARKSCLIENT_INITIALIZE(Class) \
                 priv_ptr=new Class##Private(); \
                 Class##Private* priv=priv_func();\
                 priv->pub_ptr = this;
#define BOOKMARKSCLIENT_PUBLICPTR(Class) Class * const pub = pub_func()
#define BOOKMARKSCLIENT_PRIVATEPTR(Class) Class##Private* const priv=priv_func()
// #define BOOKMARKSENGINECLIENT_EXPORT Q_DECL_EXPORT


#ifdef BUILDING_BOOKMARK_CONTENTDLL
    #define BOOKMARKSCONTENTDLL_EXPORT Q_DECL_EXPORT
#else
    #define BOOKMARKSCONTENTDLL_EXPORT Q_DECL_IMPORT
#endif 

enum EBrowserContentErrorTypes
    {
    ErrNone,
    ErrGeneral = -2
    };
#endif //BOOKMARKSCLIENTDEFS_H
