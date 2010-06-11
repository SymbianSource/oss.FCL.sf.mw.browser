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

#include<QString>
#include<browsercontentdll.h>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
const QString dbLocation="browserContent.db";

class BrowserContentPrivate {
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

    //Check if the table exists, create table only when it's empty 
    QStringList tablelist = sqlDB.tables(QSql::Tables);
    if (tablelist.count() == 0) {
        QSqlQuery query(sqlDB);
        query.exec(
           "CREATE TABLE BookMarkTable (title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),tags VARCHAR(40),rowindex INTEGER, domain VARCHAR(100),CONSTRAINT pk_BookMarkTable PRIMARY KEY(title))");
           error=query.lastError();

        if (error.type() == QSqlError::NoError) {
          query.exec("CREATE TABLE HistoryTable (rowindex INTEGER  PRIMARY KEY, title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),atime VARCHAR(40), domain VARCHAR(100))");
          error=query.lastError();
        }                        
    }
      
    if (error.type() == QSqlError::NoError) {
          return ErrNone;
     } else {
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

    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("UPDATE BookMarkTable SET rowindex=rowindex+1 WHERE rowindex>=:aIndex");
        query.bindValue(":aIndex", aIndex);
        query.exec();
        error = query.lastError();
    
        query.prepare("INSERT INTO BookMarkTable (title, url, adate, tags, rowindex, domain) "
            "VALUES (:title, :url, :adate, :tags, :aIndex, :domain)");

        query.bindValue(":title",   QVariant(title));
        query.bindValue(":url",     QVariant(url));
        query.bindValue(":adate",   QVariant(adate));
        query.bindValue(":tags",    QVariant(tags));
        query.bindValue(":rowindex",QVariant(aIndex));
        query.bindValue(":domain",  QVariant(filterUrl(url)));

        query.exec();
        error = query.lastError();
    }

    if (error.type() == QSqlError::NoError) {
        return ErrNone;
    } else {
        return ErrGeneral;
    }
}

/**==============================================================
 * Description: deletes the requested bookmark
 ================================================================*/
