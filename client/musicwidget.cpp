#include "musicwidget.h"
#include "ui_musicwidget.h"
#include "ui_playlistform.h"
#include "ui_form.h"
#include <QCryptographicHash>
#include<iostream>
#define MD5_KEY (1234)
#define MULTICAST   ("233.233.233.233")
#define VIDEOPORT   (1234)
#define MULTIPORT   (12301)
#define MESSPORT   (45454)
//获取MD5编码
static QByteArray  GetMD5(QString str)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QString tmp ;
    tmp = QString("%1_%2").arg(str).arg(MD5_KEY);
    hash.addData(tmp.toStdString().c_str());

    // 添加数据到加密哈希值
    QByteArray result = hash.result();  // 返回最终的哈希值
    return result.toHex();
}
MusicWidget::MusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicWidget),m_volumnNow(30),m_volumnLast(30)
{
    ui->setupUi(this);

    setWindowTitle("Cloud Music");
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->horizontalSlider_sound->setValue(30);

        //播放列表模块
        playlistform = new PlayListForm(this);
        playlistform->setGeometry(241,63,1285,545);
        playlistform->hide();
        //桌面歌词模块
        lyrics= new Form(this);
        lyrics->setGeometry(241,63,1285,545);
        lyrics->hide();
    //推荐列表 列表样式
       ui->listWidget->setStyleSheet("QListWidget{color:rgb(155,155,155); background:rgb(255,255,255);border:0px solid gray;}"
                                     "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                     "QListWidget::Item:hover{color:rgb(255,255,255);background:transparent;border:0px solid gray;}"
                                     "QListWidget::Item:selected{border-image:url(images/listwidget_h.png); color:rgb(255,255,255);border:0px solid gray;}"
                                     "QListWidget::Item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}"
                                     );

       QListWidgetItem *add_item_1 = new QListWidgetItem(ui->listWidget);
       add_item_1->setIcon(QIcon(":/images/music.png"));
       add_item_1->setText("发现音乐");


       QListWidgetItem *add_item_2 = new QListWidgetItem(ui->listWidget);
       add_item_2->setIcon(QIcon(":/images/fm.png"));
       add_item_2->setText("私人FM");

       QListWidgetItem *add_item_3 = new QListWidgetItem(ui->listWidget);
       add_item_3->setIcon(QIcon(":/images/video.png"));
       add_item_3->setText("MV");

       QListWidgetItem *add_item_4 = new QListWidgetItem(ui->listWidget);
       add_item_4->setIcon(QIcon(":/images/friend.png"));
       add_item_4->setText("朋友");
       //我的音乐列表
       ui->listWidget_mymusic->setStyleSheet("QListWidget{color:rgb(155,155,155); background:rgb(255,255,255);border:0px solid gray;}"
                                             "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                             "QListWidget::Item:hover{color:rgb(255,255,255);background:transparent;border:0px solid gray;}"
                                             "QListWidget::Item:selected{border-image:url(images/listwidget_h.png); color:rgb(255,255,255);border:0px solid gray;}"
                                             "QListWidget::Item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}");

       QListWidgetItem *add_item_5 = new QListWidgetItem(ui->listWidget_mymusic);
       add_item_5->setIcon(QIcon(":/images/music_1.png"));
       add_item_5->setText("本地音乐");

       QListWidgetItem *add_item_6 = new QListWidgetItem(ui->listWidget_mymusic);
       add_item_6->setIcon(QIcon(":/images/download.png"));
       add_item_6->setText("下载管理");

       QListWidgetItem *add_item_7 = new QListWidgetItem(ui->listWidget_mymusic);
       add_item_7->setIcon(QIcon(":/images/yun.png"));
       add_item_7->setText("我的音乐云盘");

       QListWidgetItem *add_item_8 = new QListWidgetItem(ui->listWidget_mymusic);
       add_item_8->setIcon(QIcon(":/images/diantai.png"));
       add_item_8->setText("我的电台");

       QListWidgetItem *add_item_9 = new QListWidgetItem(ui->listWidget_mymusic);
       add_item_9->setIcon(QIcon(":/images/shoucang.png"));
       add_item_9->setText("我的搜藏");
       //bind
       QObject::connect(playlistform, SIGNAL(SendPicturePath()), this, SLOT(GetPath()));
       QObject::connect(playlistform, SIGNAL(SendCurrentName(QString)), lyrics, SLOT(slot_setWindowBackGroundImage(QString)));
       QObject::connect(playlistform, SIGNAL(SendTime()), this, SLOT(ControlSlider()));
       QObject::connect(this, SIGNAL(DealSound(int)), playlistform, SLOT(SetSound(int)));

//        m_ServerIP="192.168.10.23";
//       m_pTcp = new QMyTcpClient;
//       if( !m_pTcp->InitNetWork( /*DEF_TCP_SERVER*/(char*)m_ServerIP.toStdString().c_str(), DEF_TCP_PORT ))
//       {
//                QMessageBox::warning(this,"提示","服务器异常");
//                exit(-1);
//       }
//        connect( m_pTcp ,SIGNAL(SIG_ReadyData(char*,int)) , this,SLOT(slot_DealData(char*,int)));

        m_pLogin  = new ChatDlg;

        connect( m_pLogin ,SIGNAL( SIG_LoginCommit(QString,QString)) , this , SLOT( slot_LoginCommit(QString,QString)));
        connect( m_pLogin ,SIGNAL( SIG_RegsiterCommit(QString,QString)) , this , SLOT( slot_RegisterCommit(QString,QString)));
}

