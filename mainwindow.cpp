#include "mainwindow.h"
#include "ui_mainwindow.h"


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
		socket->write("NICK michelBotQt\nUSER michelQt 8 * :BotQT");
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
		socket->write("JOIN #ancelb\n");

	}
	if(l[0].startsWith(":") && l[1] == "PRIVMSG"){

		format.setForeground( QBrush( QColor( "blue" ) ) );

		QStringList parse = in.right(in.size()-1).split("!");
		QString nick = parse[0];
		if(!nicks.contains(nick)){
			nicks[nick] = QColor(qrand() % 125 + 100, qrand() % 125 + 100, qrand() % 125 + 100);
		}
		QString message = l[3].right(l[3].size()-1);
		table->appendRows(1);

		QTextCharFormat format;

		QTextCursor nickCursor = table->cellAt(row,0).firstCursorPosition();
		format.setForeground(QBrush(nicks[nick]));
		nickCursor.setCharFormat(format);
		nickCursor.insertText(nick);


		QTextCursor messageCursor = table->cellAt(row++,1).firstCursorPosition();
		messageCursor.insertText(message);

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
	socket->connectToHost("irc.mibbit.net", 6667);
	ui->chatBrowser->append("connect");

	connect(socket, SIGNAL(readyRead()), this, SLOT(handleServerMessages()) );


}

void MainWindow::sendMessage(){
	QByteArray out = (ui->message->text() + "\n").toLocal8Bit();
	socket->write(out);
	ui->chatBrowser->append(ui->message->text());
	if(socket->isOpen())
		ui->chatBrowser->append("open");
	ui->message->setText("");
}

void MainWindow::pong(){
	ui->chatBrowser->append(ans);
	QByteArray out = (ans + "\n").toLocal8Bit();
	socket->write(out);
}
