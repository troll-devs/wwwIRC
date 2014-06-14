#include "mainwindow.h"
#include "ui_mainwindow.h"

QString server = "irc.mibbit.com";
QString channel = "#derp";
QString serverNick = "michelBotQT";

QMainWindow* f;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QTime::currentTime().msec());

    f = new QMainWindow();
    f->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    f->setGeometry(100, 100, 400, 200);
    f->setCentralWidget(new QWidget);
    QVBoxLayout *layoutFrame = new QVBoxLayout;
    f->centralWidget()->setLayout(layoutFrame);

    previewer = new QLabel("asasdasaas");

    f->show();
    ui->message->installEventFilter(this);

    chatBrowser = ((IrcTab* )ui->tabWidget->currentWidget())->chatBrowser;

    chatBrowser->viewport()->setMouseTracking(true);
    chatBrowser->viewport()->installEventFilter(this);

    webpage = new QWebView(f->centralWidget());
    webpage->setGeometry(0, 0, 400, 300);
    webpage->show();

    QRect screenGeometry = QDesktopWidget().availableGeometry();
    desktopWidth = screenGeometry.width();
    desktopHeight = screenGeometry.height();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::handleServerMessages(){

    QScrollBar *sb = chatBrowser->verticalScrollBar();
	bool maxScroll = false;
	int oldScroll = sb->value();
	if(sb->value() == sb->maximum())
		maxScroll = true;


	QString in = socket->read(1024);
	if(first){
		QString w = ("NICK " + serverNick + "\nUSER michelQt 8 * :BotQT");
		socket->write(w.toLocal8Bit());
		first = false;

        cursor = chatBrowser->textCursor();
		table = cursor.insertTable(1, 2);

	}

	QStringList l = in.split(" ");
	QString answer;

	if(l[0] == "PING"){
		answer = "PONG " + l[1];
		answer.chop(2);

		ans = answer;

		QByteArray out = (answer+"\n").toLocal8Bit();

		socket->write(out.data());
		socket->write(out.data());

	}
	if(l[0].startsWith(":IRC")){
		QString w = "JOIN " + channel + "\n";
		socket->write(w.toLocal8Bit());

	}
	if(l[0].startsWith(":") && l[1] == "PRIVMSG"){

		QStringList parse = in.right(in.size()-1).split("!");
		QString nick = parse[0];
		if(!nicks.contains(nick)){
			nicks[nick] = QColor( (qrand()%2) * (100 + qrand() % 125), (qrand()%2) * (qrand() % 125 + 100), (qrand()%2) * (qrand() % 125 + 100) );
		}
        l[3] = l[3].right(l[3].size()-1);
        l.removeAt(0);
        l.removeAt(0);
        l.removeAt(0);
        QString message = l.join(" ");
		outputMessage(nick, message);
	}else{
		QTextCursor cellCursor;

		format.setForeground( QBrush( QColor( "blue" ) ) );
        cellCursor.setCharFormat( format );

		table->appendRows(1);
		table->mergeCells(row, 0, 1, 2);
		cellCursor = table->cellAt(row++,0).firstCursorPosition();
		cellCursor.insertText(in);

	}

	if(maxScroll)
		sb->setValue(sb->maximum());
	else
		sb->setValue(oldScroll);

}

void MainWindow::connectToServer(){
	first = true;
	socket = new QTcpSocket(this);
	socket->connectToHost(server, 6667);
    chatBrowser->append("connect");

	connect(socket, SIGNAL(readyRead()), this, SLOT(handleServerMessages()) );
	nicks[serverNick] = QColor("red");

}

void MainWindow::outputMessage(QString nick, QString message){

	QTextCharFormat formatNick;
	formatNick.setForeground(nicks[nick]);
	formatNick.setFontWeight(80);

	table->appendRows(1);
	QTextCursor messageCursor = table->cellAt(row,0).firstCursorPosition();
	messageCursor.setCharFormat(formatNick);
	messageCursor.insertText(nick);

	QTextCharFormat formatMessage;
	messageCursor = table->cellAt(row++,1).firstCursorPosition();
	messageCursor.setCharFormat(formatMessage);
	messageCursor.insertText(message);
}

