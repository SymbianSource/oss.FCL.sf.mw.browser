#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

TEMPLATE = app
TARGET = Bookmarkstestui 
DEFINES += BUILDING_BOOKMARKS_TESTUI
# establish relative root dir
ROOT_DIR = $$PWD/../..
include($$ROOT_DIR/browser.pri)

QT           += core gui network sql

HEADERS += $$PWD/BookMarkServerTestui.h 
SOURCES += $$PWD/BookMarkServerTestui.cpp \
           $$PWD/main.cpp



isEmpty(BEDROCK_OUTPUT_DIR): {
    symbian {
    	CONFIG(release,release|debug):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../WrtBuild/Release
    	CONFIG(debug,release|debug):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../WrtBuild/Debug

    else {
    	CONFIG(release,release|debug):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$ROOT_DIR/WrtBuild/Release
    	CONFIG(debug,release|debug):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$ROOT_DIR/WrtBuild/Debug
    }    
} else {
    BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$BEDROCK_OUTPUT_DIR
}
CONFIG(release, debug|release):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../../../../WrtBuild/Release
CONFIG(debug, debug|release):BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../../../../WrtBuild/Debug

QMAKE_RPATHDIR += $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/bin
QMAKE_LIBDIR += $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/bin



INCLUDEPATH += $$PWD $$PWD/../bookmarksclient/inc $$PWD/../../browsercore/appfw/Api/Bookmarks $$PWD/../../browsercore/appfw/Api/Common $$PWD/../../browsercore/appfw/Api/Dialogs $$PWD/../../browsercore/appfw/Api/Controls $$PWD/../../browsercore/appfw/Api/Managers  $$PWD/../../browsercore/appfw/Api/Transitions $$PWD/../../browsercore/appfw/Api/Views $$PWD/../../browsercore/appfw/Common 

LIBS += -lBookMarksClient

# Use IPC components from import directory.
include($$PWD/../../../../../import/import.pri)
LIBS += -lwrtserviceipcclient
LIBS += -lwrtserviceipcserver

CONFIG(gcov)   {
   LIBS +=   -lgcov
   QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
   message( "building for coverage statics" )
}

     
OBJECTS_DIR = $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServerTest/tmp
DESTDIR = $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/bin
MOC_DIR = $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServerTest/tmp
RCC_DIR = $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServerTest/tmp
TEMPDIR = $$BOOKMARKTEST_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServerTest/build




CONFIG (maemo) {
    include(../../../../cwrt-maemo.pri)
} 

symbian: {
    TARGET.UID3 = 0x102C2774
    TARGET.VID = VID_DEFAULT
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = All -TCB -DRM -AllFiles
    
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
   LIBS += -lefsrv -lcaf -lcafutils 

}

symbian: {
    INCLUDEPATH +=  $$PWD $$MW_LAYER_SYSTEMINCLUDE $$APP_LAYER_SYSTEMINCLUDE
}
