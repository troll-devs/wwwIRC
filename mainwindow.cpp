#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
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
		//format.setFontWeight( QFont::DemiBold );
		table = cursor.insertTable(1, 2);

		f = false;
	}

	QStringList l = in.split(" ");
	QString answer;

	if(l[0] == "PING"){
		answer = "PONG " + l[1];
		//while (answer.endsWith("\n") || answer.endsWith("\n"))
		answer.chop(2);

		ans = answer;
		//answer += "\n";
		ui->chatBrowser->append("String ends with <" + answer[answer.length()-1] +">");
		//if(answer[answer.length()-1] == QChar("\n"))
			//ui->chatBrowser->append("paragrafo");
		QByteArray out = (answer+"\n").toLocal8Bit();


		socket->write(out.data());
		socket->write(out.data());

		//ui->chatBrowser->append("<" + answer + ">");
	}
	if(l[0].startsWith(":IRC")){
		socket->write("JOIN #exp\n");
		//socket->write("JOIN #exp\n");
	}
	if(l[1] == "PRIVMSG"){

		format.setForeground( QBrush( QColor( "blue" ) ) );
		cursor.setCharFormat( format );

		//QTextCursor cellCursor;
		//cellCursor = table->cellAt(0,0).firstCursorPosition();
		//cellCursor.insertText("OLAASASASASASAasd");

		//table->append(parse[0]);
		//table->append(l[3]);
		QStringList parse = in.split("!");
		table->appendRows(1);
		QTextCursor cellCursor;
		cellCursor.setCharFormat(format);
		cellCursor = table->cellAt(row,0).firstCursorPosition();
		cellCursor.insertText(parse[0]);
		cellCursor = table->cellAt(row++,1).firstCursorPosition();
		cellCursor.insertText(l[3]);

		//cursor.insertText( "Hello world!" );

		//in = parse[0] + l[3];
		//ui->chatBrowser->append(in);
		format.setForeground( QBrush( QColor( "white" ) ) );
		//ui->chatBrowser->append(in);

	}else{
		QTextCursor cellCursor;
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
