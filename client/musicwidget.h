#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QPushButton>
#include <QListWidgetItem>
#include <QListWidget>
#include <QScrollArea>
#include "playlistform.h"
#include "form.h"
#include"qmytcpclient.h"
#include"cominclude.h"
#include"chatdlg.h"
namespace Ui {
class MusicWidget;
}

class MusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicWidget(QWidget *parent = 0);
    ~MusicWidget();

private slots:

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_mymusic_itemClicked(QListWidgetItem *item);

    void on_pushButton_close_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_min_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_listWidget_mymusic_doubleClicked(const QModelIndex &index);

    void on_pushButton_play_clicked();

    void GetPath();

    void on_pushButton_lyric_clicked();

   void ControlSlider();
   void slot_LoginCommit(QString,QString);
   void slot_RegisterCommit(QString,QString);

   void on_horizontalSlider_sound_valueChanged(int value);
    void slot_DealData(char*,int);
    void on_pushButton_search_clicked();

    void on_pushButton_people_1_clicked();

private:
    //绘制背景函数
    void paintEvent(QPaintEvent* event);
    //鼠标按下
    void mousePressEvent(QMouseEvent* event);
    //鼠标移动
    void mouseMoveEvent(QMouseEvent* event);
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent* event);
    void DealRegisterRs(char* buf,int len);
    void DealLoginRs(char* buf,int len);
signals:
    void DealSound(int);
private:
    Ui::MusicWidget *ui;

    int m_volumnNow;
    int m_volumnLast;
    PlayListForm *playlistform = NULL;          //播放列表模块
    Form  *lyrics = NULL;
    bool m_playflag=true;
    bool m_playlistflag=false;
    QMyTcpClient *m_pTcp;
    QScrollArea *scrollArea = NULL;             //滚动区域
    ChatDlg * m_pLogin;
    QListWidget *my_Create_Music_List = NULL;   //创建的歌单
    QListWidget *my_Collect_Music_List = NULL;  //收藏的歌单
    QString m_ServerIP;
};


#endif // MUSICWIDGET_H
