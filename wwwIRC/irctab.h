#ifndef IRCTAB_H
#define IRCTAB_H

#include <QWidget>
#include <QTextBrowser>


class IrcTab : public QWidget
{
    Q_OBJECT
public:
    explicit IrcTab(QWidget *parent = 0);
    QTextBrowser* chatBrowser;
signals:

public slots:

};

#endif // IRCTAB_H
