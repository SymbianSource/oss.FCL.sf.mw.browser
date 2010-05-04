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


#include<QString>
#include<browsercontentdll.h>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
const QString dbLocation="browserContent.db";

class BrowserContentPrivate
    {
BOOKMARKSCLIENT_PUBLIC(BrowserContent)
public:
    ~BrowserContentPrivate();
    QString m_connectionName;
    };

/**==============================================================
 * Description: Constructor of BrowserContentPrivate
 ================================================================*/
BrowserContentPrivate::~BrowserContentPrivate()
    {

    }

/**==============================================================
 * Description: Constructor of BrowserContent
 ================================================================*/
BrowserContent::BrowserContent(QString aClientName)
    {
    BOOKMARKSCLIENT_INITIALIZE(BrowserContent);
    
    priv->m_connectionName=aClientName;
    createDatabase();
    }

/**==============================================================
 * Description: creates the database
 ================================================================*/
int BrowserContent::createDatabase()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    
    QSqlDatabase sqlDB = QSqlDatabase::addDatabase("QSQLITE", priv->m_connectionName);
    sqlDB.setHostName("Simulator");
    sqlDB.setDatabaseName(dbLocation);


    if (!sqlDB.open())
        return -1;
    QSqlError error;
	int  err = ErrGeneral;
    
	QFileInfo dbFile(dbLocation);
    if (dbFile.exists() && dbFile.size() == 0)
        {
        QSqlQuery query(sqlDB);
        query.exec(
           "CREATE TABLE BookMarkTable (title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),tags VARCHAR(40),rowindex INTEGER,CONSTRAINT pk_BookMarkTable PRIMARY KEY(title))");
           error=query.lastError();

         if (error.type() == QSqlError::NoError) {
          query.exec("CREATE TABLE HistoryTable (rowindex INTEGER  PRIMARY KEY, title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),atime VARCHAR(40))");
          error=query.lastError();
         }                        
    }
      
    if (error.type() == QSqlError::NoError) {
             return ErrNone;
     }
     else {
            return ErrGeneral;
     }
 
 }
 

/**==============================================================
 * Description: Destrcutor of the BookmarksClientSide
 ================================================================*/
BrowserContent::~BrowserContent()
    {
    }

/**==============================================================
 * Description: adds the bookmark to the database
 ================================================================*/
int BrowserContent::AddBookmark(
        BookmarkLeaf* BookmarkContent)
    {
    	
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    QString title =BookmarkContent->getTitle();
    QString url = BookmarkContent->getUrl();
    QString adate =BookmarkContent->getDate().toString("dd.MM.yyyy");
    QString tags = BookmarkContent->getTag();
    int aIndex=BookmarkContent->getIndex();
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);

    if (db.isOpen())
        {
        QSqlQuery query(db);
        query.prepare("UPDATE BookMarkTable SET rowindex=rowindex+1 WHERE rowindex>=:aIndex");
        query.bindValue(":aIndex", aIndex);
        query.exec();
        error = query.lastError();
		
        query.prepare("INSERT INTO BookMarkTable (title, url, adate, tags, rowindex) "
            "VALUES (:title, :url, :adate, :tags, :aIndex)");

        query.bindValue(":title", QVariant(title));
        query.bindValue(":url", QVariant(url));
        query.bindValue(":adate", QVariant(adate));
        query.bindValue(":tags", QVariant(tags));
		query.bindValue(":rowindex", QVariant(aIndex));
        
        query.exec();
        error = query.lastError();
        }

    if (error.type() == QSqlError::NoError)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }
    }


/**==============================================================
 * Description: deletes the requested bookmark
 ================================================================*/
int BrowserContent::DeleteBookmark(
        QString atitle)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    QString title = atitle;
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    bool ok;
    if (db.isOpen())
        {
        QSqlQuery query(db);
        query.prepare("SELECT title,url,adate,tags,rowindex FROM BookMarkTable WHERE title=:title");
        query.bindValue(":title", title);
        query.exec();
        error = query.lastError();
        query.next();
        int aIndex=query.value(4).toInt(&ok);

        query.prepare("DELETE FROM BookMarkTable WHERE title=:title");
        query.bindValue(":title", title);
        query.exec();
        error = query.lastError();
        if (error.type() == QSqlError::NoError)
            {
            query.prepare("UPDATE BookMarkTable set rowindex=rowindex-1 WHERE rowindex>:aIndex");
            query.bindValue(":aIndex", aIndex);
            query.exec();
            error = query.lastError();
            }
        }
    if (error.type() == QSqlError::NoError)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }

    }


/**==============================================================
 * Description: fetches Allbookmarks From database
 ================================================================*/
