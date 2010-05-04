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


#include <QCoreApplication>
#include<BookMarkServerTestui.h>
#include<QtGui>

int main( int argc, char **argv )
{
    QApplication app(argc, argv);
    BookMarkServerTestui obj;
    //QWidget* win=new QWidget();
    QWidget win;
    win.setWindowTitle("BOOKMARKS SERVER TESTUI");
    QPushButton* butadd=new QPushButton("Add Bookmark Google" );
    QPushButton* butadd1=new QPushButton("Add Bookmark yahoo" );
    QPushButton* butadd2=new QPushButton("Add Bookmark Nokia" );
    
    QPushButton* butdel=new QPushButton("del Bookmark Google" );
    QPushButton* butdel1=new QPushButton("del Bookmark Yahoo" );
    QPushButton* butdel2=new QPushButton("del Bookmark Nokia" );
    
    QPushButton* butfetch=new QPushButton("Fetch Bookmark" );
    QPushButton* butexit=new QPushButton("Exit Server" );
    QPushButton* Exitapp=new QPushButton("Exit UI" );
    //Add 
    QObject::connect(butadd,SIGNAL(clicked()),&obj,SLOT(buttonclicked()));
    QObject::connect(butadd1,SIGNAL(clicked()),&obj,SLOT(yahooclicked()));
    QObject::connect(butadd2,SIGNAL(clicked()),&obj,SLOT(nokiaclicked()));
    
    QObject::connect(butdel,SIGNAL(clicked()),&obj,SLOT(delbutton()));
    QObject::connect(butdel1,SIGNAL(clicked()),&obj,SLOT(delyahoo()));
    QObject::connect(butdel2,SIGNAL(clicked()),&obj,SLOT(delnokia()));
    
    
    QObject::connect(butfetch,SIGNAL(clicked()),&obj,SLOT(fetchbutton()));
    //QObject::connect(butexit,SIGNAL(clicked()),&obj,SLOT(closeAll()));
    QObject::connect(Exitapp,SIGNAL(clicked()),&app,SLOT(quit()));
    
    
    QVBoxLayout* lay=new QVBoxLayout();
    lay->addWidget(butadd);
    lay->addWidget(butadd1);
    lay->addWidget(butadd2);
    lay->addWidget(butdel);
    lay->addWidget(butdel1);
    lay->addWidget(butdel2);
    lay->addWidget(butfetch);
    //lay->addWidget(butexit);
    lay->addWidget(Exitapp);

    win.setLayout(lay);
    //win->show();
    win.showFullScreen();
    
/*    butadd->show();    
    butdel->show();
    butfetch->show();*/
    return app.exec();
}

