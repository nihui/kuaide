#include "sctasks.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>

#include <KLocale>
#include <KIcon>
#include <KAuthorized>

#include <taskmanager/abstractgroupableitem.h>
#include <taskmanager/startup.h>
#include <taskmanager/taskactions.h>
#include <taskmanager/taskitem.h>
#include <taskmanager/groupmanager.h>
#include <taskmanager/taskmanager.h>

#include <KDebug>

SCTasks::SCTasks( QWidget* parent ) : Applet(parent)
{
}

SCTasks::~SCTasks()
{
}

void SCTasks::init()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    m_rootGroupWidget = new TaskGroupWidget(this, this);
    layout->addWidget(m_rootGroupWidget);

    m_groupManager = new TaskManager::GroupManager(this);
    connect(m_groupManager, SIGNAL(reload()), this, SLOT(reload()));

//     m_rootGroupWidget->expand();
    m_rootGroupWidget->setWindowGroup(m_groupManager->rootGroup());
    m_groupManager->setShowOnlyCurrentDesktop(false);
    m_groupManager->setShowOnlyCurrentActivity(false);
    m_groupManager->setShowOnlyCurrentScreen(false);
    m_groupManager->setShowOnlyMinimized(false);
    m_groupManager->setOnlyGroupWhenFull(false);
    m_groupManager->readLauncherConfig();
}

int SCTasks::heightForWidth(int /*width*/) const
{
    return 0;
}

int SCTasks::widthForHeight(int /*height*/) const
{
    return 0;
}

TaskManager::GroupManager* SCTasks::groupManager() const
{
    return m_groupManager;
}

void SCTasks::reload()
{
    TaskManager::TaskGroup* newGroup = m_groupManager->rootGroup();
//     if ( newGroup != m_rootGroupItem->abstractItem() ) {
        m_rootGroupWidget->setWindowGroup( newGroup );
//     }
//     else {
//         m_rootGroupItem->reload();
//     }
}

AbstractTaskWidget::AbstractTaskWidget( QWidget* parent ) : QWidget(parent)
{
}

AbstractTaskWidget::~AbstractTaskWidget()
{
}

void AbstractTaskWidget::mousePressEvent( QMouseEvent* event )
{
}

void AbstractTaskWidget::mouseReleaseEvent( QMouseEvent* event )
{
    activate();
}

WindowTaskWidget::WindowTaskWidget( SCTasks* sctasks, QWidget* parent ) : AbstractTaskWidget(parent)
{
    m_sctasks = sctasks;
}

WindowTaskWidget::~WindowTaskWidget()
{
}

void WindowTaskWidget::setWindowTask( TaskManager::TaskItem* task )
{
    m_task = task;
    connect( m_task, SIGNAL(changed(::TaskManager::TaskChanges)),
             this, SLOT(updateTask(::TaskManager::TaskChanges)) );
    updateTask( ::TaskManager::EverythingChanged );
}

TaskManager::TaskItem* WindowTaskWidget::windowTask() const
{
    return m_task;
}

void WindowTaskWidget::contextMenuEvent( QContextMenuEvent* event )
{
    if ( !KAuthorized::authorizeKAction( "kwin_rmb" ) /*|| m_task.isNull()*/ ) {
        return AbstractTaskWidget::contextMenuEvent( event );
    }

    QList<QAction*> actionList;
    TaskManager::BasicMenu menu( 0, m_task, m_sctasks->groupManager(), actionList );
    menu.exec( event->globalPos() );
}

void WindowTaskWidget::paintEvent(QPaintEvent* event)
{
    int iconSize = height();

    QPainter p(this);
    p.save();
    if (m_task->isMinimized()) {
        p.drawPixmap(0, 0, m_task->icon().pixmap(iconSize, iconSize, QIcon::Disabled));
        QFont f = p.font();
        f.setItalic(true);
        p.setFont(f);
    }
    else {
        p.drawPixmap(0, 0, m_task->icon().pixmap(iconSize, iconSize, QIcon::Active));
        if (m_task->isActive()) {
            QFont f = p.font();
            f.setUnderline(true);
            p.setFont(f);
        }
    }
    p.drawText(height(), 0, width() - iconSize, iconSize, Qt::AlignLeft | Qt::AlignVCenter, m_task->name());
    p.restore();
}

void WindowTaskWidget::activate()
{
    if ( m_task->task() )
        m_task->task()->activateRaiseOrIconify();
}

void WindowTaskWidget::close()
{
    if ( m_task )
        m_task->close();
}

void WindowTaskWidget::gotTaskPointer()
{
}

void WindowTaskWidget::updateTask( ::TaskManager::TaskChanges changes )
{
    update();
}


LauncherWidget::LauncherWidget( SCTasks* sctasks, QWidget* parent ) : AbstractTaskWidget(parent)
{
    m_sctasks = sctasks;
//     setToolButtonStyle( Qt::ToolButtonIconOnly );
}

LauncherWidget::~LauncherWidget()
{
}

void LauncherWidget::setLauncherItem( TaskManager::LauncherItem* launcher )
{
    m_launcher = launcher;
    connect( m_launcher, SIGNAL(changed(::TaskManager::TaskChanges)),
             this, SLOT(updateTask(::TaskManager::TaskChanges)) );
    updateTask( ::TaskManager::EverythingChanged );
}

TaskManager::LauncherItem* LauncherWidget::launcherItem() const
{
    return m_launcher;
}

void LauncherWidget::contextMenuEvent( QContextMenuEvent* event )
{
    if ( !KAuthorized::authorizeKAction( "kwin_rmb" )/* || m_task.isNull()*/ ) {
        return AbstractTaskWidget::contextMenuEvent( event );
    }

    QList<QAction*> actionList;
    TaskManager::BasicMenu menu( 0, m_launcher, m_sctasks->groupManager(), actionList );
    menu.exec( event->globalPos() );
}

