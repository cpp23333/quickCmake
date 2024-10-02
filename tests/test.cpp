#include <iostream>
#include <thread>
#include "sylar/log.h"
#include "sylar/util.h"


int main(int argc, char** argv) {
   sylar::Logger::ptr logger(new sylar::Logger);
   logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));
    // sylar::LogEvent::ptr event(new sylar::LogEvent(logger,sylar::LogLevel::DEBUG,__FILE__, __LINE__, 0, sylar::GetThreadId(),sylar::GetFiberId(), time(0)));
    // event->getSS()<<"hello sylar";
    // logger->log(sylar::LogLevel::DEBUG, event);
    // std::cout<<"hello sylar"<<std::endl;

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);
    logger->addAppender(file_appender);
    SYLAR_LOG_INFO(logger)<<"test log";
    SYLAR_LOG_ERROR(logger)<<"test log error";
    SYLAR_LOG_FMT_ERROR(logger,"test macro fmt error %s","aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    
    return 0;
}