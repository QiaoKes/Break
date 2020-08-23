#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QHeaderView>
#include <qmediaplayer.h>
#include <QMediaPlayer>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
protected:
    void paintEvent(QPaintEvent* event);
private:
    Ui::Form *ui;
private slots:
        void slot_setWindowBackGroundImage(QString m_CurrentMusicName);
};

#endif // FORM_H
