#include "spriteviewer.h"
#include <QPainter>
#include <QTimer>

SpriteViewer::SpriteViewer(QWidget *parent) :
    QWidget(parent), orientation(true), alignement(), tick(0), step(0), direction(true), position(0), pixmap()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, [this]()
        {
            if (pixmap.isNull())
                return;

            if (direction)
                position += 2;
            else
                position -= 2;

            if (orientation)
            {
                if (position > size().width()-getSpriteSize().width())
                {
                    position = size().width()-getSpriteSize().width();
                    direction = false;
                }
            }
            else
            {
                if (position > size().height()-getSpriteSize().height())
                {
                    position = size().height()-getSpriteSize().height();
                    direction = false;
                }
            }

            if (position < -1)
            {
                position = 0;
                direction = true;
            }

            tick++;

            if (tick % 8 == 0)
                step++;

            if (step >= sprite.steps)
                step = 0;

            update();
        });
    timer->start();
}

void SpriteViewer::setOrientation(bool horizontal)
{
    orientation = horizontal;
}

void SpriteViewer::setAlignement(Qt::AlignmentFlag flags)
{
    alignement = flags;
}

void SpriteViewer::setSprite(QString filename, QSize size, unsigned int steps, unsigned int directions)
{
    sprite.filename = filename;
    sprite.size = size;
    sprite.steps = steps;
    sprite.directions = directions;
    pixmap.load(sprite.filename);
    position = 0;
    step = 0;
    direction = true;
}

QSizeF SpriteViewer::getSpriteSize()
{
    if (sprite.size.width() > size().width() || sprite.size.height() > size().height())
        return sprite.size.scaled(size(), Qt::KeepAspectRatio);
    else
        return sprite.size;
}

void SpriteViewer::paintEvent(QPaintEvent *)
{
    if (pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRectF dest(QPoint(0,0), getSpriteSize());

    if (orientation)
    {
        if (alignement & Qt::AlignTop)
            dest.moveTop(0);
        else if (alignement & Qt::AlignBottom)
            dest.moveBottom(rect().bottom());
        else if (alignement & Qt::AlignVCenter)
            dest.moveCenter(rect().center());
        dest.moveLeft(position);
    }
    else
    {
        if (alignement & Qt::AlignLeft)
            dest.moveLeft(0);
        else if (alignement & Qt::AlignRight)
            dest.moveRight(rect().right());
        else if (alignement & Qt::AlignHCenter)
            dest.moveCenter(rect().center());

        dest.moveTop(position);
    }

    QRectF source;
    if (sprite.directions >= 2)
        source.setTopLeft(QPointF(step*sprite.size.width(), direction*sprite.size.height()));
    else
        source.setTopLeft(QPointF(step*sprite.size.width(), 0));
    source.setSize(sprite.size);

    painter.drawPixmap(dest, pixmap, source);
}
