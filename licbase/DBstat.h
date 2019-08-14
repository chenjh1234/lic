#ifndef DBSTAT_HHH
#define DBSTAT_HHH

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/** add data to container up to size,
 *  when the data full , sumary all datas and add to upper layer
 *  ,and next data  will override from beginning  of the
 *  container .
 *  set data : eachtime as a itme data in the database
 *  container;
*/

class DBlayer
{
public:
    DBlayer();
    string name;
    int layer;
    /// set number of the data
    void setSize(int sz);
    int size();
    ///insert data to DB;
    int set(int data);
    /// get data;
    vector<int> get();
    DBlayer *up;
private:
    int ip;
    int _size;
    int sum();
    vector<int> data;
    
};
#define SND_SIZE 60
#define SND_NAME "second"
#define MNT_SIZE 60
#define MNT_NAME "minute"
#define HOUR_SIZE 24
#define HOUR_NAME "hour"
#define DAY_SIZE 30
#define DAY_NAME "day"
#define MONTH_SIZE 30
#define MONTH_NAME "month"
#define YEAR_SIZE 10
#define YEAR_NAME "year"
/// construct database of second(60)->minute(60)->hour(24)->day(30)->month(12)->year(0) ,
/// set data : eachtime as a itme data in the database;
class DBstat
{
public:
    DBstat();
    int setStart(string name);
    int size();
    int set(int data);
    vector<int> get(int layer = 0);
    string curTime();
    string curS();
    long curSL();
    
private:
    int _size;
    void init();// construct
    DBlayer * getLayer(int idx);
    DBlayer *snd,*mnt,*hour,*day,*month,*year;
    DBlayer *basLayer;
};
/// check the clock , ensure  the data store to database items is a continuous second data.
class DBsecond:public DBstat
{
public:
    DBsecond();
    int set(int d); 

private:
    long _lastS;
    int _v;
 
};


#endif

