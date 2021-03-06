//
//  threads.h
//  Luves
//
//  Created by leviathan on 16/5/18.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef THREADS_H_
#define THREADS_H_

#include <functional>
#include <pthread.h>
#include <vector>
#include <map>

#include "tcpConnection.h"

namespace luves
{
    //
    //
    //线程池模块
    //Singleton模式
    //
    
    class ThreadsPool
    {
    public:
        static ThreadsPool & Instance()
        {
            static ThreadsPool pool;
            return pool;
        }
        
        
        static void AddTask(int accept_fd)
        {
            pthread_mutex_lock(&acceptMutex_);
            accpetVec_.push_back(accept_fd);
            pthread_mutex_unlock(&acceptMutex_);
            pthread_cond_signal(&pthreadCond_);
        };
        
        void CreatePool();
        
        static void * ThreadCallBack(void * data);     //pthread_create()回调函数
        
        //static void SetReadCb(std::function<void (const TcpConnPtr &)> & cb){readcb_=cb;}
        //static void SetWriteCb(std::function<void (const TcpConnPtr &)> & cb){writecb_=cb;}
        
        static void SetTcpConnectionFdPtr(std::map<int,TcpConnectionPtr> * Tcpconnection_fd_map){tcpConnectionFd_= Tcpconnection_fd_map;}
        
        static void SetThreadsNum(int threadNum){threadNum_=threadNum;}
        
        static TcpConnectionPtr  GetTcpConnectionPtr(int fd);
    private:
        ThreadsPool(){};
        ~ThreadsPool();
        ThreadsPool & operator=(ThreadsPool const &);
        ThreadsPool(ThreadsPool const &);
        
        bool isHSHA_;
        bool isLF_;
        
        static std::map<int,TcpConnectionPtr>  * tcpConnectionFd_;
        static std::vector<int > accpetVec_;                                  //accept socket
        static int threadNum_;
        pthread_t * pthreadId_;
        
        static pthread_mutex_t pthreadMutex_;
        static pthread_mutex_t acceptMutex_;
        static pthread_cond_t pthreadCond_;
        static std::function<void (const TcpConnectionPtr &)> readcb_,writecb_;
    };
}

#endif /* threads_h */

