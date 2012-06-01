#include "kuaipanel.h"

#include "container.h"

KuaiPanel::KuaiPanel()
{
    m_container = new Container;
    m_container->show();
}

KuaiPanel::~KuaiPanel()
{
    delete m_container;
}
