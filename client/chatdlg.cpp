#include "chatdlg.h"
#include "ui_chatdlg.h"

ChatDlg::ChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDlg)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->lineEdit_log_name->setText("");
    ui->lineEdit_log_pw->setText("");
}

ChatDlg::~ChatDlg()
{
    delete ui;
}


void ChatDlg::on_pushButton_4_clicked()
{
    this->close();
}

void ChatDlg::on_pushButton_3_clicked()
{
    this->showMinimized();
}

void ChatDlg::on_pushButton_2_clicked()
{
    ui->stackedWidget_Main->setCurrentIndex(1);

}

void ChatDlg::on_pushButton_register_turn_clicked()
{
    ui->stackedWidget_Register_Log->setCurrentIndex(1);

}

void ChatDlg::on_pushButton_5_clicked()
{
     ui->stackedWidget_Main->setCurrentIndex(0);
}

void ChatDlg::on_pushButton_re_return_clicked()
{
     ui->stackedWidget_Register_Log->setCurrentIndex(0);
}

void ChatDlg::on_pushButton_log_clicked()
{
    QString name = ui->lineEdit_log_name->text();
    QString code = ui->lineEdit_log_pw->text();
    if( name.isEmpty() || code.isEmpty() )
    {
        QMessageBox::warning(this,"提示","账号或密码为空");
        return;
    }
    if( name.length() > 20 || code.length() >10)
    {
        QMessageBox::warning(this,"提示","账号或密码过长");
        return;
    }
    emit SIG_LoginCommit( name ,  code);
}

void ChatDlg::on_pushButton_register_clicked()
{
    QString name = ui->lineEdit_register_name->text();
    QString code = ui->lineEdit_register_pw->text();
    QString rcode = ui->lineEdit_register_Rpw->text();
    if( name.isEmpty() || code.isEmpty() )
    {
        QMessageBox::warning(this,"提示","账号或密码为空");
        return;
    }
    if( name.length() > 20 || code.length() >10 || QString::compare(rcode,code)!=0)
    {
        QMessageBox::warning(this,"提示","账号或密码过长或密码不匹配");
        return;
    }
    emit SIG_RegsiterCommit( name ,  code);
}
