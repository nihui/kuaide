#include "container.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QTimer>
#include <KWindowSystem>

#include "applets/dclock.h"
#include "applets/kbutton.h"
#include "applets/mnpager.h"
#include "applets/sctasks.h"
#include "applets/systray.h"

Container::Container(QWidget* parent) : QWidget(parent)
{
    KWindowSystem::setType(winId(), NET::Dock);
    KWindowSystem::setState(winId(), NET::Sticky);
    KWindowSystem::setState(winId(), NET::StaysOnTop);
    KWindowSystem::setOnAllDesktops(winId(), true);

    m_side = Bottom;
    m_occ = 28;

    declareArea();

    m_applets << new KButton(this);
    m_applets << new MNPager(this);
    m_applets << new SCTasks(this);
    m_applets << new SysTray(this);
    m_applets << new DClock(this);

    QTimer::singleShot(0, this, SLOT(init()));
}

Container::~Container()
{
    qDeleteAll(m_applets);
    m_applets.clear();
}

void Container::resizeEvent(QResizeEvent* event)
{
    const QSize& size = event->size();
    /// update layout
    updateLayout();
}

void Container::init()
{
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(declareArea()));

    foreach (Applet* applet, m_applets) {
        applet->init();
        connect(applet, SIGNAL(sizeChanged()), this, SLOT(updateLayout()));
    }

}

void Container::declareArea()
{
    const QRect r = QApplication::desktop()->screenGeometry();

    /// set wm area
    NETExtendedStrut strut;

    switch (m_side) {
        case Bottom:
            setGeometry(r.x(), r.height() - m_occ, r.width(), m_occ);
            strut.bottom_width = m_occ;
            strut.bottom_start = r.x();
            strut.bottom_end = r.x() + r.width() - 1;
            break;
        case Top:
            setGeometry(r.x(), r.top(), r.width(), m_occ);
            strut.top_width = m_occ;
            strut.top_start = r.x();
            strut.top_end = r.x() + r.width() - 1;
            break;
        case Left:
            setGeometry(r.x(), r.top(), m_occ, r.height());
            strut.left_width = m_occ;
            strut.left_start = r.top();
            strut.left_end = r.top() + r.height() - 1;
            break;
        case Right:
            setGeometry(r.width() - m_occ, r.top(), m_occ, r.height());
            strut.right_width = m_occ;
            strut.right_start = r.top();
            strut.right_end = r.top() + r.height() - 1;
            break;
    }

    KWindowSystem::setExtendedStrut(winId(),
                                    strut.left_width,   strut.left_start,   strut.left_end,
                                    strut.right_width,  strut.right_start,  strut.right_end,
                                    strut.top_width,    strut.top_start,    strut.top_end,
                                    strut.bottom_width, strut.bottom_start, strut.bottom_end);
    KWindowSystem::setStrut(winId(),
                            strut.left_width, strut.right_width, strut.top_width, strut.bottom_width);
}

void Container::updateLayout()
{
    int expands = 0;
    int freeSpace = width();
    foreach (const Applet* applet, m_applets) {
        int h = height();
        int w = applet->widthForHeight(h);
        if (w == 0) {
            ++expands;
        }
        else {
            freeSpace -= w;
        }
    }
    int widthForExpand = expands > 0 ? freeSpace / expands : freeSpace;

    int x = 0;
    foreach (Applet* applet, m_applets) {
        int h = height();
        int w = applet->widthForHeight(h);
        if (w == 0) {
            /// expanding
            applet->setGeometry(x, 0, widthForExpand, h);
            x += widthForExpand;
        }
        else {
            applet->setGeometry(x, 0, w, h);
            x += w;
        }
    }
}
