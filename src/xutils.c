/*
 * @Author: Opeheart w7302360@163.com
 * @Date: 2026-04-19 13:13:52
 * @LastEditors: Opeheart w7302360@163.com
 * @LastEditTime: 2026-04-19 14:21:06
 * @FilePath: \demo\src\xutils.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "xutils.h"
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
// #include "minwinbase.h"
#include "windows.h"
#elif defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#endif

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

const char* getTimeData(const char* format){
    time_t nowtime = time(NULL);
    struct tm* time_infor = localtime(&nowtime);
    static char buffer[80];
    /*strftime(buffer,sizeof(buffer),"%Y%m%d-%H_%M_%S",time_infor);*/
    if(!format)
        format = "%Y%m%d";
    strftime(buffer, sizeof(buffer), format, time_infor);
    return buffer;
}

bool xMakeDirectionary(const char* path) {
    // 1. 获取所需缓冲区大小（字符数，含结尾 null）
    int size_needed = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, 0);
    if (size_needed <= 0) return NULL;
    // 2. 分配宽字符串缓冲区
    LPCWSTR wstr = (wchar_t*)malloc(size_needed * sizeof(wchar_t));
    if (wstr == NULL) return NULL;
    MultiByteToWideChar(CP_ACP, 0, path, -1, wstr, size_needed);
    // 3. 遍历路径 通过分隔符选择文件夹 迭代创建
    size_t length = wcslen(wstr) + 1;
    wchar_t* dynamic_path = wstr;
    wprintf(L"path name = %s \n size = %d \n", wstr, length);
    for (size_t i = 0;i < length;i++) {
        if (*dynamic_path == L'\0') {
            if (CreateDirectory(wstr, NULL))
                wprintf(L"create directory %s\n", wstr);
            else {
                // 创建失败，获取错误码
                DWORD err = GetLastError();
                if (err == ERROR_ALREADY_EXISTS)
                    wprintf(L"already exists: %s\n", wstr);
                else
                    wprintf(L"create directory failed, error code: %lu\n", err);
            }
            break;
        }
        else if (*dynamic_path == L'\\') {
            *dynamic_path = L'\0';
            if(CreateDirectory(wstr, NULL))
			    wprintf(L"create directory %s\n", wstr);
            else {
                // 创建失败，获取错误码
                DWORD err = GetLastError();
                if (err == ERROR_ALREADY_EXISTS)
                    wprintf(L"already exists: %s\n", wstr);
                else
                    wprintf(L"create directory failed, error code: %lu\n", err);
            }
            *dynamic_path = L'\\';
        }
        dynamic_path++;
    }
    // 4. 释放资源 返回
	free(wstr);
    return true;
}

bool xGetAppFullPath(char const* path_buf,size_t buf_size) {
        #ifdef _WIN32
        wchar_t widePath[PATH_MAX_LENGTH];
        if (GetModuleFileNameW(NULL, widePath, PATH_MAX_LENGTH) == 0) {
            printf("GetModuleFileName failure");
            return false;
        }
        if (WideCharToMultiByte(CP_ACP, 0, widePath, -1, path_buf, buf_size, NULL, NULL) == 0) {
            printf("WideCharToMultiByte failed\n");
            return false;
        }
    #elif defined(__linux__)
        ssize_t len = readlink("/proc/self/exe", path_buf, buf_size - 1);
        if (len == -1) {
            printf("readlink /proc/self/exe failure");
            return false;
        }
        path_buf[len] = '\0';
    #endif

    return true;
}

bool xGetAppPath(char const* path_buf,size_t buf_size) {
    if(!xGetAppFullPath(path_buf,buf_size))
        return false;
    //移除文件名 保留目录
    #ifdef _WIN32
    char* last_slash = strrchr(path_buf, '\\');
    #elif defined(__linux__)
    char* last_slash = strrchr(path_buf, '/');
    #endif
    if (last_slash)
        (*last_slash) = '\0';
    return true;
}
