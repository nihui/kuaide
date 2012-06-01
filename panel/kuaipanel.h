#ifndef KUAIPANEL_H
#define KUAIPANEL_H

#include <KUniqueApplication>

class Container;
class KuaiPanel : public KUniqueApplication
{
public:
    explicit KuaiPanel();
    virtual ~KuaiPanel();
private:
    Container* m_container;
};

#endif // KUAIPANEL_H
