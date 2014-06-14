#include "irctab.h"
#include <stdio.h>
#include <iostream>
#include <QGridLayout>

IrcTab::IrcTab(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout* layoutG = new QGridLayout();
    this->setLayout(layoutG);

    chatBrowser = new QTextBrowser();
    layoutG->addWidget(chatBrowser);
    chatBrowser->append("HELLO");

}