void LauncherWidget::paintEvent(QPaintEvent* event)
{
    int iconSize = height();

    QPainter p(this);
    p.drawPixmap(0, 0, m_launcher->icon().pixmap(iconSize, iconSize, QIcon::Active));
}

void LauncherWidget::activate()
{
    m_launcher->launch();
}

void LauncherWidget::close()
{
    m_launcher->close();
}

void LauncherWidget::updateTask( ::TaskManager::TaskChanges changes )
{
//     if ( changes & TaskManager::IconChanged ) {
//         setIcon( m_launcher->icon() );
//     }
    update();
}


TaskGroupWidget::TaskGroupWidget( SCTasks* sctasks, QWidget* parent ) : AbstractTaskWidget(parent)
{
    m_sctasks = sctasks;
//     QHBoxLayout* layout = new QHBoxLayout;
//     layout->setMargin(0);
//     layout->setSpacing(0);
//     setLayout( layout );
    m_windowTaskCount = 0;
    m_launcherCount = 0;
}

TaskGroupWidget::~TaskGroupWidget()
{
}

void TaskGroupWidget::setWindowGroup( TaskManager::GroupPtr group )
{
    m_group = group;
    connect( m_group, SIGNAL(itemAdded(AbstractGroupableItem*)),
             this, SLOT(itemAdded(AbstractGroupableItem*)) );
    connect( m_group, SIGNAL(itemRemoved(AbstractGroupableItem*)),
             this, SLOT(itemRemoved(AbstractGroupableItem*)) );
    connect( m_group, SIGNAL(itemPositionChanged(AbstractGroupableItem*)),
             this, SLOT(itemPositionChanged(AbstractGroupableItem*)) );
    connect( m_group, SIGNAL(changed(::TaskManager::TaskChanges)),
             this, SLOT(updateTask(::TaskManager::TaskChanges)) );
    connect( m_group, SIGNAL(groupEditRequest()),
             this, SLOT(editGroup()) );
}

TaskManager::GroupPtr TaskGroupWidget::windowGroup() const
{
    return m_group;
}

void TaskGroupWidget::activate()
{
}

void TaskGroupWidget::close()
{
}

void TaskGroupWidget::resizeEvent(QResizeEvent* event)
{
    updateLayout();
}

void TaskGroupWidget::itemAdded( AbstractGroupableItem* item )
{
    AbstractTaskWidget* absTaskWidget = m_groupMembers.value( item );
    if ( absTaskWidget ) {
        // already added
        return;
    }

    if ( item->itemType() == TaskManager::GroupItemType ) {
        ;
        //
    }
    else if ( item->itemType() == TaskManager::LauncherItemType ) {
        TaskManager::LauncherItem* launcher = static_cast<TaskManager::LauncherItem*>(item);
        LauncherWidget* launcherWidget = new LauncherWidget( m_sctasks, this );
        launcherWidget->setLauncherItem( launcher );
        m_groupMembers[item] = launcherWidget;
        ++m_launcherCount;
        updateLayout();
        launcherWidget->show();
    }
    else {
        TaskManager::TaskItem* taskItem = static_cast<TaskManager::TaskItem*>(item);
        WindowTaskWidget* taskWidget = new WindowTaskWidget( m_sctasks, this );
        taskWidget->setWindowTask( taskItem );
        m_groupMembers[item] = taskWidget;
        ++m_windowTaskCount;
        updateLayout();
        taskWidget->show();
    }
}

void TaskGroupWidget::itemRemoved( AbstractGroupableItem* item )
{
    AbstractTaskWidget* taskWidget = m_groupMembers.take( item );
    if ( !taskWidget ) {
        // already removed
        return;
    }

    if ( item->itemType() == TaskManager::GroupItemType ) {
        ;
        //
    }
    else if ( item->itemType() == TaskManager::LauncherItemType ) {
        --m_launcherCount;
    }
    else {
        --m_windowTaskCount;
    }

    delete taskWidget;
    updateLayout();
}

void TaskGroupWidget::itemPositionChanged( AbstractGroupableItem* item )
{
}

void TaskGroupWidget::updateTask( ::TaskManager::TaskChanges changes )
{
    bool needsUpdate = false;
    if ( changes & TaskManager::IconChanged ) {
        /// update icon
        needsUpdate = true;
    }
    if ( changes & TaskManager::NameChanged ) {
        /// update text
        needsUpdate = true;
    }
    if ( needsUpdate )
        update();
}

void TaskGroupWidget::editGroup()
{
}

void TaskGroupWidget::updateLayout()
{
    int freeSpace = width() - height() * m_launcherCount;
    int widthForWindowTask = m_windowTaskCount > 0 ? freeSpace / m_windowTaskCount : freeSpace;

    int x = 0;
    QHash<AbstractGroupableItem*, AbstractTaskWidget*>::Iterator it = m_groupMembers.begin();
    QHash<AbstractGroupableItem*, AbstractTaskWidget*>::Iterator end = m_groupMembers.end();
    for (; it != end; ++it) {
        AbstractGroupableItem* item = it.key();
        AbstractTaskWidget* taskWidget = it.value();

        if ( item->itemType() == TaskManager::GroupItemType ) {
        }
        else if ( item->itemType() == TaskManager::LauncherItemType ) {
            LauncherWidget* w = static_cast<LauncherWidget*>(taskWidget);
            w->setGeometry(x, 0, height(), height());
            x += height();
        }
        else {
            WindowTaskWidget* w = static_cast<WindowTaskWidget*>(taskWidget);
            w->setGeometry(x, 0, widthForWindowTask, height());
            x += widthForWindowTask;
        }
    }
}