void MainWindow::sendMessage(){
    QScrollBar *sb = chatBrowser->verticalScrollBar();
	bool maxScroll = false;
	int oldScroll = sb->value();
	if(sb->value() == sb->maximum())
		maxScroll = true;

	QByteArray out;
	QString m;
    hIndex = historic.size()-1;
	if(ui->message->text().startsWith("/")){
		QString commandS = ui->message->text();
		commandS = commandS.mid(1);
		QStringList words = commandS.split(" ");
		words[0] = words[0].toUpper();
		QString command = words.join(" ");
		m = command + "\n";

	}else{
		QString message = "PRIVMSG " + channel + " :" + ui->message->text() + "\n";
		m = message;
        historic.append(ui->message->text());
        hIndex = historic.size();
		outputMessage(serverNick, ui->message->text());
	}

	out = m.toLocal8Bit();
	socket->write(out);

	ui->message->setText("");

	if(maxScroll)
		sb->setValue(sb->maximum());
	else
		sb->setValue(oldScroll);
}

void MainWindow::pong(){
    chatBrowser->append(ans);
	QByteArray out = (ans + "\n").toLocal8Bit();
	socket->write(out);
}

bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == ui->message)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = (QKeyEvent*)event;
            keyEvent->key();
            if (keyEvent->key() == Qt::Key_Up)
            {
                hIndex--;
                if(hIndex < 0)
                    hIndex = 0;
                 if(historic.size() > 0){
                     ui->message->setText(historic.at(hIndex));

                 }
                 return true;
            }
            else if(keyEvent->key() == Qt::Key_Down)
            {
                hIndex++;
                if(hIndex >= historic.size()){
                    hIndex = historic.size();
                    ui->message->setText("");
                }else{
                    ui->message->setText(historic.at(hIndex));
                }
                return true;
            }
        }
        return false;
    }
    if (event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        int key = keyEvent->key();

        ui->message->text().append((QChar)key);

    }
    if(obj == chatBrowser->viewport() /*&& (event->type()==QEvent::Enter || event->type()==QEvent::Leave || event->type()==QEvent::Move )*/ ){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->type() == QMouseEvent::Enter){
            activePreview = true;
        }else if(mouseEvent->type() == QMouseEvent::Leave){
            activePreview = false;
            if(!f->isHidden())
                f->hide();
        }
        if(mouseEvent->type() == QMouseEvent::Move || mouseEvent->type() == QMouseEvent::MouseMove){
            int x, y, gx, gy;
            x = mouseEvent->pos().x() + 3;
            y = mouseEvent->pos().y() + 3;
            gx = mouseEvent->globalPos().x() + 8;
            gy = mouseEvent->globalPos().y() + 6;
            gx = gx + 400 < desktopWidth ? gx : desktopWidth - 400;
            gy = gy + 300 < desktopHeight ? gy : desktopHeight - 300;
            f->setGeometry(gx, gy, 400, 300);
            QTextCursor cursor = chatBrowser->cursorForPosition(QPoint(x, y));

            int p = cursor.positionInBlock();
            cursor.select(QTextCursor::LineUnderCursor);

            QString s = cursor.selectedText();

            //qDebug() << p << " " << s;
            QString link;
            if(isLink(s, p, link)){
                if(f->isHidden()){
                    link = "http://" + link;
                    qDebug() << link;
                    f->show();

                    webpage->load(QUrl(link));

                    //webpage->show();
                    //webpage->load(url);
                    //f->show();

                }
                //previewer->setText(s);

            }else{
                if(!f->isHidden())
                    f->hide();
            }

        }
    }

    return QMainWindow::eventFilter(obj, event);

}

void MainWindow::closeEvent(QCloseEvent* e){
    f->close();
}
