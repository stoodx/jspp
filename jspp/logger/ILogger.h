#pragma once
#include <memory>
#include <mutex>

//#define USE_CONSOLE_LOGGER //fix me!!! viktor

#define LOG_LEVEL_INFO  "info"
#define LOG_LEVEL_ERROR "error"
#define LOG_LEVEL_WARN  "warn"
#define LOG_LEVEL_DEBUG "debug"

// use this flag to switch between file and console logger. 
// When this flag is set the logger will work with the console.

// Turn off logging for production
#ifndef _PRODUCTION
	#define LOG_MSG(logger, level, messageFormat, ...) \
		logger->log(level, messageFormat,__VA_ARGS__);
#else
	#define LOG_MSG(logger, level, messageFormat, ...)
#endif

class ILogger
{
public:
  ILogger(void);
  virtual ~ILogger(void);

  virtual void log(const char* level, const char* messageFormat, ...) = 0;
  virtual void log(const char* level, const wchar_t* messageFormat, ...) = 0;

  static std::shared_ptr<ILogger> default();
};


class ConsoleLogger
  : public ILogger {

public:
  virtual void log(const char* level, const char* messageFormat, ...);
  virtual void log(const char* level, const wchar_t* messageFormat, ...);
  
  static std::shared_ptr<ConsoleLogger> default() {
    if ( !defaultLogger.get() ) {
      defaultLogger.reset(new ConsoleLogger);
    }
    return defaultLogger;
  }

private:
  std::mutex mutex;
  static std::shared_ptr<ConsoleLogger> defaultLogger;

};


class FileLogger
  : public ILogger {

public:
  virtual void log(const char* level, const char* messageFormat, ...);
  virtual void log(const char* level, const wchar_t* messageFormat, ...);

  void setFilePath(const char* logger_file);
  
  static std::shared_ptr<FileLogger> default() {
    if ( !defaultLogger.get() ) {
      defaultLogger.reset(new FileLogger);
    }
    return defaultLogger;
  }

private:
  std::string filePath;
  std::mutex mutex;
  static std::shared_ptr<FileLogger> defaultLogger;

};