int BrowserContent::DeleteBookmark(QString atitle)
{
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    QString title = atitle;
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    bool ok;
    if (db.isOpen()) {
        QSqlQuery query(db);
    
        if(atitle.contains("'", Qt::CaseInsensitive))
            atitle.replace(QString("'"), QString("''"));

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
        if (error.type() == QSqlError::NoError) {
            query.prepare("UPDATE BookMarkTable set rowindex=rowindex-1 WHERE rowindex>:aIndex");
            query.bindValue(":aIndex", aIndex);
            query.exec();
            error = query.lastError();
        }
    }
        
    if (error.type() == QSqlError::NoError) {
        return ErrNone;
    } else {
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
    
    if (dbopen) {
        QSqlQuery query(db);
        query.prepare("SELECT title,url,adate,tags,rowindex FROM BookMarkTable ORDER BY rowindex");
        query.exec();
        QSqlError error = query.lastError();

        while (query.next()) {
            QString title = query.value(0).toString();
            if(title.contains("'", Qt::CaseInsensitive))
                title.replace(QString("'"), QString("&#39"));
      if(title.contains("\"", Qt::CaseInsensitive))
                title.replace(QString("\""), QString("&#34"));
            QString url = query.value(1).toString();
            if(url.contains("'", Qt::CaseInsensitive))
                url.replace(QString("'"), QString("&#39"));
            if(url.contains("\"", Qt::CaseInsensitive))
                url.replace(QString("\""), QString("&#34"));
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
 * Description: fetches suggested bookmarks from database
 ================================================================*/
QList<BookmarkLeaf*> BrowserContent::suggestBookMarks(QString atitle)
{
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    QList<BookmarkLeaf*> nodeslist;
        
    if (db.isOpen()) {
        QSqlQuery query(db);
        QString queryStatement = "SELECT title,url FROM BookMarkTable WHERE title LIKE '%" + atitle + "%' OR url LIKE '%" + atitle + "%' LIMIT 3";
        query.prepare(queryStatement);
        query.exec();
        QSqlError error = query.lastError();

        while (query.next()) {
            QString title = query.value(0).toString();
            QString url = query.value(1).toString();
            BookmarkLeaf* node = new BookmarkLeaf();
            node->setTitle(title);
            node->setUrl(url);
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
    
    if(title.contains("'", Qt::CaseInsensitive))
      title.replace(QString("'"), QString("''"));

    query.prepare("SELECT title,url,adate,tags,rowindex FROM BookMarkTable WHERE title=:title");
    query.bindValue(":title", title);
    query.exec();
    error = query.lastError();
    query.next();
    int old_index=query.value(4).toInt(&ok);

    if(old_index>new_index) {
          //Moving the item UP
          query.prepare("UPDATE BookMarkTable set rowindex=rowindex+1 WHERE rowindex BETWEEN :new_index AND :old_index-1");
    } else if(old_index<new_index) {
         //Moving items Down
         query.prepare("UPDATE BookMarkTable set rowindex=rowindex-1 WHERE rowindex BETWEEN :old_index+1 AND :new_index");
    }
    
    query.bindValue(":old_index", old_index);
    query.bindValue(":new_index", new_index);
    //Moving items Down
     query.exec();
     error= query.lastError();

    if (error.type() == QSqlError::NoError) {
        query.prepare("UPDATE BookMarkTable set rowindex=:new_index WHERE title=:title");
        query.bindValue(":title", title);
        query.bindValue(":new_index", new_index);
        query.exec();
        error = query.lastError();
    }

    if (error.type() == QSqlError::NoError) {
        //No error
        return ErrNone;
    }  else {
        return ErrGeneral;
    }
}


/**==============================================================
 * Description: modify the requested bookmark
 ================================================================*/
int BrowserContent::modifyBookmark(QString aOrgTitle, QString aNewTitle, QString aNewUrl)
{
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
   
    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("UPDATE BookMarkTable set title=:aNewTitle, url=:aNewUrl, domain=:aNewDomain WHERE title=:aOrgTitle");
        query.bindValue(":aNewTitle", aNewTitle);
        query.bindValue(":aNewUrl", aNewUrl);
        query.bindValue(":aNewDomain", filterUrl(aNewUrl));
        query.bindValue(":aOrgTitle", aOrgTitle);
        query.exec();
        error = query.lastError();
    }

    if (error.type() == QSqlError::NoError) {
        return ErrNone;
    } else {
        return ErrGeneral;
    }
}


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

    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);

    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO HistoryTable (rowindex,title, url, adate, atime, domain) "
            "VALUES (NULL,:title, :url, :adate, :atime, :domain)");

        query.bindValue(":title",  QVariant(title));
        query.bindValue(":url",    QVariant(url));
        query.bindValue(":adate",  QVariant(adate));
        query.bindValue(":atime",  QVariant(atime));
        query.bindValue(":domain", QVariant(filterUrl(url)));
        query.exec();
        error = query.lastError();
    }

    if (error.type() == QSqlError::NoError) {
          return ErrNone;
    } else {
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

    if (dbopen) {
        QSqlQuery query(db);

        query.prepare("SELECT title,url,adate,atime,rowindex FROM HistoryTable ORDER BY rowindex");
        query.exec();
        QSqlError error = query.lastError();

        while (query.next()) {
            QString title = query.value(0).toString();
            if(title.contains("\"", Qt::CaseInsensitive))
                title.replace(QString("\""), QString("&#34"));
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
    
    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("DELETE  FROM  HistoryTable" );
        
    query.exec();
        error = query.lastError();
        
    if (error.type() == QSqlError::NoError) {
      return ErrNone;
    } else {
      return ErrGeneral;
    }
    }
    
    return ErrGeneral;
}

/**==============================================================
 * Description: Clear all rows  From Bookmarks database
 ================================================================*/
int BrowserContent::clearBookmarks()
{
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlError error;
    
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    
    if (db.isOpen()){
        QSqlQuery query(db);
        query.prepare("DELETE  FROM  BookMarkTable" );
        
    query.exec();
        error = query.lastError();
        
    if (error.type() == QSqlError::NoError) {
      return ErrNone;
    }
    else{
      return ErrGeneral;
    }
    }
    
    return ErrGeneral;
}

/**==============================================================
 * Description: fetches suggested History From database
 ================================================================*/
QList<HistoryLeaf*> BrowserContent::suggestHistory(QString atitle){
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    QList<HistoryLeaf*> nodeslist;
    
    bool dbopen = db.isOpen();
    if (dbopen){
        QSqlQuery query(db);
        QString queryStatement = "SELECT title,url FROM HistoryTable WHERE title LIKE '%" + atitle + "%' OR url LIKE '%" + atitle + "%'";
        query.prepare(queryStatement);
        query.exec();
        QSqlError error = query.lastError();

        while (query.next()){
            QString title = query.value(0).toString();
            QString url = query.value(1).toString();
            HistoryLeaf* node = new HistoryLeaf();
            node->setTitle(title);
            node->setUrl(url);
            nodeslist.append(node);         
        }
    }
    return nodeslist;
}
/**==============================================================
 * Description: fetches suggested History and Bookmarks From database
 ================================================================*/
QObjectList BrowserContent::suggestContent(QString atitle){
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    
  while(!suggestedList.isEmpty()) {
        delete suggestedList.takeFirst();
    }
    
    bool dbopen = db.isOpen();
    if (dbopen){
        QSqlQuery query(db);
        
    if(atitle.contains("'", Qt::CaseInsensitive))
      atitle.replace(QString("'"), QString("''"));
 
    QString queryStatement = "SELECT title,url,1 FROM HistoryTable WHERE title LIKE '%" + atitle + "%' OR url LIKE '%" + atitle + "%'" +
                             " UNION " +
                             "SELECT title,url,2 FROM BookMarkTable WHERE title LIKE '%" + atitle + "%' OR url LIKE '%" + atitle + "%'" +
							 "ORDER BY 3";
							 
        
        query.prepare(queryStatement);
        query.exec();
        QSqlError error = query.lastError();

        while (query.next()){
            QString bookmarkTitle = query.value(0).toString();
            QString bookmarkUrl =   query.value(1).toString();
            SuggestData* node = new SuggestData(bookmarkTitle, bookmarkUrl);
            suggestedList.append(node);         
        }
    }
    
  return suggestedList;
}
/**==============================================================
 * Description: removes common strings from URLs
 ================================================================*/
QString BrowserContent::filterUrl(QString atitle){
  QString https = "https://";
  QString http = "http://";
  QString www  = "www";
  QString com  = "com";
  QString org  = "org";
  QString htm  = "htm";
  QString html = "html";

  if(atitle.contains(https, Qt::CaseInsensitive))
    atitle = atitle.remove(https);
  if(atitle.contains(http, Qt::CaseInsensitive))
    atitle = atitle.remove(http);
  if(atitle.contains(www, Qt::CaseInsensitive))
    atitle = atitle.remove(www);
  if(atitle.contains(com, Qt::CaseInsensitive))
    atitle = atitle.remove(com);
  if(atitle.contains(org, Qt::CaseInsensitive))
    atitle = atitle.remove(org);
  if(atitle.contains(html, Qt::CaseInsensitive))
    atitle = atitle.remove(html);
  if(atitle.contains(htm, Qt::CaseInsensitive))
    atitle = atitle.remove(htm);
  
    
  return atitle;
}
/**==============================================================
 * Description: Retrieves the bookmarks and sends it in serialized fashion
 ================================================================*/
QString BrowserContent::FetchSerializedBookmarks()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);

    bool dbopen = db.isOpen();
    bool ok;
    QString bookmakrData = "[";
    if(dbopen)
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
    bookmakrData.append("{");
    bookmakrData.append("\"title\": \"");
    bookmakrData.append(title);
    bookmakrData.append("\", \"urlvalue\": \"");
    bookmakrData.append(url);
    bookmakrData.append("\"},");
        }

    bookmakrData.remove(bookmakrData.length(),1);
    bookmakrData.append("]");


        }
    return bookmakrData;
    }


