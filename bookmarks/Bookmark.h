/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/
#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "bookmarksapi.h"
class QString;

class BOOKMARKSAPI_EXPORT Bookmark {

public:
    QString url() { return m_url; }
    QString title() { return m_title; }
    int sortIndex() { return m_sortIndex; }
    int id() { return m_id; }
    
    void setUrl(QString url) { m_url = url; }
    void setTitle(QString title) { m_title = title; }
    void setSortIndex(int sortIndex) { m_sortIndex = sortIndex; }

private:    
    QString m_url;
    QString m_title;
    int m_sortIndex;
    int m_id;

};

#endif //BOOKMARK_H
