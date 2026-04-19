#ifndef XAPPLICATION_H
#define XAPPLICATION_H

#include <QWidget>
#include <QGraphicsView>
#include "xgraphicsscene.h"

class XApplication : public QWidget
{
    Q_OBJECT

public:
    XApplication(QWidget* parent = nullptr);
    ~XApplication();

private:
    QGraphicsView* m_graphicsView;
    XGraphicsScene* m_graphicsScene;

    QImage m_img;
};

#endif // !XAPPLICATION_H
