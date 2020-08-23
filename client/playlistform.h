#ifndef PLAYLISTFORM_H
#define PLAYLISTFORM_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QHeaderView>
#include <qmediaplayer.h>
#include <QMediaPlayer>
#include<QPixmap>
#include<QTime>
#include<QMessageBox>
#include<QMenu>
#define MUSICLIST_MAXLEN 200
#define MUSICLRC_MAXLEN 500

namespace Ui {
class PlayListForm;
}

class PlayListForm : public QWidget
{
    Q_OBJECT
public:
    explicit PlayListForm(QWidget *parent = 0);
    ~PlayListForm();
    QMediaPlayer * myplayer;
     Ui::PlayListForm *ui;
protected:
    void paintEvent(QPaintEvent* event);
private slots:


    void on_pushButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

     void slot_MusicPositionChanged(qint64 val);

     void SetSound(int);
     void on_listWidget_activated(const QModelIndex &index);

     void on_listWidget_customContextMenuRequested(const QPoint &pos);

signals:
    void SendPicturePath();
    void SendCurrentName(QString);
    void SendTime();
private:

    QString musicList[MUSICLIST_MAXLEN];
        QString musicLrc[MUSICLRC_MAXLEN];
        QString currentMusicName;
        QString m_currentMusicName;
        int musicListCounter;
        int musicLrcLineCounter;
        int lrcAddLineCounter;
        int musicVolume_last;
        int musicVolume_now;
        QPoint startRelativePos;

        int playFlag;
        bool musicPositionChangeFlag;
        bool musicStartFlag;
        bool voiceOpenFlag;
        bool m_musicStartFlag;
        bool m_musicPositionChangeFlag;


};
#endif // PLAYLISTFORM_H
