#ifndef KBUTTON_H
#define KBUTTON_H

#include <KServiceGroup>

#include "applet.h"

class QMenu;
class QSignalMapper;
class QToolButton;

class KButton : public Applet
{
Q_OBJECT
public:
    explicit KButton(QWidget* parent = 0);
    virtual ~KButton();
    virtual void init();
private Q_SLOTS:
    void toggleMenu();
    void slotExec(const QString& entryPath);
    void slotRunCommand();
    void slotLogout();
private:
    void fillMenu(QMenu* menu, const KServiceGroup::Ptr menuRoot);

    QToolButton* m_button;
    QSignalMapper* m_signalMapper;
    QMenu* m_menu;
};

#endif // KBUTTON_H