MusicWidget::~MusicWidget()
{
    delete ui;
}
/*
 * 绘制函数
 * 绘制程序
 */

void MusicWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter_mainback(this);
    painter_mainback.drawPixmap(0,0,this->width(),this->height(),QPixmap(""));
    ui->pushButton_people_1->setMask(QRegion(0,0,25,25,QRegion::Ellipse));              //设置遮罩

}
/*
 * 鼠标按下操作
 * 记录当前坐标
 */
static QPoint last(0,0);        //保存坐标
const int TITLE_HEIGHT = 50;    //这里也可以使用宏定义，保存标题高度，也就是鼠标点击区域的高度
void MusicWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->y()<TITLE_HEIGHT)
    {
        last = event->globalPos();
    }
}
/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void MusicWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y()<TITLE_HEIGHT)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        this->move(this->x()+dx,this->y()+dy);
    }
}
/*
 * 鼠标释放函数
 */
void MusicWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->y()<TITLE_HEIGHT)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        this->move(this->x()+dx,this->y()+dy);
    }
}


void MusicWidget::on_pushButton_close_clicked()
{
    this->close();
}

void MusicWidget::on_pushButton_max_clicked()
{
   // this->showMaximized();
}

void MusicWidget::on_pushButton_min_clicked()
{
    this->showMinimized();
}

/*
 * 推荐列表
 */
void MusicWidget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_mymusic->clearSelection();

}
/*
 * 我的音乐列表
 */
void MusicWidget::on_listWidget_mymusic_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_mymusic->clearSelection();

}

void MusicWidget::on_listWidget_doubleClicked(const QModelIndex &index)
{

}

void MusicWidget::on_listWidget_mymusic_doubleClicked(const QModelIndex &index)
{

    if(playlistform)
    {
        if(playlistform->isHidden())
        {
            playlistform->show();
             m_playlistflag = true;
        }
        else
        {
            playlistform->hide();
             m_playlistflag = false;
        }
    }

}
void MusicWidget::GetPath()
{
    ui->pushButton_play->setIcon(QIcon(":/images/btn_play2.png"));

}

void MusicWidget::on_pushButton_play_clicked()
{

    if(m_playlistflag)
    {
        if(m_playflag)
        {
             playlistform->myplayer->pause();
             ui->pushButton_play->setIcon(QIcon(":/images/icon_play.png"));
             m_playflag = false;
        }else
        {
            playlistform->myplayer->play();
            ui->pushButton_play->setIcon(QIcon(":/images/btn_play2.png"));
            m_playflag = true;
        }
    }


}


void MusicWidget::on_pushButton_lyric_clicked()
{

    if(lyrics)
    {
        if(lyrics->isHidden())
        {
            lyrics->show();
        }
        else
        {
            lyrics->hide();
        }
    }
}

