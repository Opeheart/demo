#ifndef XUTILS_H
#define XUTILS_H

#ifdef  __cplusplus
extern "C"{
#else
#include <stdbool.h>
#endif

#ifdef _WIN32
#ifdef MAX_PATH
#define     PATH_MAX_LENGTH     MAX_PATH
#else
#define     PATH_MAX_LENGTH     260
#endif
#elif defined(__linux__)
#ifdef PATH_MAX
#define     PATH_MAX_LENGTH     PATH_MAX
#else
#define     PATH_MAX_LENGTH     4096
#endif 
#endif

/**
 * @brief 微秒级时间辍获取函数
 *      windows/linux兼容
 * @return  返回微秒级时间辍 格式[hour:minute:second:microsecond]
 */
extern const char* getMMsTimeStamp(void);

/**
 * @brief   获得时间
 *      时间格式：  年月日-时_分_秒 example:20001010-00_00_00
 * @param   format :const char*类型，比如："%Y%m%d"(默认) 或者 %Y%m%d-%H_%M_%S
 *      当format == NULL时，默认格式化为："%Y%m%d"
 * @attention   格式化字符串得到的结果长度不能超过80个字节
 */
extern const char* getTimeData(const char* format);

/**
 * @brief   迭代创建文件夹 支持中文路径创建
 * @param   path :const char*,要创建的文件夹路径
 */
extern bool xMakeDirectionary(const char* path);

/**
 * @brief   获得当前可执行文件的包含文件名的完整路径
 * @param   path_buf :char const*类型，存储根目录完整路径结果的buffer
 * @param   buf_size :size_t类型，path_buffer的有效空间大小
 */
extern bool xGetAppFullPath(char const* path_buf,size_t buf_size);

/**
 * @brief   获得当前可执行文件的根目录 只包含目录
 * @param   path_buf :char const*类型，存储根目录结果的buffer
 * @param   buf_size :size_t类型，path_buffer的有效空间大小
 * @attention   目录结尾没有默认分隔符'/'或'\\'
 */
extern bool xGetAppPath(char const* path_buf,size_t buf_size);

#ifdef  __cplusplus
}
#endif

#endif // XUTIL_H