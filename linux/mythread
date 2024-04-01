#ifndef mythread
#define mythread
#include<memory>
#include<queue>
#include<pthread.h>
#include<list>
#include<thread>
#include<functional>
#include<iostream>
#include<unistd.h>
#ifndef Threads_max
#define Threads_max 12
#endif

using std::thread;
using std::unique_ptr;

//Multi-producer , Multi-consumer Queue
template<class T>
class TaskQueue {
public:
    explicit TaskQueue(int val):capacity(val){}
    virtual bool push(T) = 0;
    virtual T pop()=0;
    virtual ~TaskQueue() {};
protected:
    int capacity;
};

template<typename T>
class taskq:public TaskQueue<T>{
public:
    explicit taskq(int val=30):TaskQueue<T>(val){pthread_mutex_init(&cur_mtx,nullptr);}
    bool push(T val)
    {
        pthread_mutex_lock(&cur_mtx);
        if(cur<this->capacity)
        {
            que.push(val);
            cur++;
            pthread_mutex_unlock(&cur_mtx);
            return 1;
        }
        else
        {
            pthread_mutex_unlock(&cur_mtx);
            return 0;
        }
    }
    T pop()
    {
        pthread_mutex_lock(&cur_mtx);   
        if(cur==0)
        {
            pthread_mutex_unlock(&cur_mtx);
            return nullptr;
        }
        else
        {
            cur--;
            auto ret=que.front();
            que.pop();
            pthread_mutex_unlock(&cur_mtx);
            return ret;
        }
    }
    int size()
    {
        return cur;
    }
    ~taskq(){};
private:
    std::queue<T> que;
    int cur=0;
    pthread_mutex_t cur_mtx;
};

//threadpool
class threadpool{
public:
    explicit threadpool(int val=8,int val2=30):cur(val>Threads_max?Threads_max:val),task(val2),max_task(val2)
    {
        pthread_mutex_init(&pool_mtx,nullptr);
        pthread_cond_init(&pool_cond,nullptr);
        for(int c=0;c<cur;c++)
        {
            threads.emplace_back(&threadpool::working,this);
        }
    }
    template<typename T,typename...A>bool testadd(T&&atask,A&&...argc)
    {
        if(task.size()<(max_task>>1))return addtask(std::forward<T>(atask),std::forward<A>(argc)...);
        return 0; 
    }
    template<typename T,typename...A>bool addtask(T*atask,A&&...argc)
    {
        auto result=task.push(std::bind(std::forward<T*>(atask),std::forward<A>(argc)...));
        if(result==0)
        {
            pthread_cond_broadcast(&pool_cond);
            return 0;
        }
        if(task.size()>=(max_task-(max_task>>2)))
            addthread();
        pthread_cond_broadcast(&pool_cond);
        return 1;
    }
    void terminal()
    {
        term=true;
        pthread_cond_broadcast(&pool_cond);
        for(auto &a:threads)
        {
            if(a.joinable())a.join();
        }
    }
    ~threadpool()
    {
        term=true;
        pthread_cond_broadcast(&pool_cond);
        for(auto &a:threads)
        {
            if(a.joinable())a.join();
        }
    }
    int thread_num(){return cur;}
private:
    taskq<std::function<void()>> task;
    int max_task;
    std::list<thread> threads;
    int max=Threads_max;
    unsigned int cur;
    bool term=false;
    pthread_mutex_t pool_mtx;
    pthread_cond_t pool_cond;
private:
    void working();
    bool addthread()
    {
        if(cur==Threads_max)
            return 0;
        else
        {
            ++cur;
            threads.emplace_back(&threadpool::working,this);
            return 1;
        }
    }
};

void threadpool::working()
{
    while(term==false||task.size()!=0)
    {
        auto atask=task.pop();
        if(atask!=nullptr)
        {
            atask();
        }
        else
        {

            pthread_mutex_lock(&pool_mtx);
            pthread_cond_wait(&pool_cond,&pool_mtx);
            pthread_mutex_unlock(&pool_mtx);
        }
    }
    return ;
}

template<class T>
class MPMCQueue {
public:
    explicit MPMCQueue(int val):capacity(val){}
    virtual bool push(unique_ptr<T>) = 0;
    virtual unique_ptr<T> pop() = 0;
    virtual ~MPMCQueue() {};
protected:
    int capacity;
};
template<typename T>
class mpmc:public MPMCQueue<T>{
public:
    explicit mpmc(int val=30):MPMCQueue<T>(val){pthread_mutex_init(&cur_mtx,nullptr);}
    bool push(unique_ptr<T> val)
    {
        pthread_mutex_lock(&cur_mtx);
        if(cur<this->capacity)
        {
            que.push(std::move(val));
            cur++;
            pthread_mutex_unlock(&cur_mtx);
            return 1;
        }
        else
        {
            pthread_mutex_unlock(&cur_mtx);
            return 0;
        }
    }
    unique_ptr<T> pop()
    {
        pthread_mutex_lock(&cur_mtx);   
        if(cur==0)
        {
            pthread_mutex_unlock(&cur_mtx);
            return nullptr;
        }
        else
        {
            cur--;
            auto ret=std::move(que.front());
            que.pop();
            pthread_mutex_unlock(&cur_mtx);
            return ret;
        }
    }
    int size()
    {
        return cur;
    }
    ~mpmc(){};
private:
    std::queue<unique_ptr<T>> que;
    int cur=0;
    pthread_mutex_t cur_mtx;
};

#endif