void MusicWidget::ControlSlider()
{
     QTime duration(0, playlistform->myplayer->position()/60000, qRound( playlistform->myplayer->position()%60000/1000.0),0);
     QString time = duration.toString("mm:ss");
     QTime duration2(0, playlistform->myplayer->duration()/60000, qRound( playlistform->myplayer->duration()%60000/1000.0),0);
     ui->horizontalSlider_music->setValue(  playlistform->myplayer->position()*100 /  playlistform->myplayer->duration());
     ui->label_5->setText(time);
     ui->label_6->setText(duration2.toString("mm:ss"));
}

void MusicWidget::on_horizontalSlider_sound_valueChanged(int value)
{
    std::cout<<value<<endl;
    emit DealSound(value);

}
void MusicWidget::slot_DealData(char* buf,int len)
{
    int type = *(int *) buf;
    switch( type )
    {
        case DEF_PACK_REGISTER_RS:         //注册
            DealRegisterRs(buf,len);
        break;
        case DEF_PACK_LOGIN_RS:            //登录
            DealLoginRs(buf,len);
        break;
        case DEF_PACK_CREATEROOM_RS:        //创建房间
           // DealCreateRoomRs(buf,len);
        break;
        case DEF_PACK_JOINROOM_RS:           //加入房间
           // DealJoinRoomRs(buf,len);
        break;
        case DEF_PACK_ROOM_MEMBER:            //房间列表请求
           // DealRoomMemberRq(buf,len);
        break;
        case DEF_PACK_LEAVEROOM_RS:            //退出房间回复
          //  DealLeaveRoomRs(buf,len);
        break;
    default:
        break;
    }
    delete[] buf;
}

void MusicWidget::on_pushButton_search_clicked()
{
    char *buf="1111111111111";
   //  m_pTcp->SendData( (char*)buf , sizeof(buf));
}
void MusicWidget::slot_LoginCommit(QString name,QString password)
{
//    STRU_LOGIN_RQ rq;
//    std::string str = name.toStdString();
//    char* buf = (char*) str.c_str();

//    strcpy_s( rq.m_szUser , buf);

//    char* arr = GetMD5( password );

//    memcpy( rq.m_szPassword , arr.data(),arr.length() );

//    m_UserName = name;
   // m_pTcp->SendData( (char*)&rq , sizeof(rq));
}

void MusicWidget::slot_RegisterCommit(QString name,QString password)
{
    STRU_REGISTER_RQ rq;


    std::string str = name.toStdString();
    char* buf = (char*) str.c_str();

    strcpy_s( rq.m_szUser , buf);

    QByteArray arr = GetMD5( password );

    memcpy( rq.m_szPassword , arr.data() ,arr.length() );

   // m_pTcp->SendData( (char*)&rq , sizeof(rq));
}
void MusicWidget::DealRegisterRs(char* buf,int len)
{
    //    //注册请求结果
    //    #define userid_is_exist      0
    //    #define register_sucess      1

        STRU_REGISTER_RS * rs = (STRU_REGISTER_RS *)buf;
        switch( rs->m_lResult)
        {
            case userid_is_exist:
            QMessageBox::warning( m_pLogin , "提示","用户已存在,注册失败" );
            break;
            case register_sucess:
            QMessageBox::warning( m_pLogin , "提示","注册成功" );
            break;
        }
}

void MusicWidget::DealLoginRs(char* buf,int len)
{
    //登录请求结果
    //    #define userid_no_exist      0
    //    #define password_error       1
    //    #define login_sucess         2
    //    #define user_online          3
        STRU_LOGIN_RS * rs = (STRU_LOGIN_RS*)buf;

        switch( rs->m_lResult)
        {
        case userid_no_exist:
            QMessageBox::warning( m_pLogin , "提示","用户不存在,登录失败" );
            break;
        case password_error:
            QMessageBox::warning( m_pLogin , "提示","密码错误,登录失败" );
            break;
        case user_online:
            QMessageBox::warning( m_pLogin , "提示","用户已在线,登录失败" );
            break;
        case login_sucess:
              QMessageBox::warning( m_pLogin , "提示","登录成功" );
           // m_UserID = rs->m_UserID;
            this->hide();
           //更新歌单列表

            break;
        }
}

void MusicWidget::on_pushButton_people_1_clicked()
{

}
