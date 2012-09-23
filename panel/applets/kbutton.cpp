#include "kbutton.h"
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QSignalMapper>
#include <QToolButton>
#include <KDebug>
#include <KLocale>
#include <KPluginFactory>
#include <KMenu>
#include <KIcon>
#include <KServiceGroup>
#include <KAuthorized>
#include <KRun>
#include <KToolInvocation>
#include <kworkspace/kworkspace.h>

KButton::KButton(QWidget* parent) : Applet(parent)
{
}

KButton::~KButton()
{
}

void KButton::init()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    m_button = new QToolButton;
    m_button->setIcon(KIcon("start-here-kde"));
    m_button->setIconSize(QSize(24, 24));
    m_button->setAutoRaise(true);
    layout->addWidget(m_button);

    m_signalMapper = new QSignalMapper(this);

    m_menu = new QMenu(this);
    m_menu->clear();

    fillMenu(m_menu, KServiceGroup::root());
    m_menu->addSeparator();

    if (KAuthorized::authorizeKAction("run_command")) {
        m_menu->addAction(KIcon("system-run"), i18n("Run Command..."), this, SLOT(slotRunCommand()));
        m_menu->addSeparator();
    }

    if (KAuthorized::authorizeKAction("logout")) {
        m_menu->addAction(KIcon("application-exit"), i18n("Log Out..."), this, SLOT(slotLogout()));
    }

    connect(m_signalMapper, SIGNAL(mapped(const QString&)), this, SLOT(slotExec(const QString&)));
    connect(m_button, SIGNAL(clicked()), this, SLOT(toggleMenu()));
}

void KButton::toggleMenu()
{
    QPoint globalPos = mapToGlobal(m_button->pos());
    m_menu->move(globalPos.x(), globalPos.y() - m_menu->sizeHint().height());
    m_menu->show();
}

void KButton::slotExec(const QString& entryPath)
{
    KToolInvocation::startServiceByDesktopPath(entryPath, QStringList(), 0, 0, 0, "", true);
}

void KButton::slotRunCommand()
{
    KRun::runCommand("krunner", 0);
}

void KButton::slotLogout()
{
    KWorkSpace::requestShutDown(KWorkSpace::ShutdownConfirmDefault,
                                KWorkSpace::ShutdownTypeDefault,
                                KWorkSpace::ShutdownModeDefault);
}

void KButton::fillMenu(QMenu* menu, KServiceGroup::Ptr menuRoot)
{
    KServiceGroup::List list = menuRoot->entries(true, true, false, false);
    KServiceGroup::List::ConstIterator it = list.constBegin();
    KServiceGroup::List::ConstIterator end = list.constEnd();
    for (; it != end; ++it) {
        const KSycocaEntry::Ptr e = *it;
        if ( e->isType( KST_KServiceGroup ) ) {
            KServiceGroup::Ptr subMenuRoot( KServiceGroup::Ptr::staticCast( e ) );
            if ( subMenuRoot->childCount() == 0 )
                continue;
            QMenu* subMenu = menu->addMenu( KIcon( subMenuRoot->icon() ), subMenuRoot->caption() );
            fillMenu( subMenu, subMenuRoot );
        }
        else if ( e->isType( KST_KService ) ) {
            KService::Ptr menuItem( KService::Ptr::staticCast( e ) );
            QString entryCaption = menuItem->name();
            if ( !menuItem->genericName().isEmpty() )
                entryCaption = entryCaption + '(' + menuItem->genericName() + ')';
            QAction* act = menu->addAction( KIcon( menuItem->icon() ), entryCaption );
            connect( act, SIGNAL(triggered()), m_signalMapper, SLOT(map()) );
            m_signalMapper->setMapping( act, menuItem->entryPath() );
            ///
        }
        else if ( e->isType( KST_KServiceSeparator ) ) {
//             menu->addSeparator();
            ///
        }
    }
}
