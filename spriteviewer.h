#ifndef SPRITEVIEWER_H
#define SPRITEVIEWER_H

#include <QWidget>

class Sprite
{
public:
    QString filename;
    QSizeF size;
    unsigned int steps;
    unsigned int directions;
};

class SpriteViewer : public QWidget
{
    Q_OBJECT
public:
    explicit SpriteViewer(QWidget *parent = 0);
    void setOrientation(bool horizontal);
    void setAlignement(Qt::AlignmentFlag flags);
    void setSprite(QString filename, QSize size, unsigned int steps, unsigned int directions);
    QSizeF getSpriteSize();
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    Sprite sprite;
    bool orientation;
    Qt::AlignmentFlag alignement;

    unsigned long tick;
    unsigned int step;
    bool direction;
    int position;

    QPixmap pixmap;
};

#endif // SPRITEVIEWER_H
