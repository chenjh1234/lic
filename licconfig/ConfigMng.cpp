#include "ConfigMng.h"
//  ConfigMng=============================

ConfigMng::ConfigMng()
{
}
ConfigMng::~ConfigMng()
{
}
int ConfigMng::loadFile(QString file)
{
   return readFile(file);
}
int ConfigMng::loadStr(QString str)
{
   return readStr(str);
}

int ConfigMng::login(QString packID, QString user, QString node, QString project, int n)
{
   LimitConfig *pl;
   int ir;
   pl = packLimit(packID);
   if (pl == NULL) return LOGIN_PASS; // no this pack??

   ir = LOGIN_FAILED;
   if (pl->canLogin(user, node, project, n))// we have to canLogin before login;
   {
      ir = pl->login(user, node, project, n);
   }

   return ir;
}
int ConfigMng::unlogin(QString packID, QString user, QString node, QString project, int n)
{
   LimitConfig *pl;
   int ir;
   pl = packLimit(packID);
   if (pl == NULL) return LOGIN_PASS; // no this pack??

   ir = pl->unlogin(user, node, project, n);
   return ir;
}
QString ConfigMng::dbFile()
{
   return _fileConfig;
}
void ConfigMng::setDbFile(QString file)
{
    _fileConfig = file;

}
bool ConfigMng::saveDB()
{
   int ir;
   QString dbfile;
   dbfile = dbFile();
   QFile dbf(dbfile);
   //open:
//save to PTR:
   qDebug() << " savedb, file = " << dbfile;
   if (!dbf.open(QIODevice::WriteOnly | QIODevice::Truncate)) return DSTAT_FAILED;
   QDataStream ds(&dbf);
   ir = doDB(DO_SAVEDB,ds);
   dbf.close();

   return ir;
}
bool ConfigMng::loadDB()
{
   int ir;
   QString dbfile;
   dbfile = dbFile();
   QFile dbf(dbfile);
   qDebug() << " loaddb, file = " << dbfile;
   //open:
//save to PTR:
   ir = 1;
   if (!dbf.open(QIODevice::ReadOnly ))
   {
        qDebug() << "DB file no exist: " << dbfile;
        qDebug() << "no Config DB loaded";
   }
   else
   {
       QDataStream ds(&dbf);
       ir = doDB(DO_LOADDB,ds);
       dbf.close();
    }

   return ir;

}
int  ConfigMng::doDB(int ty, QDataStream& ds)
{
   int sz, i;
   QStringList slist;
   QString packid;
   LimitConfig *lc;
   lc = NULL;

// project List:
   if (ty == DO_SAVEDB)
   {
      slist = packages();
      sz = slist.size();
      ds << slist;
      qDebug() << "saveDB=" << slist;
   }
   if (ty == DO_LOADDB)
   {
      ds >> slist;
      sz = slist.size();
      qDebug() << "loadDB=" << slist;
   }

// items:  

   for (i = 0; i < sz; i++)
   {
      packid = slist[i];   
      qDebug() <<  "pack id = " << packid << i;
   
         if (ty == DO_SAVEDB)
         {
             lc = packLimit(packid);
             if (lc !=NULL)  
                 lc->saveDB(ds);
         }
         if (ty == DO_LOADDB)
         {
             qDebug() <<"loaddb = "<<  packLimit(packid)<< lc;
             if (packLimit(packid) == NULL)  
             {
                 qDebug() << "new =" << packid;
                 lc = new LimitConfig();
                 mapConfig[packid] = lc;
             }
             if (lc !=NULL)  
             {
                 lc->loadDB(ds);
                 
             }
         }
 
   }
   return DSTAT_OK;

}
int  ConfigMng::doWork(int ty)
{
   int sz, i,iret;
   QStringList slist;
   QString packid;
   iret = DSTAT_OK;
   sz = size();
   LimitConfig *lc;
   slist = packages();
   qDebug() << "packs = " << sz;
   if (ty == DO_ISNULL)
       if (sz == 0)  return DSTAT_OK;

   for (i = 0; i < sz; i++)
   {
      packid = slist[i];
      lc = packLimit(packid);
      if (lc != NULL)
      {
         if (ty == DO_SET_REMOVE)
         {
            lc->setRemove();
         }
         if (ty == DO_CLEAR_REMOVED)
         {
            lc->clearRemoved();
            qDebug() <<" in clear_removed: isNull packid =" <<packid;
            if (lc->isNull())
            {
               qDebug() << "----yes Null";
               mapConfig.remove(packid);
               delete lc;
            }
         }
         if (ty == DO_ISNULL)
         {
             if (!lc->isNull())
             {
                 iret = DSTAT_FAILED; 
                 break;   
             }
         }
      }
   }
   return iret;

}
int ConfigMng::setRemove() // set status to STAT_REMOVE;
{
   return doWork(DO_SET_REMOVE);
}
int ConfigMng::clearRemoved()
{
   return doWork(DO_CLEAR_REMOVED);

}
int ConfigMng::clear()
{
   setRemove();
   return clearRemoved();
}
bool ConfigMng::isNull() // if is a null data
{
    int i;
    qDebug() << "in  is NULL";
    i =  doWork(DO_ISNULL);
    qDebug() << "in  is NULL 1 = "<< i;
    if (i == DSTAT_OK) return true;
    else 
        return false;
}
int ConfigMng::size()
{
   return mapConfig.size();

}
LimitConfig* ConfigMng::packLimit(QString pack)
{
    if(mapConfig.contains(pack))
        return mapConfig[pack];
    else 
        return NULL;
}
QStringList ConfigMng::packages()
{
   return mapConfig.keys();

}
bool ConfigMng::isPack(QString pack)
{
   return mapConfig.contains(pack);
}
int ConfigMng::readFile(QString filen)
{
   QFile f;
   int ret;
   f.setFileName(filen);
   if (f.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      saveDB();// saveDB if file is NULL ,restore it after read
      clear();
      QTextStream in(&f);
      ret = readStream(&in);
      f.close();
      qDebug() << "ret=" << ret;
      if (isNull()) 
      {
          clear();// clear the null file options
          loadDB();//restore before read file 
          qDebug() << " is a null config file  error";
          ret = -1;
      }
   }
   else
   {
      qDebug() << " open config file  error, file =" << filen;
      ret =  -1;
   }
   return ret;
}
int ConfigMng::readStr(QString text)
{
   int ret;
   if (!text.isEmpty())
   {
      saveDB();// saveDB if file is NULL ,restore it after read
      clear();
      QTextStream in(&text);
      ret = readStream(&in);
      if (isNull()) 
      {
          clear();// clear the null file options
          loadDB();//restore before read file 
          qDebug() << " is a null config file  error";
          ret = -1;
      }
   }
   else
   {
      qDebug() << " Input text is NULL =";
      ret =  -1;
   }
   return ret;

}
int ConfigMng::readStream(QTextStream *in)
{
   QFile f;
   QString str, str1, s1, s2, s3, s, packid;
   QStringList slist, packList, userList, projectList;

   int i, line, n, ty,ic;
   LimitConfig *packConfig;
   packConfig = NULL;
   line = 0;
   ic = 0;
   do
   {
//read
      line++;
      n = 0;

      str = in->readLine();
      qDebug() << "read line = " << line << str << str.length();
//encode
      i = str.indexOf(CONFIG_REM_CHAR);
      str1 = str.mid(0, i);  // left of #
                             //qDebug() << "left of # = " << str1 << str1.length() ;
      slist = str1.split(CONFIG_SEP_CHAR);
      if (slist.size() < 2)  // no ":" in the line
      {
         qDebug() << " card err :<2   = " << str;
         continue;
      }

      //qDebug()<< " left = " << slist[0] << slist[0].length() << slist[1] << slist[1].length() ;
      s1 = slist[0].trimmed().toLower();
      s2 = slist[1].trimmed();
      if (slist.size() >= 3)
      {
         s3 = slist[2].trimmed();
         n = s3.toInt();
      }
      else s3 = "";

// packid:-------------------------------------
      if (s1 == CONFIG_PACKID)
      {

         // decode : packids:
         slist = s2.split(CONFIG1_SEP_CHAR);
         packList = slist;
         // qDebug() << "pack = " << packList << slist.size();
         for (i = 0; i < slist.size(); i++)
         {
            packid = slist[i];
            if (!isPack(packid)) // if PackID exist?
            {
               packConfig = new LimitConfig();
               mapConfig[packid] = packConfig;
            }
         }

      }
// user:---------------------------------------------here
      if (s1 == CONFIG_USER)
      {
         ty = ITEM_USER;
         i = readItem(ty, s2, packList, n);
      }
      if (s1 == CONFIG_NODE)
      {
         ty = ITEM_NODE;
         i = readItem(ty, s2, packList, n);
      }
      if (s1 == CONFIG_PROJECT)
      {
         ty = ITEM_PROJECT;
         i = readItem(ty, s2, packList, n);
      }
   }
   while (!str.isNull());

   return 0;
}
// read items:
int ConfigMng::readItem(int ty, QString text, QStringList packList, int limit)
{
   QString s, packid, name; // type;
   QStringList slist;
   int i, j, sz, n;
   LimitData *ldata;
   ldata = NULL;
   n = limit;

   slist = text.split(CONFIG1_SEP_CHAR);
   sz = slist.size();
   for (i = 0; i < sz; i++)
   {
      name  = slist[i];
      for (j = 0; j < packList.size(); j++)
      {
         packid = packList[j];
         if (!mapConfig[packid]->isValid(ty, name))
         {
            ldata = new LimitData();
            mapConfig[packid]->insert(ty, name, ldata);
         }
         mapConfig[packid]->setLimit(ty, name, n);
      } //for pack
   } // for item


   return 0;

}
QString ConfigMng::view(QString pack, QString item, QString name)
{
   QStringList packList;
   QString str, ret;
   LimitConfig *lc;
   str = "";

   if (pack == "" || pack == "*") packList = mapConfig.keys();
   else
   {
      packList << pack;
   }

   for (int i = 0; i < packList.size(); i++)
   {
      str  += "packID: " + packList[i] + "\n";
      lc = mapConfig[packList[i]];
      if (lc != NULL)
      {
         str += lc->view(item, name);
      }
   }
   return str;
}
