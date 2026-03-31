#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#elif defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#endif // DEBUG

#include "logger.h"

//不包含路径的日志文件的文件名的最大长度50字符
#define     LOG_NAME_MAX_LENGTH     50
//日志的整个文件路径最大长度
#define     LOG_DIR_MX_LENGTH       200
//日至时间辍的最小长度 从0开始 预留计算用
#define     LOG_TIMESTAMP_LENGTH    (sizeof("00:00:00:000") - 1)
//日至的文件夹默认名log的长度 从0开始 预留计算用
#define     LOG_FORDER_LENGTH       (sizeof("Log") - 1)
//整个日至的最大长度
#define     LOG_FILE_MAX_LENGTH     (LOG_NAME_MAX_LENGTH + LOG_DIR_MX_LENGTH)

char* log_dir = NULL;           //指向日志文件夹
char* log_file = NULL;          //直指日志文件 绝对路径
char* log_file_prefix = NULL;   //日志名前缀 默认：日志名前缀-time
const char* log_time = NULL;    //日志日期时间
char* log_name = NULL;          //日志名称  不包含路径
FILE* log_ptr = NULL;           //日志文件指针

/**
 * @brief 微妙级时间辍获取函数
 *      windows/linux兼容
 * @return  返回微妙级时间辍 格式[hour:minute:second:microsecond]
 */
const char* getMMsTimeStamp(){
    static char buffer[30];
#if defined(_WIN32) 
    SYSTEMTIME sys;   
    GetLocalTime( &sys );

    sprintf_s(buffer,sizeof(buffer),"[%d:%d:%d:%d]",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
    //printf("time mm = %s \n",buffer);
#elif defined(__linux__)
    struct timeval timvalue;
    int ret = gettimeofday(&timvalue,NULL);
    if(-1 == ret) {
        perror("get timeval structure failure");
        snprintf(buffer,sizeof(buffer),"[%s]","[errer timestamp]");
        return buffer;
    }
    struct tm* time_infor = localtime(&timvalue.tv_sec);
    snprintf(buffer,sizeof(buffer),"[%d:%d:%d:%d]",time_infor->tm_hour,time_infor->tm_min,time_infor->tm_sec,(int)(timvalue.tv_usec/1000));
#endif // DEBUG
    return buffer;
}

/**
 * @brief   获得时间
 *      时间格式：  年月日-时_分_秒 example:20001010-00_00_00
 */
const char* getTimeData(){
    time_t nowtime = time(NULL);
    struct tm* time_infor = localtime(&nowtime);
    static char buffer[30];
    strftime(buffer,sizeof(buffer),"%Y%m%d-%H_%M_%S",time_infor);
    return buffer;
}

bool logFilemake(const char* prefix,const char* path){
    if(prefix && (strlen(prefix) > 30)){
        printf("log path or filename too long to build!");
        // error = EPERM;
        return false;
    }

    if(path && (strlen(path) > 190)){
        printf("log dir or name too long to build!");
        // error = EPERM;
        return false;
    }
    
    static char fileprefix[LOG_NAME_MAX_LENGTH - LOG_TIMESTAMP_LENGTH] = {0};
    static char filename[LOG_NAME_MAX_LENGTH] = {0};
    static char dirname[LOG_DIR_MX_LENGTH] = {0};
    static char logfile[LOG_FILE_MAX_LENGTH] = {0};
    log_time = getTimeData();

    if(prefix != NULL)    //指定文件名前缀
    {
        #ifdef _WIN32
        strcpy_s(fileprefix,sizeof(fileprefix),prefix);
        #elif defined(__linux__)
        strcpy(fileprefix,prefix);
        #endif
    } else {
        #ifdef _WIN32
        strcpy_s(fileprefix,sizeof(fileprefix),"log");
        #elif defined(__linux__)
        strcpy(fileprefix,"log");
        #endif
    }
#ifdef _WIN32
    sprintf_s(filename,sizeof(filename),"%s-%s",fileprefix,log_time);
#elif defined(__linux__)
    snprintf(filename,sizeof(filename),"%s-%s",fileprefix,log_time);
#endif // DEBUG

    if(path != NULL)    //指定文件路径
    {
        #ifdef _WIN32
        sprintf_s(dirname,sizeof(dirname),"%s", path);
        #elif defined(__linux__)
        snprintf(dirname,sizeof(dirname),"%s",dir);
        #endif // DEBUG
    } else {
#ifdef _WIN32
        char exe_path[MAX_PATH];
        if (GetModuleFileName(NULL, exe_path, MAX_PATH) == 0) {
            printf("GetModuleFileName failure");
            return false;
        }
        // 移除文件名，保留目录
        char* last_slash = strrchr(exe_path, '\\');
        if (last_slash) {
            *last_slash = '\0';
        }
        strcpy_s(dirname, sizeof(dirname), exe_path);
#elif defined(__linux__)
        char exe_path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len == -1) {
            printf("readlink /proc/self/exe failure");
            return false;
        }
        exe_path[len] = '\0';
        // 移除文件名，保留目录
        char* last_slash = strrchr(exe_path, '/');
        if (last_slash) {
            *last_slash = '\0';
        }
        strcpy(dirname, exe_path);
#endif
    }
    if(strrchr(dirname,'\\') == (dirname + strlen(dirname) - 1))     //最后一个字符是 /
        memset(dirname + strlen(dirname) - 1,0,sizeof(dirname) - strlen(dirname) + 1);

    #ifdef _WIN32
    sprintf_s(logfile,sizeof(logfile),"%s\\%s",dirname,filename);
    #elif defined(__linux__)
    snprintf(logfile,sizeof(logfile),"%s/%s",dirname,filename);
    #endif
    
    log_dir = dirname;
    log_file_prefix = fileprefix;
    log_name = filename;
    log_file = logfile;

    printf("log_dir = %s\n",log_dir);
    printf("log_file_prefix = %s\n",log_file_prefix);
    printf("log_name = %s\n",log_name);
    printf("log_file = %s\n",log_file);
    printf("**************************\n");
    //log_file
    //TO DO...

    return true;
}

void log_DeInit(){
    if(!log_ptr)
        return;
    fflush(log_ptr);
    fclose(log_ptr);
}

bool log_Init(const char* prefix,const char* dirpath){
    if(!logFilemake(prefix, dirpath)){
        printf("log file analise failure,log start terminal\n");
        return false;
    }

    if((log_ptr = fopen(log_file,"a+")) == NULL){
        printf("Log file{%s}-{" PRIu64 "} creat failure,log start terminal\n",log_file,log_ptr);
        perror("file open failure:");
        return false;
    }
    
    return true;
}

void log_printf(LOG_TYPE type,const char* format,...){
    static char buffer[200];
    static const char* line_head = NULL;

    va_list _ArgList;
    va_start(_ArgList, format);
    #ifdef _WIN32
    _vsprintf_l(buffer, format, NULL, _ArgList);
    #elif defined(__linux__)
    vsnprintf(buffer, sizeof(buffer),format, _ArgList);
    #endif  //defined(_Win32)

    va_end(_ArgList);

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