QList<BookmarkLeaf*> BrowserContent::FetchAllBookmarks()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    QList<BookmarkLeaf*> nodeslist;
    
    bool dbopen = db.isOpen();
    bool ok;

    if (dbopen)
        {

        QSqlQuery query(db);

        query.prepare("SELECT title,url,adate,tags,rowindex FROM BookMarkTable ORDER BY rowindex");
        query.exec();
        QSqlError error = query.lastError();

        while (query.next())
            {

            QString title = query.value(0).toString();
            QString url = query.value(1).toString();
            QString date = query.value(2).toString();
            QString tag = query.value(3).toString();
            int aIndex=query.value(4).toInt(&ok);
            BookmarkLeaf* node = new BookmarkLeaf();
            QDate adate = QDate::fromString(date, "dd.MM.yyyy");
            node->setTitle(title);
            node->setUrl(url);
            node->setDate(adate);
            node->setTag(tag);
            node->setIndex(aIndex);
            nodeslist.append(node);         
            }
        }
       return nodeslist;

    }

/**==============================================================
 * Description: Reoders the Bokmarks based on index
 ================================================================*/
int BrowserContent::reorderBokmarks(QString title,int new_index)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
      QSqlQuery query(db);
      QSqlError error;
      bool ok;      
      query.prepare("SELECT title,url,adate,tags,rowindex FROM BookMarkTable WHERE title=:title");
      query.bindValue(":title", title);
      query.exec();
      error = query.lastError();
      query.next();
      int old_index=query.value(4).toInt(&ok);
      
      if(old_index>new_index)
          {
          //Moving the item UP
          query.prepare("UPDATE BookMarkTable set rowindex=rowindex+1 WHERE rowindex BETWEEN :new_index AND :old_index-1");
                    }
      else if(old_index<new_index)
          {
            //Moving items Down
             query.prepare("UPDATE BookMarkTable set rowindex=rowindex-1 WHERE rowindex BETWEEN :old_index+1 AND :new_index");
           }
      query.bindValue(":old_index", old_index);
      query.bindValue(":new_index", new_index);
      //Moving items Down
      query.exec();
      error= query.lastError();

      if (error.type() == QSqlError::NoError)
          {
           query.prepare("UPDATE BookMarkTable set rowindex=:new_index WHERE title=:title");
           query.bindValue(":title", title);
           query.bindValue(":new_index", new_index);
           query.exec();
           error = query.lastError();
          }

      if (error.type() == QSqlError::NoError)
          {
              return ErrNone;
          }
      else
          {
          return ErrGeneral;
          }
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**==============================================================
 * Description: adds the bookmark to the database
 ================================================================*/
int BrowserContent::AddHistory(
        HistoryLeaf* HistoryContent)
    {
    	
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    QString title =HistoryContent->getTitle();
    QString url = HistoryContent->getUrl();
    QString adate =HistoryContent->getDate().toString("dd.MM.yyyy");
    QString atime = HistoryContent->getLastVisited().toString("h:mm ap");
//    int aIndex=HistoryContent->getIndex();
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);

    if (db.isOpen())
        {
        QSqlQuery query(db);
       // query.prepare("UPDATE HistoryTable SET rowindex=rowindex+1 WHERE rowindex>=:aIndex");
        //query.bindValue(":aIndex", aIndex);
        //query.exec();
        //error = query.lastError();
		
		//"CREATE TABLE HistoryTable (rowindex INTEGER  PRIMARY KEY, title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),atime VARCHAR(40))");

        query.prepare("INSERT INTO HistoryTable (rowindex,title, url, adate, atime) "
            "VALUES (NULL,:title, :url, :adate, :atime)");

		//query.bindValue(":rowindex", QVariant(aIndex));
        query.bindValue(":title", QVariant(title));
        query.bindValue(":url", QVariant(url));
        query.bindValue(":adate", QVariant(adate));
        query.bindValue(":atime", QVariant(atime));
        query.exec();
        error = query.lastError();
        }

    if (error.type() == QSqlError::NoError)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }
    }


/**==============================================================
 * Description: fetches History From database
 ================================================================*/
QList<HistoryLeaf*> BrowserContent::FetchHistory()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    QList<HistoryLeaf*> nodeslist;
    
    bool dbopen = db.isOpen();
    bool ok;

    if (dbopen)
        {

        QSqlQuery query(db);

        query.prepare("SELECT title,url,adate,atime,rowindex FROM HistoryTable ORDER BY rowindex");
        query.exec();
        QSqlError error = query.lastError();

        while (query.next())
            {

            QString title = query.value(0).toString();
            QString url = query.value(1).toString();
            QString date = query.value(2).toString();
            QString time = query.value(3).toString();
            int aIndex=query.value(4).toInt(&ok);
            HistoryLeaf* node = new HistoryLeaf();
            QDate adate = QDate::fromString(date, "dd.MM.yyyy");
			QTime atime = QTime::fromString(time, "h:mm ap");
            node->setTitle(title);
            node->setUrl(url);
            node->setDate(adate);
            node->setLastVisited(atime);
//            node->setTag(tag);
            node->setIndex(aIndex);
            nodeslist.append(node);         
            }
        }
       return nodeslist;

    }

/**==============================================================
 * Description: Clear all rows  From History database
 ================================================================*/
int BrowserContent::clearHistory()
{
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    bool ok;
    if (db.isOpen()){
        QSqlQuery query(db);
        query.prepare("DELETE  FROM  HistoryTable" );
        
		query.exec();
        error = query.lastError();
        
		if (error.type() == QSqlError::NoError)	{
			return ErrNone;
		}
		else{
			return ErrGeneral;
		}
    }
}
