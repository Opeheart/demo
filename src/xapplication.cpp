/*
 * @Author: Opeheart w7302360@163.com
 * @Date: 2026-04-11 19:40:10
 * @LastEditors: Opeheart w7302360@163.com
 * @LastEditTime: 2026-04-12 22:12:02
 * @FilePath: \demo\src\xApplication.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "xapplication.h"
#include <QPainter>
#include <QDebug>

XApplication::XApplication(QWidget* parent)
    : QWidget(parent)
    , m_graphicsScene(new XGraphicsScene(this))
    , m_graphicsView(new QGraphicsView(this))
{
    resize(400, 300);

    m_img = QImage("G:/1_projectFile/1_testGit/test1.png");
    // if(m_img.load("G:/1_projectFile/1_testGit/test1.png")) {
    //     qWarning() << "loag img failure";
    //     m_img = QImage(1920,1080,QImage::Format_RGB888);
    //     m_img.fill(Qt::green);
    // }

    // m_graphicsScene->setSceneRect(0, 0, width(), height());
    m_graphicsScene->setSceneRect(0, 0, m_img.width(), m_img.height());

    m_graphicsScene->setBackgroundBrush(Qt::lightGray);
    m_graphicsScene->addPixmap(QPixmap::fromImage(m_img));

    m_graphicsView->setScene(m_graphicsScene);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_graphicsView->setGeometry(rect());
    m_graphicsView->setFrameShape(QFrame::NoFrame);
}

XApplication::~XApplication()
{
}
