#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>

#ifdef _WIN32
 #include <windows.h>
//#include <windef.h>
#include <direct.h>
#elif defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#endif

#include "xutils.h"
#include "logger.h"

//不包含路径的日志文件的文件名的最大字符长度定义
#define     LOG_NAME_MAX_LENGTH     50
//日志时间辍长度计算
#define     LOG_TIMESTAMP_BUFFER_LENGTH    (sizeof("00:00:00:000"))
//日志文件名时间戳长度定义
//#define     LOG_NAME_TIMESTAMP_BUFFER_LENGTH   (sizeof("20001010-00_00_00"))
 #define     LOG_NAME_TIMESTAMP_BUFFER_LENGTH   (sizeof("20001010"))
//日志默认文件夹名称
#define     LOG_DEFAULT_DIR       "Log"
//日志的默认文件夹名（LOG_DEFAULT_DIR）称长度自计算宏
#define     LOG_DEFAULT_DIR_LENGTH       (sizeof(LOG_DEFAULT_DIR) - 1)
//日志文件的完整路径的最大字符长度定义
#ifdef _WIN32
#ifdef MAX_PATH
#define     LOG_FULL_PATH_MAX_LENGTH     MAX_PATH
#else
#define     LOG_FULL_PATH_MAX_LENGTH     260
#endif
#elif defined(__linux__)
#ifdef PATH_MAX
#define     LOG_FULL_PATH_MAX_LENGTH     PATH_MAX
#else
#define     LOG_FULL_PATH_MAX_LENGTH     4096
#endif 
#endif


FILE* log_ptr = NULL;           //日志文件指针
static LogConfig log_config = { NULL };              //日志配置结构体实例


bool logFilemake(const char* prefix, const char* surfix, const char* path){
    //日志文件名前缀或后缀过长无法构建完整日志文件路径
    if( (prefix && (strlen(prefix) > (LOG_NAME_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH))) || 
        (surfix && (strlen(surfix) > (LOG_NAME_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH))) )
    {
        printf("log prefix or surfix is too long to build log file!");
        // error = EPERM;
        return false;
    }
    //路径长度过长无法构建完整日志文件路径
    if(path && (strlen(path) > (LOG_FULL_PATH_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH )))
    {
        printf("log path is too long to build log file!");
        // error = EPERM;
        return false;
    }

    //静态变量
    static char log_name[LOG_NAME_MAX_LENGTH] = {0};
    static char log_prefix[LOG_NAME_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH] = {0};
    static char log_surfix[LOG_NAME_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH] = {0};
    static char log_path[LOG_FULL_PATH_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH] = {0};
    static char log_full_path[LOG_FULL_PATH_MAX_LENGTH] = {0};
    log_config.log_time = getTimeData(NULL);

    //确定参数
    prefix = prefix ? prefix : "log";
    surfix = surfix ? surfix : "";
#ifdef _WIN32
    const char separator = '\\';
#elif defined(__linux__)
    const char separator = '/';
#endif
    if(!path)
    {
        //获得可执行文件完整路径
        if(!xGetAppPath(log_full_path, sizeof(log_full_path)))
            return false;
        //移除文件名 保留目录
        if (strlen(log_full_path) > (LOG_FULL_PATH_MAX_LENGTH - strlen("log") - 1)) {
            printf("file full path is too long to creat default log folder \"log\"\n");
            return false;
        }
        snprintf(log_full_path + strlen(log_full_path), 
                LOG_FULL_PATH_MAX_LENGTH - strlen(log_full_path) - 1, 
                "%clog", separator);
        path = log_full_path;   //确定path的值
    }
    xMakeDirectionary(path);
    //日志路径长度判断
    size_t length_path = strlen(path);
    if(strrchr(path,separator) != (path + length_path - 1))
        length_path += 1; //如果路径末尾没有分隔符 则需要加上分隔符的长度
    if((LOG_FULL_PATH_MAX_LENGTH - LOG_NAME_TIMESTAMP_BUFFER_LENGTH) < length_path) {
        printf("log path is too long to build log file!");
        // error = EPERM;
        return false;
    }
    //日志文件名长度计算
    size_t length_name = strlen(log_prefix) + strlen(log_surfix) + LOG_NAME_TIMESTAMP_BUFFER_LENGTH;
    //日志文件名长度过长无法构建完整日志文件路径
    if((LOG_NAME_MAX_LENGTH) < length_name)
    {
        printf("log name is too long to build log file!");
        // error = EPERM;
        return false;
    }
    //日志文件完整路径长度计算
    size_t length_full_path = length_path + length_name;
    //日志文件完整路径长度过长无法构建完整日志文件路径
    if((LOG_FULL_PATH_MAX_LENGTH - 1) < length_full_path)
    {
        printf("log full path is too long to build log file!");
        // error = EPERM;
        return false;
    }
    //长度都检查完毕
    //确定静态参数
    #ifdef _WIN32
    strcpy_s(log_prefix, sizeof(log_prefix), prefix);
    strcpy_s(log_surfix, sizeof(log_surfix), surfix);
    strcpy_s(log_path, sizeof(log_path), path);
    #elif defined(__linux__)
    snprintf(log_prefix, sizeof(log_prefix), "%s", prefix);
    snprintf(log_surfix, sizeof(log_surfix), "%s", surfix);
    snprintf(log_path, sizeof(log_path), "%s", path);
    #endif

    if(strrchr(log_path,separator) != (log_path + length_path - 1))
    {
        //路径末尾没有分隔符 则需要加上分隔符
        #ifdef _WIN32
        strcat_s(log_path, sizeof(log_path), "\\");
        #elif defined(__linux__)
        strncat(log_path, "/", sizeof(log_path) - length_path - 1);
        #endif
    }
#ifdef _WIN32
    sprintf_s(log_name,sizeof(log_name),"%s%s%s",log_prefix,log_config.log_time,log_surfix);
    sprintf_s(log_full_path,sizeof(log_full_path),"%s%s",log_path,log_name);
#elif defined(__linux__)
    snprintf(log_name,sizeof(log_name),"%s%s%s",log_prefix,log_config.log_time,log_surfix);
    snprintf(log_full_path,sizeof(log_full_path),"%s%s",log_path,log_name);
#endif
    
    log_config.log_name_prefix = log_prefix;
    log_config.log_name_surfix = log_surfix;
    log_config.log_path = log_path;
    log_config.log_name = log_name;
    log_config.log_full_path = log_full_path;

    printf("*********log_config information [Start]***********\n");
    printf("log_path = %s\n",log_config.log_path);
    printf("log_full_path = %s\n",log_config.log_full_path);
    printf("log_name = %s\n",log_config.log_name);
    printf("log_name_prefix = %s\n",log_config.log_name_prefix);
    printf("log_name_surfix = %s\n",log_config.log_name_surfix);
    printf("*********log_config information************\n");
    
    //log_file
    //TO DO...

    return true;
}

