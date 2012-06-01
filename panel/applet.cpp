#include "applet.h"

Applet::Applet(QWidget* parent) : QWidget(parent)
{
}

Applet::~Applet()
{
}

int Applet::heightForWidth(int width) const
{
    return width;
}

int Applet::widthForHeight(int height) const
{
    return height;
}
