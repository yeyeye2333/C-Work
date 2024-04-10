#ifndef chat_Timer
#define chat_Timer
#include<sys/time.h>
#include<signal.h>
#include<string.h>
#include<vector>
#include<mutex>
class Timer{
public:
    Timer(int once,int _sec,int _usec=0):sec(_sec),usec(_usec)
    {
        timer_once=once;
        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler=func;
        sigaction(SIGALRM,&act,nullptr);
        itimerval timer;
        memset(&timer,0,sizeof(timer));
        timer.it_value.tv_sec=sec;
        timer.it_interval.tv_sec=1;
        setitimer(ITIMER_REAL,&timer,nullptr);
    }
    void restart()
    {
        itimerval timer;
        memset(&timer,0,sizeof(timer));
        timer.it_value.tv_sec=sec;
        timer.it_interval.tv_sec=1;
        setitimer(ITIMER_REAL,&timer,nullptr);
    }
    void stop()
    {
        itimerval timer;
        memset(&timer,0,sizeof(timer));
        setitimer(ITIMER_REAL,&timer,nullptr);
    }
    ~Timer()
    {
        stop();
        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler=SIG_DFL;
        sigaction(SIGALRM,&act,nullptr);
    }
    static int timer_flag;
    static int once;
private:
    int sec;
    int usec;
    void (*func)(int);
};
int timer_once;
int Timer::timer_flag;
void static func(int sig)
{
    Timer::timer_flag+=timer_once;
}


#endif