/*
 * @Author: Opeheart w7302360@163.com
 * @Date: 2026-04-01 02:29:15
 * @LastEditors: Opeheart w7302360@163.com
 * @LastEditTime: 2026-04-18 18:47:53
 * @FilePath: \demo\src\logger.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef LOGGER_H
#define LOGGER_H

#define LOG_DEBUG(fmt,...)        log_printf(LOG_DEBUG,fmt,##__VA_ARGS__)
#define LOG_INFO(fmt,...)         log_printf(LOG_INFOR,fmt,##__VA_ARGS__)
#define LOG_WARN(fmt,...)         log_printf(LOG_WARNING,fmt,##__VA_ARGS__)
#define LOG_CRITI(fmt,...)        log_printf(LOG_CRITICAL,fmt,##__VA_ARGS__)
#define LOG_ERR(fmt,...)          log_printf(LOG_ERROR,fmt,##__VA_ARGS__)

/**
 * 日志类型枚举
 */
typedef enum{
    LOG_DEBUG = 0,      //调试类型
    LOG_INFOR ,         //普通类型
    LOG_WARNING,        //警告类型
    LOG_ERROR,          //错误类型
    LOG_CRITICAL,       //致命类型

    LOG_ENUM_MAX_LENGTH
}LOG_TYPE;

#ifdef  __cplusplus
extern "C"{
#else
#include <stdbool.h>
#endif

//日志配置结构体
typedef struct LogConfig{
    char* log_path;         //日志文件夹路径
    char* log_full_path;    //日志文件完整路径
    char* log_name;         //日志文件名
    char* log_name_prefix;  //日志文件名前缀
    char* log_name_surfix;  //日志文件名后缀
    const char* log_time;         //日志时间戳
}LogConfig;

/**
 * @brief   日志初始化函数
 *      必须和log_DeInit成对使用
 * @param   prefix:日志文件名字前缀
 * @param   dir:指定日志文件路径名称，默认在可执行文件根目录下
 */
extern bool log_Init(const char* prefix ,const char* surfix, const char* dirpath);
/**
 * @brief   日志模块资源释放函数
 *      软件关闭时调用 和log_Init(...)配合使用
 *      保证日志文件的完整性 释放内部资源
 */
extern void log_DeInit();
/**
 * @brief   格式化打印日志函数
 *      推荐使用宏定义输出日志消息
 * @param   type:日志类型 参考枚举LOG_TYPE
 * @param   format  格式化消息，支持标准格式化
 */
extern void log_printf(LOG_TYPE type,const char* format,...);

#ifdef  __cplusplus
}
#endif  //__cplusplus

#endif // !LOGGER_H
