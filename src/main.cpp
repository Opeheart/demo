/*
 * @Author: Opeheart w7302360@163.com
 * @Date: 2026-03-27 21:44:51
 * @LastEditors: Opeheart w7302360@163.com
 * @LastEditTime: 2026-04-19 15:59:05
 * @FilePath: \1_testGit\demo\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "logger.h"
#include <QApplication>
#include "xapplication.h"
#include <QWidget>
#include <QDebug>

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    log_Init("test", NULL, NULL);
    LOG_INFO("application is starting...");
    XApplication w;
    w.show();
    qInfo() << "this is first vs qt proj";
    LOG_INFO("this is first vs qt proj");
    app.exec();

    std::cout << "Hello, World!" << std::endl;
    LOG_INFO("application is exiting...");
    log_DeInit();
    return EXIT_SUCCESS;
}
