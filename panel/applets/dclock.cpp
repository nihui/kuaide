#include "dclock.h"

#include <QApplication>
#include <QTimer>
#include <QDateTime>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <KIcon>
#include <KLocale>

DClock::DClock( QWidget* parent ) : Applet(parent)
{
}

DClock::~DClock()
{
}

void DClock::init()
{
    QFont font = QApplication::font();
    font.setPointSize(16);
    QFontMetrics fm(font);
    m_textSize = fm.size(Qt::TextSingleLine, "00:00");

    QTimer* timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(update()) );
    // update every second
    timer->start( 1000 );

    update();
}

int DClock::heightForWidth( int width ) const
{
    return m_textSize.height();
}

int DClock::widthForHeight( int height ) const
{
    return m_textSize.width();
}

void DClock::paintEvent(QPaintEvent* event)
{
    QTime time = QTime::currentTime();
    QString text = time.toString( "hh:mm" );
    if ( time.second() % 2 == 0 )
        text[2] = ' ';


    QFont font = QApplication::font();
    font.setPointSize(16);

    QPainter p(this);
    p.setFont(font);
    p.drawText(0, 0, width(), height(), Qt::AlignCenter, text);
}
