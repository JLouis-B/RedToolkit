#include "GUI_MainWindow.h"
#include <QApplication>
#include "UIThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load setting.xml
    Settings::loadFromXML(QCoreApplication::applicationDirPath() + "/config.xml");
    UIThemeManager::Init();
    UIThemeManager::SetTheme(Settings::_theme);

    GUI_MainWindow w;
    w.show();
    w.initIrrlicht();

    const QStringList arguments = QCoreApplication::arguments();
    for (int i = 1; i < arguments.size(); ++i)
    {
        w.addFileGeneric(arguments.at(i));
    }

    return a.exec();
}
