#ifndef CONTAINER_H
#define CONTAINER_H

#include <QList>
#include <QWidget>

class Applet;

class Container : public QWidget
{
Q_OBJECT
public:
    enum Side {
        Bottom,
        Top,
        Left,
        Right
    };
    explicit Container(QWidget* parent = 0);
    virtual ~Container();
protected:
    virtual void resizeEvent(QResizeEvent* event);
private Q_SLOTS:
    void init();
    void declareArea();
    void updateLayout();
private:
    Side m_side;
    int m_occ;
    QList<Applet*> m_applets;
};

#endif // CONTAINER_H
