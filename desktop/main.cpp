#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
#include <kdeversion.h>
#include <KLocale>

#include "kuaidesktop.h"

int main(int argc, char** argv)
{
    KAboutData aboutData("kuaidesktop", 0, ki18n("KuaiDE Desktop"),
                         KDE_VERSION_STRING, ki18n("The KuaiDE desktop"),
                         KAboutData::License_GPL, ki18n("(c) 2012, Ni Hui"));
    aboutData.addAuthor(ki18n("Ni Hui"), ki18n("Author"), "shuizhuyuanluo@126.com");

    KCmdLineArgs::init(argc, argv, &aboutData);

    if (!KuaiDesktop::start()) {
        kWarning() << "kuaidesktop is already running!" << endl;
        return 0;
    }

    KuaiDesktop kuaidesktop;
    return kuaidesktop.exec();
}
