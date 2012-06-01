#include "kuaidesktop.h"

#include "iconview.h"

KuaiDesktop::KuaiDesktop()
{
    m_iconView = new IconView;
    m_iconView->show();
}

KuaiDesktop::~KuaiDesktop()
{
    delete m_iconView;
}
