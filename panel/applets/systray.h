#ifndef SSTRAY_H
#define SSTRAY_H

#include <QX11EmbedContainer>

#include "applet.h"

// #include <X11/Xlib.h>
#include <X11/Xdefs.h>

class SysTrayWidget;

class SysTray : public Applet
{
Q_OBJECT
public:
    explicit SysTray(QWidget* parent = 0);
    virtual ~SysTray();
    virtual void init();
    virtual int heightForWidth( int width ) const;
    virtual int widthForHeight( int height ) const;
protected:
    virtual bool x11Event( XEvent* event );
private Q_SLOTS:
    void updateSysTray();
private:
    void updateLayout();
    QList<SysTrayWidget*> SysTrayWidgetList;
    // These need to remain allocated for the duration of our lifetime
//         Atom m_selectionAtom;
    Atom m_opcodeAtom;
};

class SysTrayWidget : public QX11EmbedContainer
{
Q_OBJECT
public:
    explicit SysTrayWidget( QWidget* parent = 0 );
    virtual ~SysTrayWidget();
    bool embedSystemTrayClient( WId clientId );
protected:
//         virtual bool x11Event( XEvent* event );
private:
    bool prepareFor( WId id );

//         XWindowAttributes attr;
};

#endif // SSTRAY_H
