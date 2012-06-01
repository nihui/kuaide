#ifndef KUAIDESKTOP_H
#define KUAIDESKTOP_H

#include <KUniqueApplication>

class IconView;

class KuaiDesktop : public KUniqueApplication
{
public:
    explicit KuaiDesktop();
    virtual ~KuaiDesktop();
private:
    IconView* m_iconView;
};

#endif // KUAIDESKTOP_H
