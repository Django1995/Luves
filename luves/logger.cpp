//
//  log.cpp
//  Luves
//
//  Created by leviathan on 16/6/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "logger.h"

namespace luves
{

    std::string strlevel[]=
    {
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
        "TRACE"
    };
    
    Logger::Logger(): level_(LINFO),mode_(TERMIANAL)
    {
        fd_=-1;
        level_=LWARN;
    }
    
    Logger::~Logger()
    {
        if (fd_!=-1)
        {
            close(fd_);
        }
    }
    
    void Logger::SetFileName(const std::string & filename)
    {
        int fd=open(filename.c_str(), O_APPEND|O_CREAT|O_WRONLY|O_CLOEXEC);
        if (fd<0)
        {
            return ;
        }
        filename_=filename;
        if (fd_==-1)
        {
            fd_=fd;
        }
        else
        {
            close(fd);
        }
        
        
    }
    
    void Logger::PrintLog(int level, const char* file, int line, const char* func, const char * pram, ...)
    {
        if (level>level_)
        {
            return;
        }
        
        struct timeval now_tv;
        gettimeofday(&now_tv,NULL); //得到当前时间
        const time_t seconds=now_tv.tv_sec;
        struct tm t;
        localtime_r(&seconds, &t);  //返回当地时间
        
        char *buffer=new char[4*1024];
        char* p = buffer;
        //char* limit = buffer + sizeof(buffer);
        
        p+= snprintf(p,4*1024,
                      "%04d/%02d/%02d-%02d:%02d:%02d.%06d %lu %s %s:%d %s%s",
                      t.tm_year + 1900,
                      t.tm_mon + 1,
                      t.tm_mday,
                      t.tm_hour,
                      t.tm_min,
                      t.tm_sec,
                      static_cast<int>(now_tv.tv_usec),
                      pthread_self(),
                      strlevel[level].c_str(),
                      file,
                      line,
                      func,
                      "()"
                      );

        va_list pvar;
        va_start(pvar,pram);
        p += vsnprintf(p,4*1024,pram,pvar);
        va_end(pvar);
        
        if (mode_==FILE)
            write(fd_, buffer, 1);
        else if(mode_==TERMIANAL)
            std::cout<<buffer<<std::endl<<std::endl;
        delete []  buffer;
    }
}