void log_DeInit(){
    if(!log_ptr)
        return;
    LOG_INFO("**********log end recording***********\n");
    fflush(log_ptr);
    fclose(log_ptr);
}

bool log_Init(const char* prefix ,const char* surfix, const char* dirpath){
    if(!logFilemake(prefix, surfix, dirpath)){
        printf("log file analise failure,log start terminal\n");
        return false;
    }

    if((log_ptr = fopen(log_config.log_full_path,"a+")) == NULL){
        printf("Log file{%s}-{" PRIxPTR "} creat failure,log start terminal\n", log_config.log_full_path,(uintptr_t)log_ptr);
        perror("file open failure:");
        return false;
    }
    LOG_INFO("**********log start recording***********");
    LOG_INFO("***log start time : %s ***", log_config.log_time);
    return true;
}

void log_printf(LOG_TYPE type,const char* format,...){
    static char buffer[256] = {0x00};
    static const char* line_head = NULL;

    va_list argList;
    va_start(argList, format);
    vsnprintf(buffer, sizeof(buffer),format, argList);
    va_end(argList);

    switch (type)
    {
    case LOG_DEBUG:
        line_head = "DEBUG";
        break;
    case LOG_INFOR:
        line_head = "INFOR";
        break;
    case LOG_WARNING:
        line_head = "WARN";
        break;
    case LOG_CRITICAL:
        line_head = "CRITICAL";
        break;
    case LOG_ERROR:
        line_head = "ERROR";
        break;
    default:
        line_head = "UNKNOWN";
        break;
    }
// sprintf()
    // sprintf(buffer,"%s [%s]:%s",getMMsTimeStamp(),line_head,buffer);
    if(log_ptr)
    {
        static const char* time_stamp = NULL;
        time_stamp = getMMsTimeStamp();
        fprintf(log_ptr,"%s [%s]:%s\n",time_stamp,line_head,buffer);
        fprintf(stdout,"%s [%s]:%s\n",time_stamp,line_head,buffer);
        // fprintf(stdout,"%s [%s]\n",getMMsTimeStamp(),line_head);
    } else {
        fprintf(stdout,"Unknown file operation : illegal");
    }
}
