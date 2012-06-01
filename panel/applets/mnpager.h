#ifndef MNPAGER_H
#define MNPAGER_H

#include <QList>
#include <QHash>
#include <QPoint>
#include <QRectF>
#include <QWidget>
#include <KWindowSystem>

#include "applet.h"

class QMouseEvent;
class QPaintEvent;
class QTimer;

class MNPager : public Applet
{
Q_OBJECT
public:
    explicit MNPager( QWidget* parent = 0 );
    virtual ~MNPager();
    virtual void init();
    virtual int heightForWidth(int width) const;
    virtual int widthForHeight(int height) const;
protected:
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void paintEvent( QPaintEvent* event );
private Q_SLOTS:
    void currentDesktopChanged( int desktop );
    void activeWindowChanged( WId id );
    void numberOfDesktopsChanged( int num );
    void windowAdded( WId id );
    void windowRemoved( WId id );
    void windowChanged( WId id, unsigned int properties );
    void stackingOrderChanged();
    void showingDesktopChanged( bool showing );

    void refresh();
private:
    ///====================///
    int m_desktopCount;
    int m_currentDesktop;
    WId m_activeWindowId;

    QList<QRectF> m_desktops;
    QHash<WId, KWindowInfo> m_windows;

    int m_dragStartDesktop;
    WId m_dragWindowId;
    QRectF m_dragWindowRect;
    QPointF m_dragOriginalPos;

    int m_rows;
    qreal m_scaleFactor;
    QTimer* m_timer;
};

#endif // MNPAGER_H
