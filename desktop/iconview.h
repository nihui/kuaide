#ifndef ICONVIEW_H
#define ICONVIEW_H

#include <QListView>
#include <KActionCollection>

class QItemSelectionModel;
class KDirModel;
class KDirSortFilterProxyModel;
class KNewFileMenu;

class IconView : public QListView
{
    Q_OBJECT
public:
    explicit IconView(QWidget* parent = 0);
    virtual ~IconView();
private Q_SLOTS:
    void init();
protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
private Q_SLOTS:
    void cut();
    void copy();
    void paste();
    void pasteTo();
    void rename();
    void moveToTrash(Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    void deleteSelectedItems();
    void emptyTrashBin();
    void refresh();
    void undoTextChanged(const QString& text);
    void aboutToShowCreateNew();
private:
    KDirModel* m_model;
    QItemSelectionModel* m_selectionModel;
    KDirSortFilterProxyModel* m_proxyModel;

    KActionCollection m_actionCollection;
    KNewFileMenu* m_newMenu;
};

#endif // ICONVIEW_H
