#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QTextTable>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QScrollBar>
#include <QMap>
#include <QTime>
#include <QDebug>
#include <QKeyEvent>

#define QT_NO_KEYWORDS

class QTcpSocket;
class QTextBrowser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void outputMessage(QString, QString);
    bool eventFilter(QObject *, QEvent *);

	bool first = true;
	QString ans;
	QTextTable *table;
	int row =0;
	QTextCharFormat format;
	QTextCursor cursor;
	bool f = true;
	QMap<QString, QColor> nicks;
    QStringList historic;
    int hIndex;

public Q_SLOTS:
	void connectToServer();
	void handleServerMessages();
	void sendMessage();
	void pong();



private:
	Ui::MainWindow *ui;
	QTcpSocket* socket;
};

#endif // MAINWINDOW_H
