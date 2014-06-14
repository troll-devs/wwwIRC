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
#include <QLabel>
#include <QWebView>
#include <QUrl>
#include <QRect>
#include <QDesktopWidget>

#define QT_NO_KEYWORDS

class QTcpSocket;
class QTextBrowser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

QTextEdit *te;

bool isLink(QString s, int p, QString& link){
    for(int i=p; i>0 && s[i] != ' '; i--){
        p--;
    }
    //qDebug() << p << s.split(' ');
    int c=0, dot=0;
    for(int i=p; i<s.size() && s[i] != ' '; i++){
        if(s[i] != ':')
            link += s[i];
    }
    for(int i=p; i<s.size() && s[i] != ' '; i++){
        c++;
        if(s[i] == '.'){
            c=0;
            dot = 1;
        }
        if(c>=2 && dot)
            return true;
    }
    return false;
}

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
	QMap<QString, QColor> nicks;
    QStringList historic;
    int hIndex;
    QTextBrowser* chatBrowser;
    bool activePreview = false;
    QLabel* previewer;
    QWebView *webpage;
    int desktopWidth;
    int desktopHeight;


    //void mouseMoveEvent(QMouseEvent *e);
        /*te->append(QString("%1,%2\n").arg(e->pos().x()).arg(e->pos().y()));
        QWidget::mouseMoveEvent(e);
    }*/

public Q_SLOTS:
	void connectToServer();
	void handleServerMessages();
	void sendMessage();
	void pong();


private:
	Ui::MainWindow *ui;
	QTcpSocket* socket;
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
