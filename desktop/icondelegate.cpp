#include "icondelegate.h"

#include <QPainter>
#include <QPixmap>
#include <KDirModel>
#include <KFileItem>
#include <KIcon>

IconDelegate::IconDelegate(QObject* parent) : QAbstractItemDelegate(parent)
{
}

IconDelegate::~IconDelegate()
{
}

QWidget* IconDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return 0;
}

bool IconDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return false;
}

void IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);

    int x = opt.rect.x();
    int y = opt.rect.y();
    int w = opt.rect.width();
    int h = opt.rect.height();

    int iconSize = opt.decorationSize.height();

    KFileItem item = index.data(KDirModel::FileItemRole).value<KFileItem>();

    QIcon::Mode iconMode = opt.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
    QIcon::State iconState = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
    QPixmap icon = KIcon(item.iconName()).pixmap(iconSize, iconMode, iconState);

    painter->drawPixmap(x + ( w - iconSize ) / 2, y, icon);

    painter->setPen(Qt::white);
    painter->drawText(x, y + iconSize, w, h - iconSize,
        Qt::AlignHCenter | Qt::TextWrapAnywhere, item.name()
    );
}

void IconDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
}

void IconDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
}

QSize IconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(100, 100);
}

void IconDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
}
