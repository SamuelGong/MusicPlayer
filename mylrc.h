#ifndef MYLRC_H
#define MYLRC_H

#include <QLabel>
class QTimer;

class MyLrc : public QLabel
{
    Q_OBJECT
public:
    explicit MyLrc(QWidget *parent = 0);
    void start_lrc_mask(qint64 intervaltime);
    void stop_lrc_mask();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);

signals:

public slots:

private slots:
    void timeout();

private:
    QLinearGradient linear_gradient;
    QLinearGradient mask_linear_gradient;
    QFont font;
    QTimer *timer;
    qreal lrc_mask_width;
    qreal lrc_mask_width_interval;
    QPoint offset;
};

#endif // MYLRC_H
