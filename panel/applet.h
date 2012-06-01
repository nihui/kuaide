#ifndef APPLET_H
#define APPLET_H

#include <QWidget>

class Applet : public QWidget
{
    Q_OBJECT
public:
    explicit Applet(QWidget* parent = 0);
    virtual ~Applet();
    virtual void init() = 0;
    /// return 0 if expanding
    virtual int heightForWidth(int width) const;
    virtual int widthForHeight(int height) const;
Q_SIGNALS:
    void sizeChanged();
};

#endif // APPLET_H