/**==============================================================
 * Description: Retrieves the History and sends it in serialized fashion
 ================================================================*/
void BrowserContent::FetchSerializedHistory(QVector<QString> &folderVector,QMap<QString,QString> &mymap)
    {

    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);
    QList<HistoryLeaf*> nodeslist;
    int i=0;
    bool dbopen = db.isOpen();
    bool ok;
    QString history = "";

    if (dbopen)
        {

    QSqlQuery query(db);

    query.prepare("SELECT title,url,adate,atime,rowindex FROM HistoryTable ORDER BY rowindex");
    query.exec();
    QSqlError error = query.lastError();
    QString prevtitle="";
    int len=query.numRowsAffected();
    static int count=1;
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

    QString foldertitle=findFolderForDate(adate);


    if(folderVector.contains(foldertitle))
        {
    mymap[prevtitle].append("\"},");
        }
    else
        {
    folderVector.append(foldertitle);
    if(count>1)
        {
    mymap[prevtitle].append("\"}");
    mymap[prevtitle].append ("]");
    mymap[foldertitle].append("[");
        }
    else
        {
    mymap[foldertitle].append("[");
        }
    count++;


        }

    prevtitle=foldertitle;

    mymap[foldertitle].append("{");
    mymap[foldertitle].append("\"titleVal\": \"");
    mymap[foldertitle].append(title);
    mymap[foldertitle].append("\", \"dateVal\": \"");
    mymap[foldertitle].append(adate.toString("dd.MM.yyyy"));
    mymap[foldertitle].append("\", \"urlVal\": \"");
    mymap[foldertitle].append(url);
    mymap[foldertitle].append("\", \"timeVal\": \"");
    mymap[foldertitle].append(atime.toString("h:mm ap"));
        }
    mymap[prevtitle].append("\"}");
    mymap[prevtitle].append ("]");
        }

    }
