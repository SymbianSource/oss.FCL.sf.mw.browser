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

TEMPLATE = lib
TARGET = browsercontentdll 

ROOT_DIR = $$PWD/../..
include($$ROOT_DIR/browser.pri)

DEFINES+= BUILDING_BOOKMARK_CONTENTDLL

QT += core network xml sql
HEADERS += $$PWD/inc/browsercontentdllclientdefs.h \
           $$PWD/inc/browsercontentdll.h
           
SOURCES += $$PWD/src/browsercontentdll.cpp 

isEmpty(BEDROCK_OUTPUT_DIR): {
    symbian {
        CONFIG(release, debug|release):BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR=$$PWD/../../WrtBuild/Release
        CONFIG(debug, debug|release):BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR= $$PWD/../../WrtBuild/Debug
    } else {
        CONFIG(release, debug|release):BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR=$$PWD/../../../../../WrtBuild/Release
        CONFIG(debug, debug|release):BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR= $$PWD/../../../../../WrtBuild/Debug
    }     
} else {
    BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR = $$BEDROCK_OUTPUT_DIR
}
     
OBJECTS_DIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/browsercontentdll/tmp
DESTDIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/bin
MOC_DIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/browsercontentdll/tmp
RCC_DIR = $$BOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/browsercontentdll/tmp
TEMPDIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/browsercontentdll/build
# QMAKE_LIBDIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/bin

INCLUDEPATH += $$PWD/inc
 

#I believe the following line to be useless on all platforms. (carol.szabo@nokia.com)
# !s40:LIBS += -Llib
# CONFIG += \
#    building-libs \
#    depend_includepath \
#    dll

CONFIG += dll 

# CONFIG(release, debug|release):!CONFIG(QTDIR_build){
#    !unix : contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols
#    unix : contains(QT_CONFIG, reduce_relocations): CONFIG += bsymbolic_functions
# }

CONFIG -= warn_on
*-g++* : QMAKE_CXXFLAGS += -Wreturn-type -fno-strict-aliasing
CONFIG(gcov)   {
   LIBS +=   -lgcov
   QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
   message( "building for coverage statics" )
}




isEmpty(TEMPDIR) {
    CONFIG(release, debug|release):TEMPDIR=$$DESTDIR/Release/build
    CONFIG(debug, debug|release):TEMPDIR=$$DESTDIR/Debug/build
}

CONFIG (maemo) {
    include(../../../../cwrt-maemo.pri)
} 

symbian: {
    TARGET.UID3 = 0x200267E6
    TARGET.VID = VID_DEFAULT
    TARGET.EPOCALLOWDLLDATA=1
    TARGET.CAPABILITY = All -TCB -DRM -AllFiles
    LIBS += -lefsrv -lcaf -lcafutils
    INCLUDEPATH+=$$MW_LAYER_PUBLIC_EXPORT_PATH(cwrt) $$MW_LAYER_PUBLIC_EXPORT_PATH()
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH+=/epoc32/include
    browsercontentdlllibs.sources = browsercontentdll.dll
    browsercontentdlllibs.path = /sys/bin
    DEPLOYMENT += browsercontentdlllibs
 
}
