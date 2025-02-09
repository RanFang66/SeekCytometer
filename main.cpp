#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QSplashScreen>
#include <QTranslator>
#include <QScreen>

#include "LoginDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, false);



    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SeekCytometer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QPixmap splashImg(":/resource/images/nezha.jpg");
    QScreen *screen = QApplication::primaryScreen();
    QSize screenSize = screen->availableGeometry().size();
    QSize maxSize(screenSize.width() * 0.8, screenSize.height() * 0.8);
    QPixmap scaledSplash = splashImg.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QSplashScreen splash(scaledSplash);
    splash.show();
    a.processEvents();

    LoginDialog *login = new LoginDialog;
    if (login->exec() == QDialog::Accepted) {
        splash.showMessage(QObject::tr("Loading..."), Qt::AlignCenter, Qt::white);
        MainWindow w;
        w.show();
        splash.finish(&w);
        return a.exec();
    } else {
        return 0;
    }
}
