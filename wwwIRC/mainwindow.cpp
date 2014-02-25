#include "mainwindow.h"
#include "ui_mainwindow.h"

QString server = "irc.mibbit.com";
QString channel = "#exp";
QString serverNick = "michelBotQT";

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	qsrand(QTime::currentTime().msec());
}

MainWindow::~MainWindow()
{
	delete ui;

}

void MainWindow::handleServerMessages(){

	QScrollBar *sb = ui->chatBrowser->verticalScrollBar();
	bool maxScroll = false;
	int oldScroll = sb->value();
	if(sb->value() == sb->maximum())
		maxScroll = true;


	QString in = socket->read(1024);
	if(first){
		QString w = ("NICK " + serverNick + "\nUSER michelQt 8 * :BotQT");
		socket->write(w.toLocal8Bit());
		first = false;

		cursor = ui->chatBrowser->textCursor();
		table = cursor.insertTable(1, 2);

		f = false;
	}

	QStringList l = in.split(" ");
	QString answer;

	if(l[0] == "PING"){
		answer = "PONG " + l[1];
		answer.chop(2);

		ans = answer;

		ui->chatBrowser->append("String ends with <" + answer[answer.length()-1] +">");
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
		QString message = l[3].right(l[3].size()-1);

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
	ui->chatBrowser->append("connect");

	connect(socket, SIGNAL(readyRead()), this, SLOT(handleServerMessages()) );
	nicks[serverNick] = QColor("red");

}

void MainWindow::outputMessage(QString nick, QString message){
	QTextCharFormat formatNick;
	formatNick.setForeground(nicks[nick]);
	printf(nick.toLocal8Bit());
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
	QScrollBar *sb = ui->chatBrowser->verticalScrollBar();
	bool maxScroll = false;
	int oldScroll = sb->value();
	if(sb->value() == sb->maximum())
		maxScroll = true;


	QByteArray out;
	QString m;

	if(ui->message->text().startsWith("/")){
		QString commandS = ui->message->text();
		commandS = commandS.mid(1);
		QStringList words = commandS.split(" ");
		words[0] = words[0].toUpper();
		QString command = words.join(" ");
		m = command + "\n";

	}else{
		QString message = "PRIVMSG " + channel + " :" + ui->message->text() + "\n";
		//printf(message.toLocal8Bit());
		m = message;

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
	ui->chatBrowser->append(ans);
	QByteArray out = (ans + "\n").toLocal8Bit();
	socket->write(out);
}
