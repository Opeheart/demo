/*
 * @Author: Opeheart w7302360@163.com
 * @Date: 2026-03-27 21:44:51
 * @LastEditors: Opeheart w7302360@163.com
 * @LastEditTime: 2026-03-27 21:52:37
 * @FilePath: \1_testGit\demo\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "logger.h"

int main(int argc, char* argv[]) {
    log_Init("test","log");
    std::cout << "Hello, World!" << std::endl;
    LOG_INFO("hello test world\n");
    log_DeInit();
    return EXIT_SUCCESS;
}
