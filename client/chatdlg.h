#ifndef CHATDLG_H
#define CHATDLG_H

#include <QDialog>
#include<QMessageBox>

namespace Ui {
class ChatDlg;
}

class ChatDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDlg(QWidget *parent = 0);
    ~ChatDlg();
signals:
   SIG_LoginCommit(QString name ,QString  code);
   SIG_RegsiterCommit(QString name ,QString  code);
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_register_turn_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_re_return_clicked();

    void on_pushButton_log_clicked();

    void on_pushButton_register_clicked();

private:
    Ui::ChatDlg *ui;




};

#endif // CHATDLG_H
