#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_INFO(fmt,...)         log_printf(LOG_INFOR,fmt,##__VA_ARGS__)
#define LOG_MSG(fmt,...)          log_printf(LOG_MESSAGE,fmt,##__VA_ARGS__)
#define LOG_WARN(fmt,...)         log_printf(LOG_WARNING,fmt,##__VA_ARGS__)
#define LOG_CRITI(fmt,...)        log_printf(LOG_CRITICAL,fmt,##__VA_ARGS__)
#define LOG_ERR(fmt,...)          log_printf(LOG_ERROR,fmt,##__VA_ARGS__)

typedef enum{
    LOG_MESSAGE = 0,
    LOG_INFOR ,
    LOG_WARNING,
    LOG_CRITICAL,
    LOG_ERROR,

    LOG_ENUM_MAX_LENGTH
}LOG_TYPE;

extern bool log_Init(char* prefix ,char* dir);
extern void log_DeInit();
extern void log_printf(LOG_TYPE type,char* format,...);
#endif // !LOGGER_H
