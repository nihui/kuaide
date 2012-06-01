#ifndef DCLOCK_H
#define DCLOCK_H

#include "applet.h"

class DClock : public Applet
{
Q_OBJECT
public:
    explicit DClock( QWidget* parent );
    virtual ~DClock();
    virtual void init();
    virtual int heightForWidth( int width ) const;
    virtual int widthForHeight( int height ) const;
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    QSize m_textSize;
};

#endif // DCLOCK_H