/**==============================================================
 * Description: Retrieves the bookmark titles and sends it in serialized fashion
 ================================================================*/
void BrowserContent::FetchAllBookmarkTitles(QVector<QString> &title)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BrowserContent);
    QSqlDatabase db = QSqlDatabase::database(priv->m_connectionName);

    bool dbopen = db.isOpen();
    bool ok;
    int i=0;

    if(dbopen)
        {

    QSqlQuery query(db);

    query.prepare("SELECT title FROM BookMarkTable");
    query.exec();
    QSqlError error = query.lastError();

    while (query.next())
        {

    title.append(query.value(0).toString());
        }
        }
    }

/**==============================================================
 * Description: calculate the folder title
 ================================================================*/
QString BrowserContent::findFolderForDate( QDate& nodeDate)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int currentDayOfWeek = currentDateTime.date().dayOfWeek(); 

    int nodeDayOfWeek = nodeDate.dayOfWeek();

    int daysToCurrentDate = nodeDate.daysTo(currentDateTime.date());

    //Check if date to belongs to "ToDay" Folder
    if(nodeDate == currentDateTime.date()){
        QString folder = qtTrId("txt_browser_history_today");
        return folder;
    }
    //Check if date to belongs to "YesterDay" Folder
    if(nodeDate.addDays(1) == currentDateTime.date() ){
        QString folder = qtTrId("txt_browser_history_yesterday");
        return folder;
    }

    //Check if date to belongs to current week folder
    //Should disply the day for the current week
    if(daysToCurrentDate < 7  &&  currentDayOfWeek > nodeDayOfWeek ){
        
        QString folder = qtTrId("txt_browser_history_this_week");
        return folder;
    }

    if(dateInThisMonth(nodeDate)){
        QString folder = qtTrId("txt_browser_history_this_month");
        return folder;
    }

   QString folder = nodeDate.toString("dd.MM.yyyy");
   
    return folder; 

}
/**==============================================================
 * Description: calculate whether the date falls with in this month
 ================================================================*/
bool BrowserContent::dateInThisMonth(QDate &date)
{
    QDate currentDate = QDateTime::currentDateTime().date();
    int daysToCurrentDate = currentDate.daysTo(date);

    int currentMonth = currentDate.month();
    int nodeMonth = date.month();

    if(daysToCurrentDate <= 31 && currentMonth == nodeMonth) {
        return true;
    }
    return false;  
   
}
