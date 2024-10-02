#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <boost/thread.hpp>
#include <stdarg.h>
#include "util.h"
#include "singleton.h"

#define SYLAR_LOG_LEVEL(logger, level)                                                                           \
    if (logger->getLevel() <= level)                                                                             \
    sylar::LoggerEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level,                               \
                                                                    __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                    sylar::GetFiberId(), time(0))))              \
        .getSS()

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                             \
    if (logger->getLevel() <= level)                                                                             \
    sylar::LoggerEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level,                               \
                                                                    __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                    sylar::GetFiberId(), time(0))))              \
        .getEvent()                                                                                              \
        ->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)
namespace sylar
{
    class Logger;
  

    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5,
        };
        static const char *toString(LogLevel::Level level);
    };
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level m_level, const char *file,
                 int32_t line, uint32_t elapse, uint32_t threadId,
                 uint32_t fiberId, uint64_t time);

        const char *getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        const std::string &getThreadName() const { return m_threadName; }
        std::string getContent() const { return m_ss.str(); }
        std::stringstream &getSS() { return m_ss; }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

        LogLevel::Level getLevel() const { return m_level; }

        void format(const char *fmt, ...);

        void format(const char *fmt, va_list al);

    private:
        const char *m_file = nullptr; // 文件号
        int32_t m_line = 0;
        uint32_t m_elapse = 0; // 程序启动到现在的毫秒数
        uint32_t m_threadId = 0;
        uint32_t m_fiberId = 0;
        uint64_t m_time = 0;
        std::string m_threadName;
        std::stringstream m_ss;
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
    };

    class LoggerEventWrap
    {
    public:
        LoggerEventWrap(LogEvent::ptr m_event);
        ~LoggerEventWrap();
        std::stringstream &getSS();
        LogEvent::ptr getEvent() { return m_event; }

    private:
        LogEvent::ptr m_event;
    };

    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string &pattern);
        // 时间  线程号日志级别 文件名:行号 内容
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            FormatItem(const std::string &fmt = "") {};
            virtual ~FormatItem() {}
            virtual void format(std::ostream &ss, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };
        void init();

    private:
        std::vector<FormatItem::ptr> m_items;
        std::string m_pattern;
    };

    class LogAppender
    {
        friend class Logger;

    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender() {}
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr val) { m_formatter = val; }

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }
        LogFormatter::ptr const getFormatter() { return m_formatter; }

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

    // 日志器
    class Logger : public std::enable_shared_from_this<Logger>
    {
      

    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string &name = "root");

        void log(LogLevel::Level level, LogEvent::ptr event);
        void debug(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);
        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        void clearAppenders();
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

        const std::string &getName() const { return m_name; }
        void setFormatter(const std::string &val);
        void setFormatter(LogFormatter::ptr val);

    private:
        std::string m_name;                       // 日志名称
        LogLevel::Level m_level;                  // 日志级别
        std::list<LogAppender ::ptr> m_appenders; // appender 列表
        LogFormatter::ptr m_formatter;
        /// 主日志器
        Logger::ptr m_root;
    };

    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

    private:
    };

    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string &filename);
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

    class LoggerManager
    {
    public:
        LoggerManager();
        Logger::ptr getLogger(const std::string &name);
        void init();
        Logger::ptr getRoot() const { return m_root; }

    private:
        std::map<std::string, Logger::ptr> m_loggers;
        Logger::ptr m_root;
    };
    typedef Singleton<LoggerManager> LoggerMgr;
}
#endif