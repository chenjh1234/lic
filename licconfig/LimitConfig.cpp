#include "LimitConfig.h"

// LimitConfig=============================
LimitConfig::LimitConfig()
{
   clearUserAll();
   clearNodeAll();
   clearProjectAll();

}
LimitConfig::~LimitConfig()
{

}
 
// file decode used:
int LimitConfig::setUserLimit(QString name, int n)
{
   return setLimit(ITEM_USER, name, n);
}
int LimitConfig::setNodeLimit(QString name, int n)
{
   return setLimit(ITEM_NODE, name, n);
}
int LimitConfig::setProjectLimit(QString name, int n)
{
   return setLimit(ITEM_PROJECT, name, n);
}
// is 
bool LimitConfig::isUser(QString name)
{
   return isValid(ITEM_USER, name);
}
bool LimitConfig::isNode(QString name)
{
   return isValid(ITEM_NODE, name);
}
bool LimitConfig::isProject(QString name)
{
   return isValid(ITEM_PROJECT, name);
}
// manage limit used: 0:ok,-1:user,-2:node err;-3:project err;

bool LimitConfig::canLogin(QString user, QString node, QString project, int n)
{
   //if (configUser[user]->canLogin(n) && configNode[node]->canLogin(n) && configProject[project]->canLogin(n)) return true;
   if (doLogin(DO_CANLOGIN,configUser,user,n) >=0 && doLogin(DO_CANLOGIN,configNode,node,n) >=0 &&doLogin(DO_CANLOGIN,configProject,project,n)>=0) return true;
   return false;
}
int LimitConfig::login(QString user, QString node, QString project, int n)
{
   int ir;
   //user:
   ir = doLogin(DO_LOGIN,configUser,user,n);
   if (ir < LOGIN_OK)  return ERR_LOGIN_USER;
//node:
   ir = doLogin(DO_LOGIN,configNode,node,n);
   qDebug() << "do login node ir = " <<ir ;
   if (ir < LOGIN_OK)
   {
      doLogin(DO_UNLOGIN,configUser,user,n);
      return ERR_LOGIN_NODE;
   }
//project:
   ir = doLogin(DO_LOGIN,configProject,project,n);
    qDebug() << "do login project ir = " <<ir ;
   if (ir < LOGIN_OK)
   {
      doLogin(DO_UNLOGIN,configUser,user,n);
      doLogin(DO_UNLOGIN,configNode,node,n);
      return ERR_LOGIN_PROJECT;
   }

   return LOGIN_OK;
}
int LimitConfig::unlogin(QString user, QString node, QString project, int n)
{
   int ir1, ir2, ir3;
   if (configUser.contains(user))
       ir1 = configUser[user]->unlogin(n);
   if (configNode.contains(node))
        ir2 = configNode[node]->unlogin(n);
   if (configProject.contains(project)) 
       ir3 = configProject[project]->unlogin(n);

   if (ir1 < LOGIN_OK || ir1 < LOGIN_OK || ir1 < LOGIN_OK)
   {
      qDebug() << "Error unlogin failed " <<  user << node << project << ir1 << ir2 << ir3;
      return LOGIN_FAILED;
   }
   return LOGIN_OK;
}
int LimitConfig::setRemove() // set status to STAT_REMOVE;
{
   return doRemove(DO_SET_REMOVE);
}
int LimitConfig::clearRemoved() // set status to STAT_REMOVE;
{
   return doRemove(DO_CLEAR_REMOVED);
}
int LimitConfig::doRemove(int ty)
{
   int i;
  // qDebug() << "doWort(ty)";
//user:
   i = doRemove(ty, configUser);
   if (i < 0) return i;
//node:
   i = doRemove(ty, configNode);
   if (i < 0) return i;
//project:
   i = doRemove(ty, configProject);
   if (i < 0) return i;

   return i;

}
int LimitConfig::doRemove(int ty, QMap<QString, LimitData *>& who)
{
   int sz, i;
   QStringList slist;
   QString name;
   LimitData *ld;

   sz =  who.size();
   slist = who.keys();
   if (ty == DO_SET_REMOVE) qDebug() << "setRemove =" << sz <<  slist;
   if (ty == DO_CLEAR_REMOVED) qDebug() << "clearRemoved =" << sz <<  slist;


   for (i = 0; i < sz; i++)
   {
      name = slist[i];
      ld = who[name];
      if (ld != NULL)
      {
         if (ty == DO_SET_REMOVE)
         {
            ld->setRemove();
         }
         if (ty == DO_CLEAR_REMOVED)
         {
            if (ld->isRemove())
            {

               who.remove(name);
               delete ld;
               if (name == CONFIG_ALL_CHAR)
               {
                  if (who == configUser)  clearUserAll();
                  if (who == configNode)  clearNodeAll();
                  if (who == configProject)  clearProjectAll();
               }
            }
         }
      }
      else qDebug() << " Error: LiniteData is NULL name =  " << name;
   }
   return DSTAT_OK;
}

