#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
     ui->setupUi(this);
//    QPixmap pixmap = QPixmap(":/images/start.png");
//    pixmap = pixmap.scaled(this->width(),this->height());
//    QPalette pal(this->palette());
//    pal.setBrush(QPalette::Background , QBrush(pixmap));
//    this->setPalette( pal );
//    //this->setAutoFillBackground(true);
     QPalette palette = this->palette();
     QPixmap pixmap(":/images/start.png");
     pixmap = pixmap.scaled(this->width(),this->height());
     palette.setBrush(QPalette::Background, QBrush(pixmap));
     this->setPalette(palette);

    this->setAutoFillBackground(true);
//    this->setAutoFillBackground(true);
//    QPalette palette;

//    this->setPalette(palette);
   // this->show();
}

Form::~Form()
{
    delete ui;
}
void Form::paintEvent(QPaintEvent *event)
{
     QPixmap pixmap(":/images/start.png");
    Q_UNUSED(event);
    QPainter painter_mainback(&pixmap);
    painter_mainback.setPen(Qt::NoPen);
    painter_mainback.setBrush(QColor(255,255,255));

    painter_mainback.drawRect(0,0,this->width(),this->height());

}
void Form::slot_setWindowBackGroundImage(QString m_CurrentMusicName)
{
    QPalette palette = this->palette();
    QPixmap pixmap = QPixmap(":/images/"+m_CurrentMusicName+".png");
        if(pixmap.isNull() == false)
        {
            pixmap = pixmap.scaled(this->width(),this->height());
            palette.setBrush(QPalette::Background, QBrush(pixmap));
            this->setPalette(palette);
        }else
        {
            QPixmap pixmap = QPixmap(":/images/"+m_CurrentMusicName+".jpg");
            if(pixmap.isNull() == false)
            {
                pixmap = pixmap.scaled(this->width(),this->height());

                palette.setBrush(QPalette::Background, QBrush(pixmap));
                this->setPalette(palette);
            }else
            {
                //不能找到当前歌曲的背景图片设置默认
                QPixmap pixmap = QPixmap(":/images/start.png");
                pixmap = pixmap.scaled(this->width(),this->height());
                palette.setBrush(QPalette::Background, QBrush(pixmap));
                this->setPalette(palette);
            }
        }
}
