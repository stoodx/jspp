#include "ILogger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <fstream>


// Hide annoying naming differences between Windows and other platforms
#ifdef WIN32
    #define my_vsnprintf _vsnprintf_s
    #define my_vsnwprintf _vsnwprintf_s
#else
    #define my_vsnprintf vsnprintf
    #define my_vsnwprintf vsnwprintf
#endif




ILogger::ILogger(void)
{
}


ILogger::~ILogger(void)
{
}

std::shared_ptr<ILogger> ILogger::default() {
 #ifdef USE_CONSOLE_LOGGER
  return ConsoleLogger::default();
#else 
  return FileLogger::default();
#endif
}

// Console Logger class
std::shared_ptr<ConsoleLogger> ConsoleLogger::defaultLogger;

void ConsoleLogger::log(const char* level, const char* messageFormat, ...) {
  std::unique_lock<std::mutex> lock(mutex);
  va_list argList;
  va_start(argList, messageFormat);

  char target_string[4096];

  // Calculate the final length of the formatted string
  int len = my_vsnprintf( target_string, _countof(target_string), _TRUNCATE, messageFormat, argList );

  // Clean up
  va_end( argList );

  SYSTEMTIME systemTime;
  GetLocalTime(&systemTime);

  std::string timeStr;
  //Creates a string with this format HH:MM:SS.mmmm

  
  std::ostringstream timeStream;
  timeStream << std::setw(2) << std::setfill('0') << systemTime.wHour 
      << ":" << std::setw(2) << std::setfill('0') << systemTime.wMinute
      << ":" << std::setw(2) << std::setfill('0') << systemTime.wSecond
      << ":" << std::setw(0) << std::setfill(' ') << systemTime.wMilliseconds;
  
  std::string ss = timeStream.str();
  
  printf("%s %6s: %s\n", ss.c_str(), level, target_string);
}

void ConsoleLogger::log(const char* level, const wchar_t* messageFormat, ...) {
  std::unique_lock<std::mutex> lock(mutex);
  va_list argList;
  va_start(argList, messageFormat);

  wchar_t target_string[4096];

  // Calculate the final length of the formatted string
  int len = my_vsnwprintf( target_string, _countof(target_string), _TRUNCATE, messageFormat, argList );

  // Clean up
  va_end( argList );

  SYSTEMTIME systemTime;
  GetLocalTime(&systemTime);

  std::wstring timeStr;
  //Creates a string with this format HH:MM:SS.mmmm

  std::wostringstream timeStream;
  timeStream  << std::setw(2) << std::setfill(L'0') << systemTime.wHour 
      << L":" << std::setw(2) << std::setfill(L'0') << systemTime.wMinute
      << L":" << std::setw(2) << std::setfill(L'0') << systemTime.wSecond
      << L":" << std::setw(0) << std::setfill(L' ') << systemTime.wMilliseconds;

  std::wstring timeStreamStr = timeStream.str();
  
  wprintf(L"%s", timeStreamStr.c_str());
  printf(" %6s: ", level);
  wprintf(L"%s\n", target_string);
  
}

// File Logger class
std::shared_ptr<FileLogger> FileLogger::defaultLogger;
  

void FileLogger::setFilePath(const char* logger_file) {
  filePath = logger_file;
}

void FileLogger::log(const char* level, const char* messageFormat, ...) {
  std::unique_lock<std::mutex> lock(mutex);
  va_list argList;
  va_start(argList, messageFormat);

  char target_string[4096];

  // Calculate the final length of the formatted string
  int len = my_vsnprintf( target_string, _countof(target_string), _TRUNCATE, messageFormat, argList );

  // Clean up
  va_end( argList );

  SYSTEMTIME systemTime;
  GetLocalTime(&systemTime);

  std::string timeStr;
  //Creates a string with this format HH:MM:SS.mmmm

  
  std::ostringstream timeStream;
  timeStream << std::setw(2) << std::setfill('0') << systemTime.wHour 
      << ":" << std::setw(2) << std::setfill('0') << systemTime.wMinute
      << ":" << std::setw(2) << std::setfill('0') << systemTime.wSecond
      << ":" << std::setw(0) << std::setfill(' ') << systemTime.wMilliseconds;
  
  std::string ss = timeStream.str();

  if ( !filePath.empty() ) {

    std::ofstream fileStream(filePath.c_str(),  std::ios_base::out | std::ios_base::app );

    fileStream << ss.c_str() << " " 
               << level << ": " 
               << target_string << std::endl;

    fileStream.close();
  }
}

void FileLogger::log(const char* level, const wchar_t* messageFormat, ...) {
  std::unique_lock<std::mutex> lock(mutex);
  va_list argList;
  va_start(argList, messageFormat);

  wchar_t target_string[4096];

  // Calculate the final length of the formatted string
  int len = my_vsnwprintf( target_string, _countof(target_string), _TRUNCATE, messageFormat, argList );

  // Clean up
  va_end( argList );

  SYSTEMTIME systemTime;
  GetLocalTime(&systemTime);

  std::wstring timeStr;
  //Creates a string with this format HH:MM:SS.mmmm

  std::wostringstream timeStream;
  timeStream  << std::setw(2) << std::setfill(L'0') << systemTime.wHour 
      << L":" << std::setw(2) << std::setfill(L'0') << systemTime.wMinute
      << L":" << std::setw(2) << std::setfill(L'0') << systemTime.wSecond
      << L":" << std::setw(0) << std::setfill(L' ') << systemTime.wMilliseconds;

  std::wstring timeStreamStr = timeStream.str();
  
  if ( !filePath.empty() ) {
    std::wofstream fileStream(filePath.c_str(),  std::ios_base::out | std::ios_base::app );

    fileStream << timeStreamStr.c_str() << L" " 
                << level << L": " 
                << target_string << std::endl;

    fileStream.close();
  } 
}