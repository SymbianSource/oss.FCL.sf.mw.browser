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

isEmpty(_BROWSER_PRI_INCLUDED_): {
    
    _BROWSER_PRI_INCLUDED_=1

    symbian {
        CONFIG += debug_and_release
        MMP_RULES += EXPORTUNFROZEN
    }
   
    CONFIG(debug, debug|release) {
        SUBDIRPART = Debug
    } else {
        SUBDIRPART = Release
    }
    
    # Figure out the root of where stuff should go (this could be done via configure)
symbian {
    OUTPUT_DIR = $$PWD
} else {
    OUTPUT_DIR = $$PWD/../../..
}
    OBJECTS_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    MOC_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    RCC_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    UI_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    TEMPDIR= $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/build
    DESTDIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/bin
    QMAKE_RPATHDIR = $$DESTDIR $$QMAKE_RPATHDIR
    QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR
    
    # Add the output dirs to the link path too
    LIBS += -L$$DESTDIR
    
    DEPENDPATH += .
    INCLUDEPATH += .

    symbian {
        BROWSER_INCLUDE = $$MW_LAYER_PUBLIC_EXPORT_PATH(browser)
    } else {
        BROWSER_INCLUDE = $$PWD/include/
    }
    
    QMAKE_CXXFLAGS_DEBUG += -g
    
    CONFIG(gcov)   {
       LIBS +=   -lgcov
       QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    }
}

