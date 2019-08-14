#include "DBstat.h"
DBlayer::DBlayer()
{
   _size = 0;
   ip = 0;
   up = NULL;
}
///set nearset data is d, each set fill a data in vector,and circle,
int DBlayer::set(int d) // ip set and ++
{
   int i, s;

   data[ip] = d;
   cout << "layer.set ip ,d = " << ip << "," << d <<   "," << data[ip] << endl;
   ip++;
   if (ip >= _size)
   {
      if (up != NULL)
      {

         i = sum();
         cout << "sum = " << i;
         up->set(i);
      }
      ip = 0;
   }
   return d;
}
int DBlayer::size()
{
   return size();
}
/// set size ,constuct the vector ,inialize the all data to 0;
void DBlayer::setSize(int sz)
{
   int i;
   _size = sz;
   data.clear();
   ip = 0;
   data.assign(sz, 0);

}
int DBlayer::sum()
{
   int i, s;
   s = 0;
   for (i = 0; i < _size; i++)
   {
      s = s + data[i];
   }
   return s;
}
/// get last vector of data ,[0] is the nearest.
vector<int>  DBlayer::get()
{
   int i, d, iop, iip;
   vector<int> v;
   iop = ip - 1;
   if (iop < 0) iop = _size - 1;
   for (i = 0; i < _size; i++) // reverse get ,
   {
      iip = iop - i;
      if (iip < 0)
      {
         iip = _size - (i - iop);
      }

      d = data[iip];
      v.push_back(d);
   }
   return v;
}
//=========================================================
DBstat::DBstat()
{
   init();
}
void DBstat::init()
{
   int i;
   basLayer = NULL;

   snd = new DBlayer();
   snd->setSize(SND_SIZE);
   snd->name = SND_NAME;

   mnt = new DBlayer();
   mnt->setSize(MNT_SIZE);
   mnt->name = MNT_NAME;

   hour = new DBlayer();
   hour->setSize(HOUR_SIZE);
   hour->name = HOUR_NAME;

   day = new DBlayer();
   day->setSize(DAY_SIZE);
   day->name = DAY_NAME;

   month = new DBlayer();
   month->setSize(MONTH_SIZE);
   month->name = MONTH_NAME;

   year = new DBlayer();
   year->setSize(YEAR_SIZE);
   year->name = YEAR_NAME;

   snd->up = mnt;
   mnt->up = hour;
   hour->up = day;
   day->up = month;
   month->up = year;
   year->up = NULL;

   _size = setStart("second");
}
int DBstat::setStart(string name)
{
   int i;
   DBlayer *b;
   basLayer = NULL;
   i = 0;
   if (name == SND_NAME) basLayer = snd;
   if (name == MNT_NAME) basLayer = mnt;
   if (name == HOUR_NAME) basLayer = hour;
   if (name == DAY_NAME) basLayer = day;
   if (name == MONTH_NAME) basLayer = month;
   if (name == YEAR_NAME) basLayer = year;
   b = basLayer;
   while (b != NULL)
   {
      b = b->up;
      i++;
   }
   return i;
}
int DBstat::size()
{
   return _size;
}
int DBstat::set(int d)
{
   if (basLayer != NULL)
   {
      basLayer->set(d);
   }
	return 0;
}
DBlayer* DBstat::getLayer(int index)
{
   DBlayer *b;
   int i;
   b = basLayer;
   i = 0;
   // cout << "index = " <<index << b << endl;
   if (index != 0)
   {
      while (b != NULL)
      {
         b = b->up;
         i++;
         if (index == i) break;
      }
   }
   return b;
}
vector<int>  DBstat::get(int index)
{
   DBlayer *b;
   b = getLayer(index);
   // cout << "b = " << b;
   return b->get();
}
string DBstat::curTime()
{
   char ch[256];
   time_t timer;
   struct tm *t_tm;
   time(&timer);
   t_tm = localtime(&timer);
   sprintf(ch, "%4d%02d%02d%02d%02d%02d", t_tm->tm_year + 1900,
           t_tm->tm_mon + 1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
   string s;
   s = ch;
   return s;
}
long DBstat::curSL()
{
    string s;
    s = curS();
    return atol(s.c_str());
}
string DBstat::curS()
{
   char ch[256]; 
   time_t timep;
   struct tm *p;
   string s;

   time(&timep); /*当前time_t类型UTC时间*/
   sprintf(ch, "%ld", timep);
   s = ch;
   return s;
}

//============================
DBsecond::DBsecond() : DBstat()
{
    _lastS = 0;
    _v = 0;
}
int DBsecond::set(int d)
{
    long sl;
    int len,i;// second from last set;
    
    sl =  curSL();
    if (_lastS == 0) _lastS = sl;

    len = sl - _lastS;
    if (len == 0) 
    {
        _v = _v +d;
    }
    else if(len >= 0)// set last;and just remember the data of this set;
    {
        DBstat::set(_v);// set the farest last set;
        // set(0) for the last len-1 second
        for(i = 0; i <len-1;i++)
        {
            DBstat::set(0);
        }
        // remember this set
        _v = d;
    }
    else
    {
        cout << "time go reverse ;must be a error in DBsecond::set(int d)\n";
        return -1;
    }
    _lastS = sl;
    return _v;
}

