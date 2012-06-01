#ifndef SCTASKS_H
#define SCTASKS_H

#include <QToolButton>
#include <QLayout>

#include <taskmanager/task.h>
#include <taskmanager/groupmanager.h>
#include <taskmanager/abstractgroupableitem.h>

#include "applet.h"

using TaskManager::AbstractGroupableItem;

class QContextMenuEvent;
class AbstractTaskWidget;
class TaskGroupWidget;

class SCTasks : public Applet
{
Q_OBJECT
public:
    explicit SCTasks(QWidget* parent = 0);
    virtual ~SCTasks();
    virtual void init();
    virtual int heightForWidth(int width) const;
    virtual int widthForHeight(int height) const;
    TaskManager::GroupManager* groupManager() const;
public Q_SLOTS:
    void reload();
private:
    TaskManager::GroupManager* m_groupManager;
    TaskGroupWidget* m_rootGroupWidget;
};

class AbstractTaskWidget : public QWidget
{
public:
    explicit AbstractTaskWidget( QWidget* parent );
    virtual ~AbstractTaskWidget();
    virtual void activate() = 0;
    virtual void close() = 0;
protected:
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
};

class WindowTaskWidget : public AbstractTaskWidget
{
Q_OBJECT
public:
    explicit WindowTaskWidget( SCTasks* sctasks, QWidget* parent );
    virtual ~WindowTaskWidget();
    void setWindowTask( TaskManager::TaskItem* task );
    TaskManager::TaskItem* windowTask() const;
protected:
    virtual void contextMenuEvent( QContextMenuEvent* event );
    virtual void paintEvent(QPaintEvent* event);
    virtual void activate();
    virtual void close();
private Q_SLOTS:
    void gotTaskPointer();
    void updateTask( ::TaskManager::TaskChanges changes );
private:
    SCTasks* m_sctasks;
    TaskManager::TaskItem* m_task;
};

class LauncherWidget : public AbstractTaskWidget
{
Q_OBJECT
public:
    explicit LauncherWidget( SCTasks* sctasks, QWidget* parent );
    virtual ~LauncherWidget();
    void setLauncherItem( TaskManager::LauncherItem* launcher );
    TaskManager::LauncherItem* launcherItem() const;
protected:
    virtual void contextMenuEvent( QContextMenuEvent* event );
    virtual void paintEvent(QPaintEvent* event);
    virtual void activate();
    virtual void close();
private Q_SLOTS:
    void updateTask( ::TaskManager::TaskChanges changes );
private:
    SCTasks* m_sctasks;
    TaskManager::LauncherItem* m_launcher;
};

class TaskGroupWidget : public AbstractTaskWidget
{
Q_OBJECT
public:
    explicit TaskGroupWidget( SCTasks* sctasks, QWidget* parent );
    virtual ~TaskGroupWidget();
    void setWindowGroup( TaskManager::GroupPtr group );
    TaskManager::GroupPtr windowGroup() const;
protected:
    virtual void activate();
    virtual void close();
    virtual void resizeEvent(QResizeEvent* event);
private Q_SLOTS:
    void itemAdded( AbstractGroupableItem* item );
    void itemRemoved( AbstractGroupableItem* item );
    void itemPositionChanged( AbstractGroupableItem* item );
    void updateTask( ::TaskManager::TaskChanges changes );
    void editGroup();
private:
    void updateLayout();
private:
    SCTasks* m_sctasks;
    TaskManager::GroupPtr m_group;
    QHash<AbstractGroupableItem*, AbstractTaskWidget*> m_groupMembers;
    int m_windowTaskCount;
    int m_launcherCount;
};

#endif // SCTASKS_H
