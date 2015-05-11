#include "mainwindow.h"
#include "ui_mainwindow.h"

TEMPLATIOUS_TRIPLET_STD;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _handler(makeHandler())
{
    ui->setupUi(this);
    connect(this,SIGNAL(vMessage(MsgPtr)),
        this,SLOT(processMessage(MsgPtr)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processMessage(MsgPtr msg) {
    _handler->tryMatch(*msg);
}

auto MainWindow::makeHandler() -> Handler {
    typedef MainWindowInterface Msg;
    return SF::virtualMatchFunctorPtr(
        SF::virtualMatch< Msg::InAttachCallback, Handler >(
            [=](Msg::InAttachCallback,Handler& h) {
                this->_dvmf.attach(std::move(h));
            }
        ),
        SF::virtualMatch< Msg::InSetButtonEnabled, bool >(
            [=](Msg::InSetButtonEnabled,bool value) {
                this->ui->pushButton->setEnabled(value);
            }
        ),
        SF::virtualMatch< Msg::InSetStatusText, const std::string >(
            [=](Msg::InSetStatusText,const std::string& str) {
                this->ui->label->setText(str.c_str());
            }
        ),
        SF::virtualMatch< Msg::InSetProgress, const int >(
            [=](Msg::InSetProgress,int prog) {
                this->ui->progressBar->setValue(prog);
            }
        ),
        SF::virtualMatch< Msg::InShow >(
            [=](Msg::InShow) {
                this->show();
            }
        ),
        SF::virtualMatch< Msg::QueryLabelText, std::string >(
            [=](Msg::QueryLabelText, std::string& s) {
                QString out = this->ui->lineEdit->text();
                s = out.toStdString();
            }
        )
    );
}

void MainWindow::message(MsgPtr msg) {
    emit vMessage(msg);
}

void MainWindow::message(templatious::VirtualPack& msg) {
    _handler->tryMatch(msg);
}

void MainWindow::on_pushButton_clicked()
{
    typedef MainWindowInterface Msg;
    auto msg = SF::vpack< Msg::OutButtonClicked >(Msg::OutButtonClicked());
    _dvmf.tryMatch(msg);
}
