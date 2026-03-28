#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <inttypes.h>

#ifdef _WIN32
#include <windows.h>
#endif // DEBUG

#include "logger.h"

#define     LOG_NAME_MAX_LENGTH     50
#define     LOG_DIR_MX_LENGTH       200
#define     LOG_TIMESTAMP_LENGTH    (sizeof("00:00:00:000") - 1)
#define     LOG_FORDER_LENGTH       (sizeof("Log") - 1)
#define     LOG_FILE_MAX_LENGTH     (LOG_NAME_MAX_LENGTH + LOG_DIR_MX_LENGTH)

char* log_dir = NULL;           //指向日志文件夹
char* log_file = NULL;          //直指日志文件 绝对路径
char* log_file_prefix = NULL;   //日志名前缀 默认：日志名前缀-time
char* log_time = NULL;          //日志日期时间
char* log_name = NULL;          //日志名称  不包含路径
FILE* log_ptr = NULL;

char* getMMsTimeStamp(){
    static char buffer[30];
#if defined(_WIN32) 
    SYSTEMTIME sys;   
    GetLocalTime( &sys );

    sprintf_s(buffer,sizeof(buffer),"[%d:%d:%d:%d]",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
    printf("time mm = %s \n",buffer);
#elif defined(__linux__)
    TO DO..
#endif // DEBUG
    return buffer;
}

char* getTimeData(){
    time_t nowtime = time(NULL);
    struct tm* time_infor = localtime(&nowtime);
    static char buffer[30];
    strftime(buffer,sizeof(buffer),"%Y%m%d-%H_%M_%S",time_infor);
    return buffer;
}

bool logFilemake(char* prefix,char* dir){
        printf("1111111111\n");
    if(prefix && (strlen(prefix) > 30)){
        printf("log dir or name too long to build!");
        // error = EPERM;
        return false;
    }

    if(dir && (strlen(dir) > 190)){
        printf("log dir or name too long to build!");
        // error = EPERM;
        return false;
    }
    
    printf("222222\n");
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
            TO DO...
        #endif
    } else {
        #ifdef _WIN32
        strcpy_s(fileprefix,sizeof(fileprefix),"log");
        #elif defined(__linux__)
            TO DO...
        #endif
    }
#ifdef _WIN32
    sprintf_s(filename,sizeof(filename),"%s-%s",fileprefix,log_time);
#elif defined(__linux__)
    TO DO...
#endif // DEBUG

    if(dir != NULL)    //指定文件路径
    {
        #ifdef _WIN32
        sprintf_s(dirname,sizeof(dirname),"%s",dir);
        #elif defined(__linux__)
        TO DO...
        #endif // DEBUG
    } else {
        getcwd(dirname,sizeof(dirname));   
    }
    if(strrchr(dirname,'\\') == (dirname + strlen(dirname) - 1))     //最后一个字符是 /
        memset(dirname + strlen(dirname) - 1,0,sizeof(dirname) - strlen(dirname) + 1);

    sprintf_s(logfile,sizeof(logfile),"%s\\%s",dirname,filename);

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

bool log_Init(char* prefix,char* dir){
    if(!logFilemake(prefix,dir)){
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

void log_printf(LOG_TYPE type,char* format,...){
    static char buffer[200];
    static char* time_stamp = NULL;
    static char* line_head = NULL;

    va_list _ArgList;
    va_start(_ArgList, format);
    _vsprintf_l(buffer, format, NULL, _ArgList);

    va_end(_ArgList);

    switch (type)
    {
    case LOG_MESSAGE:
        line_head = "MESSAGE";
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
        time_stamp = getMMsTimeStamp();
        fprintf(log_ptr,"%s [%s]:%s\n",time_stamp,line_head,buffer);
        fprintf(stdout,"%s [%s]:%s\n",time_stamp,line_head,buffer);
        // fprintf(stdout,"%s [%s]\n",getMMsTimeStamp(),line_head);
    } else {
        fprintf(stdout,"Unknown file operation : illegal");
    }
}
