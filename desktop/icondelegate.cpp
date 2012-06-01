#include "icondelegate.h"

#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QTextEdit>
#include <KDirModel>
#include <KFileItem>
#include <KIcon>
#include <KIconEffect>
#include <KIconLoader>

// forward declaration for kio imagefilter private header
namespace KIO {
class KIO_EXPORT ImageFilter
{
public:
    // Blurs the alpha channel of the image and recolors it to the specified color.
    // The image must have transparent padding on all sides, or the shadow will be clipped.
    static void shadowBlur(QImage &image, float radius, const QColor &color);
};
}

IconDelegate::IconDelegate(QObject* parent) : QAbstractItemDelegate(parent)
{
}

IconDelegate::~IconDelegate()
{
}

QWidget* IconDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);

    QTextEdit* editor = new QTextEdit(parent);


    QPalette p = editor->palette();
    p.setBrush(QPalette::Base, Qt::white);
    editor->setPalette(p);


    editor->setAcceptRichText(false);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setAlignment(opt.displayAlignment);
    return editor;
}

bool IconDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return false;
}

bool IconDelegate::eventFilter(QObject* object, QEvent* event)
{
    QTextEdit* editor = qobject_cast<QTextEdit*>(object);
    if (!editor)
        return false;

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                emit commitData(editor);
                emit closeEditor(editor, NoHint);
                return true;
            case Qt::Key_Enter:
            case Qt::Key_Return: {
                QString text = editor->toPlainText();
                if (text.isEmpty() || text == "." || text == "..")
                    return true;// eat new line
                emit commitData(editor);
                emit closeEditor(editor, SubmitModelCache);
                return true;
            }
            case Qt::Key_Escape:
                emit closeEditor(editor, RevertModelCache);
                return true;
            default:
                return false;
        }
    }

    if (event->type() == QEvent::FocusOut) {
        const QWidget* w = QApplication::activePopupWidget();
        if (!w || w->parent() != editor) {
            emit commitData(editor);
            emit closeEditor(editor, NoHint);
            return true;
        }
    }

    return false;
}

void IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);
    opt.showDecorationSelected = true;

    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();

    KFileItem item = index.data(KDirModel::FileItemRole).value<KFileItem>();

    int x = opt.rect.x();
    int y = opt.rect.y();
    int w = opt.rect.width();
    int h = opt.rect.height();

//     int iconSize = opt.decorationSize.height();
    int iconSize = 48;

    /// draw panel item
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

//BEGIN draw icon
    QIcon::Mode iconMode = opt.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
    QIcon::State iconState = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
    QPixmap icon = KIcon(item.iconName()).pixmap(iconSize, iconMode, iconState);

    if (opt.state & QStyle::State_MouseOver) {
        /// highlight mouse hover icon
        KIconEffect* effect = KIconLoader::global()->iconEffect();
        icon = effect->apply(icon, KIconLoader::Desktop, KIconLoader::ActiveState);
    }

    painter->drawPixmap(x + ( w - iconSize ) / 2, y, icon);
//END draw icon

//BEGIN draw name
    QPixmap pixmap(w, h - iconSize);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.setPen(Qt::white);
    p.drawText(0, 0, w, h - iconSize,
        Qt::AlignHCenter | Qt::TextWrapAnywhere, item.name()
    );

    QImage image = pixmap.toImage();
    KIO::ImageFilter::shadowBlur(image, 3, Qt::black);

    painter->drawImage(x, y + iconSize, image);
    painter->drawPixmap(x, y + iconSize, pixmap);
//END draw name

    if (opt.state & QStyle::State_HasFocus) {
        /// draw focus rect
        style->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, painter, opt.widget);
    }
}

void IconDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QTextEdit* w = qobject_cast<QTextEdit*>(editor);
    QString name = index.data(Qt::EditRole).toString();
    w->insertPlainText(name);
    w->selectAll();
}

void IconDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QTextEdit* w = qobject_cast<QTextEdit*>(editor);
    model->setData(index, w->toPlainText(), Qt::EditRole);
}

QSize IconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(100, 100);
}

void IconDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt(option);
    QRect r = opt.rect;
    r.setTop(r.top() + 48);
    editor->setGeometry(r);
}