int LimitConfig::doLogin(int ty, QMap<QString, LimitData *>& who, QString name, int n)
{
   int  ir;
   LimitData *ld;
//login:
// if no this name ,and no ALL:"*" we PASS
// if no this name ,and have  All:"*" ,we creat the name;
// unlogin:
//if no this name ,we PASS
 //qDebug() <<" ALL ====" << name;

// if no config ,we check ALL item:
   ir = LOGIN_NONE;
   qDebug() << "do login " <<ty << name << n;

    _lock.lock();

   if (!who.contains(name))
   {
      if (ty == DO_CANLOGIN)
      {
         if (isAll(who))
         {
            
            ld = new LimitData();
            who[name] = ld;
            ld->setLimit(getAll(who));
            
         }
         else 
         {
             
             ir =  LOGIN_PASS;
         }
      }
      #if 0
      else if (ty == DO_LOGIN)
      {
         if (isAll(who))
         {
            ld = new LimitData();
            who[name] = ld;
            ld->setLimit(getAll(who));
         }
         else return LOGIN_PASS;
      }
      #endif
      else if (ty == DO_UNLOGIN)// unlogin;
      {
          qDebug() << "Not Login-----";
         ir = LOGIN_PASS;
      }
      #if 1 //  the name is not in configfile  ,so pass
      else if (ty == DO_LOGIN)// unlogin;
      {
          ir =  LOGIN_PASS;
      }
      #endif
      else // unkown
      {
          ir =  LOGIN_FAILED;
      }
   }// end of no name
   _lock.unlock();
   qDebug() << "after cantain ir =" << ir;
   if(ir != LOGIN_NONE)
       return ir;

   // yes we got the name now:

   if (ty == DO_CANLOGIN)
   {
       if(who[name]->canLogin(n))
           ir = LOGIN_OK;
       else
           ir = LOGIN_FAILED;
   }
   else if (ty == DO_LOGIN)
   {
       ir =  who[name]->login(n);
       qDebug() << "LOGIN=" << ir << name << n;
   }
   else if (ty == DO_UNLOGIN)
   {
        ir =  who[name]->unlogin(n);
   }
   else
       ir = LOGIN_FAILED;
   return ir;
}
bool LimitConfig::isAll(QMap<QString, LimitData *>& who)
{
   if (who == configUser)  return isUserAll();
   if (who == configNode)  return isNodeAll();
   if (who == configProject)  return isProjectAll();
   return false;
}
int LimitConfig::getAll(QMap<QString, LimitData *>& who)
{
   // qDebug() << "all===========" << userLimit << nodeLimit << projectLimit;
   if (who == configUser)  return userLimit;
   if (who == configNode)  return nodeLimit;
   if (who == configProject)  return projectLimit;
   return -1;
}
bool LimitConfig::isUserAll()
{
   return userLimit != ALL_CLEAR;
}
bool LimitConfig::isNodeAll()
{
   return nodeLimit != ALL_CLEAR;
}
bool LimitConfig::isProjectAll()
{
   return projectLimit != ALL_CLEAR;
}
// All options:===========
int LimitConfig::clearUserAll()
{
   return setUserAll(ALL_CLEAR);
}
int LimitConfig::clearNodeAll()
{
   return setNodeAll(ALL_CLEAR);
}
int LimitConfig::clearProjectAll()
{
   return setProjectAll(ALL_CLEAR);
}
int LimitConfig::setUserAll(int n)
{
   return setLimitAll(ITEM_USER, n);
}
int LimitConfig::setProjectAll(int n)
{
   return setLimitAll(ITEM_PROJECT, n);
}
int LimitConfig::setNodeAll(int n)
{
   return setLimitAll(ITEM_NODE, n);
}
int LimitConfig::setLimitAll(int ty, int n)
{
   switch (ty)
   {
   case ITEM_USER :
      userLimit = n;
      return DSTAT_OK;
   case ITEM_NODE :
      nodeLimit = n;
      return DSTAT_OK;
   case ITEM_PROJECT :
      projectLimit = n;
      return DSTAT_OK;
   }
   return false;
}
int LimitConfig::setLimit(int ty, QString name, int n)
{
   switch (ty)
   {
   case ITEM_USER :
      configUser[name]->setLimit(n);
      if (name == CONFIG_ALL_CHAR)  setLimitAll(ty, n);
      return DSTAT_OK;
   case ITEM_NODE :
      configNode[name]->setLimit(n);
      if (name == CONFIG_ALL_CHAR)   setLimitAll(ty, n);
      return DSTAT_OK;
   case ITEM_PROJECT :
      configProject[name]->setLimit(n);
      if (name == CONFIG_ALL_CHAR)   setLimitAll(ty, n);
      return DSTAT_OK;
   }
   return false;

}
bool LimitConfig::isNull()
{
   //bool b;
   // b = sizeUser()==0&&sizeNode()==0&&sizeProject()==0;
   //qDebug() <<  "in isNull() size of item = " << sizeUser() << sizeNode() << sizeProject() << b;
   return (sizeUser() == 0 && sizeNode() == 0 && sizeProject() == 0);
}
bool LimitConfig::isValid(int ty, QString name)
{
   switch (ty)
   {
   case ITEM_USER :
      return configUser.contains(name);
   case ITEM_NODE :
      return configNode.contains(name);
   case ITEM_PROJECT :
      return configProject.contains(name);
   }
   return false;
}
int LimitConfig::insert(int ty, QString name, LimitData *ldata)
{
   switch (ty)
   {
   case ITEM_USER :
      configUser.insert(name, ldata);
      break;
   case ITEM_NODE :
      configNode.insert(name, ldata);
      break;
   case ITEM_PROJECT :
      configProject.insert(name, ldata);
      break;
   }
   return 0;
}
QString LimitConfig::view(QString item, QString name)
{
   QString str;
   str = "";
   //qDebug() << " item === " << item;
   if (item == "" || item == "*")
   {
      str += TAB_1 + CONFIG_USER + ":\n";
      str += viewItem(configUser, name);
      str += TAB_1 + CONFIG_NODE + ":\n";
      str += viewItem(configNode, name);
      str += TAB_1 + CONFIG_PROJECT + ":\n";
      str += viewItem(configProject, name);
   }
   else
   {
      str += TAB_1 + item + ":\n";
      if (item == CONFIG_USER)  str += viewItem(configUser, name);
      else if (item == CONFIG_NODE) str += viewItem(configNode, name);
      else if (item == CONFIG_PROJECT) str += viewItem(configProject, name);
      else
      {
         qDebug() << " view item err !!!";
         return str;
      }


   }
   return str;
}
QString LimitConfig::viewItem(QMap<QString, LimitData *>& item, QString name)
{
   QString str, n;
   QStringList slist;
   int i;
   str = "";
   slist.clear();

   if (name == "" || name == "*")
   {
      slist = item.keys();
   }
   else
   {
      //qDebug() << "name========" << name;
      slist << name;
   }
   //qDebug() << "slist === " << slist << name << item;
// list:
   for (i = 0; i < slist.size(); i++)
   {
      n = slist[i];
      if (item.contains(n))  str += TAB_2 + slist[i] + QString(": %1,%2,%3\n").arg(item[n]->limit()).arg(item[n]->_count).arg(item[n]->status());
      //qDebug() << i << n << str;
   }
   return str;
}

