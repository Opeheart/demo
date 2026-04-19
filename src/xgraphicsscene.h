#ifndef XGRAPHICSSCENE_H
#define XGRAPHICSSCENE_H
#include <QGraphicsScene>

class XGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    XGraphicsScene(QObject *parent = nullptr);
};

#endif // !XGRAPHICSSCENE_H