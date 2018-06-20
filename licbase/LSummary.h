#ifndef SUM_INFO_H
#define SUM_INFO_H

#include "sys/time.h"
#if  !defined(WIN32)
#include "sys/times.h"
#endif
#include "unistd.h"
#include <iostream>
using namespace std;

#define MAX_SUMU 10*1000*1000 // 10S
/**
 * statistic : 
 * time elapsed 
 * cpu time 
 * bytes;
 * files; 
 * counter: record
 */
class LSummary
{
public:
   LSummary() { sumS = 0; sumU = 0; counter = 0; bytes = 0; cpu = 0; files = 0;};


   /// get cpu clock_t for this process

   long getCputime()
   {
#if  !defined(WIN32)
      struct tms cpuTime;
      long l;
      if (times(&cpuTime) == -1) return -1;
      l = cpuTime.tms_utime + cpuTime.tms_stime;
      return l; //sysconf(_SC_CLK_TCK);//= 100; so the accuracy is 1/100S;
#endif
   };
   /// start the time counter
   int start()
   {
      struct timeval tv;
      if (gettimeofday(&tv, NULL) == 0)
      {
         tv0 = tv;
         cpu0 = getCputime();
         return 0;
      }
      return -1;
   };
/// sum all time for all  start()-elapse(), in S, double  
   long  elapsed()
   {
      struct timeval tv;
      long  u;
      if (gettimeofday(&tv, NULL) == 0)
      {
         // time:
         u = (tv.tv_sec - tv0.tv_sec) * 1000000 + (tv.tv_usec - tv0.tv_usec);
         sumU += u;
         // counter of record;
         counter++;
         //cout <<sumU <<"," << u<<endl;
         //cpu
         cpu += getCputime() - cpu0;
         cpu0 = 0;
         if (sumU >= MAX_SUMU)// 10S,10,000,000
         {// if sumU >10s plus it to sumS,
            //cout << "dddddddddddd" <<MAX_SUMU <<endl;
            sumS += double(sumU) / 1000000;
            sumU = 0;
         }
         return u;
      }
      return -1;
   };
   string startTime()// last start time in string
   {
      time_t timer;
      timer = tv0.tv_sec;
      return u2str(timer);
          
   };
   /// current Time in  yyyymmddHHMMSS
   string curTime()
   {
      time_t timer;
      time(&timer);
      return u2str(timer);
   }
   /// convert timer to format: yyyymmddHHMMSS
   string u2str(time_t timer)
   {
      char ch[256];
      //time_t timer;
      struct tm *t_tm;
      time(&timer);
      t_tm = localtime(&timer);
      sprintf(ch, "%4d%02d%02d%02d%02d%02d", t_tm->tm_year + 1900,
              t_tm->tm_mon + 1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
      string s;
      s = ch;
      return s;
   }
   /// initalize all data to zero;
   void clear() { sumS = 0; sumU = 0; counter = 0; bytes = 0; cpu = 0; files = 0;};

   /// get the time that all start-elapsed of this object;
   double getTime() { return sumS + double(sumU) / 1000000;};
   /// get cpu time that all start-elapsed of this object;
#if  !defined(WIN32)
   double getCpu() { return  double(cpu) / sysconf(_SC_CLK_TCK);};
   // double getCpu(){return  double(cpu)/sysconf(_SC_CLK_TCK);};
#else
   double getCpu()
   {return   double(clock())/100/10000;};
#endif
   /// couner of start-elapsed since clear();
   long size() { return counter;};
   /// couner of bytes;
   long getBytes() { return bytes;};
   void addBytes(int by) { bytes += by;};
   void setBytes(int by) { bytes = by;};
   /// couner of file;
   long getFiles() { return files;};
   void addFiles(int by) { files += by;};
   void setFiles(int by) { files = by;};

private:
   struct timeval tv0; // start time
   long  sumU; // usecond of left ,when >MAX_SUMU add to sumS
   double sumS; //seconds part
   long counter; // tims of stat-elapsed;
   long bytes;
   long cpu0,cpu; // cpus
   int files;
};
#endif // _H


//================================================
#if 0
double elapsed()
{
   struct timeval tv
   gettimeofday( &tv1, NULL ); //static run time
   begin_cputime = gos_get_cputime();
   m_modexec->ExecPM();
   end_cputime = gos_get_cputime();
   gettimeofday( &tv2, NULL );
   modElapsedTime = (double)(tv2.tv_sec-tv1.tv_sec) +((double)(tv2.tv_usec-tv1.tv_usec))/1000000;
   m_modexec->m_iRunTimesPM += modElapsedTime;
};
#endif

#if 0 //main
main()
{
   LSummary tm;
   double d;
   int i;
   cout << "start" <<endl;
   tm.start();
   cout << "do the loop " <<endl;
   for (i = 0; i < 1024 * 32768; ++i) ;
   cout << "sleep 1 second" <<endl;
   sleep(1);
   cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC <<endl;
   cout << "sysconf(_SC_CLK_TCK)= " << sysconf(_SC_CLK_TCK) <<endl;
   // getTime =0 because it run getTime first ,than run elapsed;
   cout << "tm elapsed= " << tm.elapsed() << ","<< tm.getTime() << endl;
   cout << "tm elapsed= " << tm.elapsed() << endl;
   cout << "tm getTime= " << tm.getTime() <<endl;
   cout << "t.cputime = " << tm.getCputime()<< endl;
   cout << "clock= " << clock() <<endl; // accuracy is the same with times;
   cout << "size of int  long,ll = " << sizeof(int) << ","<< sizeof(long)<< ","<< sizeof(long long) <<endl;
   cout << "t.getCpu = " << tm.getCpu()<< endl;
}
#endif //main
#if 0 // result
start
do the loop
sleep 1 second
CLOCKS_PER_SEC = 1000000
sysconf(_SC_CLK_TCK)= 100
1069648,1069648
tm elapsed= 1069648,0
2139326,1069678
tm elapsed= 1069678
tm getTime= 2.13933
t.cputime = 6
clock= 60000  // 1/100
size of int  long,ll = 4,8,8
#endif



#if 0  // document
time_t time(time_t *timer);
调用后将当前系统时间与1900年1月1日相差的秒数存入到timer中，timer可看成是一个长整型数
struct tm* localtime(const time_t *timer)
将time()函数调用的结果做为参数传入到localtime()函数中就能得到当前时间和日期，注意得到的年是和1970的差值，月份是和1月的差值
struct tm是一个结构体，定义如下
struct tm
{
   int tm_sec;    //当前秒
   int tm_min;    //当前分钟
   int tm_hour;   //当前小时
   int tm_mday;   //当前在本月中的天，如11月1日，则为1
   int tm_mon;    //当前月，范围是0~11
   int tm_year;   //当前年和1900的差值，如2006年则为36
   int tm_wday;   //当前在本星期中的天，范围0~6
   int tm_yday;   //当前在本年中的天，范围0~365
   int tm_isdst;  //这个我也不清楚
}

求当前时间的示例
int getSystemTime()
{
   time_t timer;
   struct tm* t_tm;
   time(&timer);
   t_tm = localtime(&timer);
   printf("today is %4d%02d%02d%02d%02d%02d\n", t_tm.tm_year+1900,
          t_tm.tm_mon+1, t_tm.tm_mday, t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec);
   return 0;
}

其他时间的函数和结构还有：
timeval结构
#include <include/linux/time.h>
struct timeval
{
   time_t     tv_sec;
   susecond_t     tv_usec;         //当前妙内的微妙数
};
tms结构
保存着一个进程及其子进程使用的cpu时间

struct tms
{
   clock_t tms_utime;
   clock_t tms_stime;
   clock_t tms_cutime;
   clock_t tms_cstime;
}
timer_struct结构
#include <include/linux/timer.h>

struct timer_struct
{
   unsigned long expires;    //定时器被激活的时刻
   void (*fn)(void);           //定时器激活后的处理函数
}
utime函数
更改文件的存取和修改时间

int utime(const char pathname, const struct utimbuf *times)                    // return value 0 or -1
times 为空指针，存取和修改时间设置为当前时间
struct utimbuf
{
   time_t     actime;
   time_t     modtime;
}
#include<time.h >头文件
然后
time_t t;
time(&t);
输出
ctime(&t)
具体为：
#include <stdio.h>
#include <time.h>
int main(void)
{
   time_t t;
   time(&t);
   printf("Today's date and time: %s\n", ctime(&t));
   return 0;
}

#endif //document
