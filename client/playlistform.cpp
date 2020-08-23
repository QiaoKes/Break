#include "playlistform.h"
#include "ui_playlistform.h"
#include <QFileDialog>
#include <QFileInfo>




PlayListForm::PlayListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayListForm)
{
    ui->setupUi(this);
   musicListCounter = 0;  //初始化音乐列表为空
   musicLrcLineCounter = 0;  //初始化歌词列表为空
   myplayer= new QMediaPlayer();
   myplayer->setVolume(30);
   connect(myplayer, SIGNAL(positionChanged(qint64)), this, SLOT(slot_MusicPositionChanged(qint64)));
   //connect(myplayer, SIGNAL(SendTime()), this, SLOT(slot_MusicPositionChanged(qint64)));
}

PlayListForm::~PlayListForm()
{
    delete ui;
}

void PlayListForm::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter_mainback(this);
    painter_mainback.setPen(Qt::NoPen);
    painter_mainback.setBrush(QColor(255,255,255));
    painter_mainback.drawRect(0,0,this->width(),this->height());

}


void PlayListForm::on_pushButton_clicked()
{
    bool hasSameItem = false;

       QStringList path = QFileDialog::getOpenFileNames(this, "选择音乐文件", "C:\\Users\\panda\\Music");
       for(int i=0; i<path.count(); i++)  //逐条添加歌曲到歌曲列表中
       {
           hasSameItem = false;
           for(int j=0; j<musicListCounter; j++)  //寻找是否有相同的地址的歌曲
           {
               if(musicList[j].compare(path.at(i)) == 0)
               {
                   hasSameItem = true;
                   break;
               }
           }
           if(hasSameItem == false)   //歌曲不重复则可以被添加进来
           {
               musicList[musicListCounter++] = path.at(i);  //把选取的每条信息加入记录列表
               QFileInfo info(path.at(i));
               ui->listWidget->addItem(info.baseName());  //将路径转化为文件名
           }
       }

}

void PlayListForm::on_listWidget_doubleClicked(const QModelIndex &index)
{


    //首先获取当前要播放的音乐
    //ui->lw_listMusic->currentIndex().row(); // 双击的行号
    // m_musicList[ui->lw_listMusic->currentIndex().row()] 双击的音乐文件路径
    myplayer->setMedia(QUrl::fromLocalFile( musicList[ui->listWidget->currentIndex().row()] ));
    myplayer->play();

    m_currentMusicName = ui->listWidget->currentItem()->text();
    m_musicStartFlag = true;
    emit SendPicturePath();
    emit SendCurrentName(m_currentMusicName);
   // f_ui->->setIcon(QIcon(":/images/btn_play2.png"));  //  :/  代表加载资源文件

   // setWindowBackGroundImage();
}

 void PlayListForm::slot_MusicPositionChanged(qint64 val)
 {
     static int playFlag = 0;
        static bool lrcBeginFlag = false;

        if( myplayer->state() == QMediaPlayer::PlayingState )
        {
     //       playFlag++;
            if( myplayer->duration()/*&& playFlag >= 2*/)
            {
                playFlag= 0;

                m_musicPositionChangeFlag = true;
                QTime duration(0,myplayer->position()/60000, qRound(myplayer->position()%60000/1000.0),0);
                QString time = duration.toString("mm:ss");
                QTime duration2(0,myplayer->duration()/60000, qRound(myplayer->duration()%60000/1000.0),0);

                 emit SendTime();
                if( duration == duration2)
                {
                    if( musicListCounter == 0 )
                    {
                        //回到出事状态
                        myplayer->pause();
                        m_musicStartFlag = false;
                        emit SendPicturePath();
                       // ui->pb_play->setIcon(QIcon(":/images/bfzn_004.png"));
                        return;
                    }
                    //以下一曲为例

                    myplayer->setMedia( QUrl::fromLocalFile(musicList[(ui->listWidget->currentIndex().row()+1)%musicListCounter]));
                    ui->listWidget->setCurrentRow((ui->listWidget->currentIndex().row()+1)%musicListCounter );
                    myplayer->play();
                    m_currentMusicName = ui->listWidget->currentItem()->text();
                    //ui->lb_CurrentMusicName->setText(m_CurrentMusicName);
                    emit SendCurrentName(m_currentMusicName);
                }
            }
        }
}
 void PlayListForm::SetSound(int value)
 {
     myplayer->setVolume(value);
 }

void PlayListForm::on_listWidget_activated(const QModelIndex &index)
{

}

void PlayListForm::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *pMenu = new QMenu(this);
    QAction *pNewTask = new QAction(tr("删除歌曲"), this);
    QAction *pEditTask = new QAction(tr("喜爱"), this);
    pNewTask->setData(1);
    pEditTask->setData(2);
    pMenu->addAction(pNewTask);
    pMenu->addAction(pEditTask);
    pMenu->exec(cursor().pos());

    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list) delete pAction;
    delete pMenu;
}
