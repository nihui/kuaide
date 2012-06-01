#ifndef ICONDELEGATE_H
#define ICONDELEGATE_H

#include <QAbstractItemDelegate>

class IconDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit IconDelegate(QObject* parent = 0);
    virtual ~IconDelegate();
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;


};

#endif // ICONDELEGATE_H
