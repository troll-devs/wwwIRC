#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	/*const char name[] = "Qt sample notification app";
	NotifyNotification *notification;
	QApplication app(argc, argv);
	QMainWindow win;

	notify_init(name);


	notification = notify_notification_new(name, "Just want you to know...", NULL);
	if (notification) {
		notify_notification_set_timeout(notification, 3000);
		g_object_unref(notification);
	}*/

	return a.exec();
}
