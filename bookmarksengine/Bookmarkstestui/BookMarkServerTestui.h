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

#include<QtGui>
class BookmarksClientSide;

class BookMarkServerTestui: public QWidget
{
 Q_OBJECT
public:
    BookMarkServerTestui();
    ~BookMarkServerTestui();
    
private:
    BookmarksClientSide* obj;
 public slots:
    void buttonclicked();
    void delbutton();
    void delyahoo();
    void delnokia();
    void fetchbutton();
    void closeAll();
    void yahooclicked();
    void nokiaclicked();
 public: 
     void getop();
     
};
