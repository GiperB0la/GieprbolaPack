#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QIcon>

#include <iostream>

#include "UI/FileSystemModel/include/FileSystemModel.hpp"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
#ifdef Q_OS_WIN
    // qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");

    AllocConsole();

    FILE* fp;

    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::cout << "Console initialized\n";
#endif

    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/qt/qml/gieprbolapack/Resources/Icon.png"));

    // QQuickStyle::setStyle("Material");
    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "FileSystemModel",
        &FileSystemModel::instance()
    );

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/gieprbolapack/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        system("pause");
        return -1;
    }

    return app.exec();
}