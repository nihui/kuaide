
#include <kdeversion.h>
#include <KLocale>
#include <KCmdLineArgs>
#include <KDebug>
#include <KAboutData>

#include "kuaipanel.h"

int main(int argc, char** argv)
{
    KAboutData aboutData("kuaipanel", 0, ki18n("KuaiDE Panel"),
                         KDE_VERSION_STRING, ki18n("The KuaiDE panel"),
                         KAboutData::License_GPL, ki18n("(c) 2012, Ni Hui"));
    aboutData.addAuthor(ki18n("Ni Hui"), ki18n("Author"), "shuizhuyuanluo@126.com");

    KCmdLineArgs::init(argc, argv, &aboutData);

    if (!KuaiPanel::start()) {
        kWarning() << "kuaipanel is already running!" << endl;
        return 0;
    }

    KuaiPanel kuaipanel;
    return kuaipanel.exec();
}
