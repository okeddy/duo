#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"
#include <boost/scoped_ptr.hpp>

namespace duo {

    class Logger {
    public:
        enum LogLevel {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };

        Logger(const char* file, int line);
        Logger(const char* file, int line, LogLevel level);
        Logger(const char* file, int line, LogLevel level, const char* func);
        Logger(const char* file, int line, bool toAbort);
        ~Logger();

        LogStream& stream() { return impl_.stream_; }

        static LogLevel logLevel();
        static void setLogLevel(LogLevel level);

        typedef void (*OutputFunc)(const char* msg, int len);
        typedef void (*FlushFunc)();
        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);

    private:

        class Impl {
        public:
            typedef Logger::LogLevel LogLevel;
            Impl(LogLevel level, int old_errno, const char* file, int line);
            void formatTime();
            void finish();

            Timestamp time_;
            LogStream stream_;
            LogLevel level_;
            int line_;
            const char* fullname_;
            const char* basename_;
        };

        Impl impl_;

    };

#define LOG_TRACE if (duo::Logger::logLevel() <= duo::Logger::TRACE) \
  duo::Logger(__FILE__, __LINE__, duo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (duo::Logger::logLevel() <= duo::Logger::DEBUG) \
  duo::Logger(__FILE__, __LINE__, duo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (duo::Logger::logLevel() <= duo::Logger::INFO) \
  duo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN duo::Logger(__FILE__, __LINE__, duo::Logger::WARN).stream()
#define LOG_ERROR duo::Logger(__FILE__, __LINE__, duo::Logger::ERROR).stream()
#define LOG_FATAL duo::Logger(__FILE__, __LINE__, duo::Logger::FATAL).stream()
#define LOG_SYSERR duo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL duo::Logger(__FILE__, __LINE__, true).stream()

    const char* strerror_tl(int savedErrno);

    // Taken from glog/logging.h
    //
    // Check that the input is non NULL.  This very useful in constructor
    // initializer lists.

#define CHECK_NOTNULL(val) \
  ::duo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

    // A small helper for CHECK_NOTNULL().
    template <typename T>
    T* CheckNotNull(const char* file, int line, const char* names, T* ptr) {
        if (ptr == NULL) {
            Logger(file, line, Logger::FATAL).stream() << names;
        }
        return ptr;
    }

    template<typename To, typename From>
    inline To implicit_cast(From const& f) {
        return f;
    }
}

#endif  // MUDUO_BASE_LOGGING_H
