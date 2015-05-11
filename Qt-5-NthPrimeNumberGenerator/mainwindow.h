#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <templatious/FullPack.hpp>

#include "mainwindow_interface.h"
#include "messeagable.h"

typedef std::shared_ptr<templatious::VirtualPack> MsgPtr;
Q_DECLARE_METATYPE(MsgPtr);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Messageable
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void message(MsgPtr msg);
    virtual void message(templatious::VirtualPack& msg);
private:
    Ui::MainWindow *ui;
    templatious::DynamicVMatchFunctor _dvmf;

    typedef std::unique_ptr< templatious::VirtualMatchFunctor > Handler;

    Handler _handler;

    Handler makeHandler();

private slots:
    void processMessage(MsgPtr msg);
    void on_pushButton_clicked();

signals:
    void vMessage(MsgPtr msg);
};

#endif // MAINWINDOW_H