//DBS:

int LimitConfig::saveDB(QDataStream &ds)
{
    //1:user size, allsize,data of each name;
    int ir;
    ir = DSTAT_OK;
// user:
    
    ds << userLimit;
    ir = doDB(DO_SAVEDB,configUser,ds);
    if (ir != DSTAT_OK) return ir;
// node:
   
    ds << nodeLimit;
    ir = doDB(DO_SAVEDB,configNode,ds);
    if (ir != DSTAT_OK) return ir;
// project:
   
    ds << projectLimit;
    ir = doDB(DO_SAVEDB,configProject,ds);
    if (ir != DSTAT_OK) return ir;
    return ir;

}
int LimitConfig::loadDB(QDataStream &ds)
{

    int lmt,ir;
    ir = DSTAT_OK;
//user:
   // ds >> sz;
    ds >> lmt;
    userLimit= lmt;
    ir = doDB(DO_LOADDB,configUser,ds);
    if (ir != DSTAT_OK) return ir;
//node:
    //ds >> sz;
    ds >> lmt;
    nodeLimit= lmt;
    ir = doDB(DO_LOADDB,configNode,ds);
    if (ir != DSTAT_OK) return ir;
//project:
    //ds >> sz;
    ds >> lmt;
    projectLimit= lmt;
    ir = doDB(DO_LOADDB,configProject,ds);
    if (ir != DSTAT_OK) return ir;

    return ir;
     
}
int LimitConfig::doDB(int ty,QMap<QString,LimitData *> &who,QDataStream &ds)
{
    int i,sz;
    QStringList slist;
    QString name;
    LimitData *ld;

    if (ty == DO_SAVEDB) 
    {
        slist = who.keys();
        sz = slist.size();

        ds << slist;
        for (i = 0; i <sz;i++) 
        {
            name = slist[i];
            ld = who[name];
            if (ld != NULL) 
            {
                ld->saveDB(ds);
            }
            else
            {
                return DSTAT_FAILED;
            }
        }


    }
    else if (ty == DO_LOADDB)
    {
        ds >> slist;
        sz = slist.size();
       // qDebug() << "loadDB =" << slist << sz;

        for (i = 0; i <sz;i++) 
        {
            name = slist[i];
            if (!who.contains(name))
            {
                ld = new LimitData();
                who[name] = ld;
                //qDebug() << "loadDBnew  = " << ld << name;
            }
            ld = who[name];
            //qDebug() << "loadDB = " << ld << name;

            if (ld != NULL) 
            {
                ld->loadDB(ds);
            }
            else
            {
                return DSTAT_FAILED;
            }
        }

    }
    else
        return DSTAT_FAILED;

    return DSTAT_OK;

}
