/* Copyright © 2013-2014 Paul DU
 * This file is part of Heavy Ball.

 * Heavy Ball is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Heavy Ball is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Heavy Ball.  If not, see <http://www.gnu.org/licenses/>. */

#include "boule.h"
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QSvgRenderer>
#include "conteneur.h"

Boule::Boule(QWidget *parent) :
    Boule(0, 0, nullptr, parent)
{

}

Boule::Boule(Boule::Etat etat) :
    Boule(0, 0, nullptr, nullptr)
{
    setEtat(etat);
}

Boule::Boule(int num, int masse, Conteneur *conteneur, QWidget *parent) :
    QWidget(parent), m_num(num), m_masse(masse), m_conteneur(conteneur), m_etat(BOULE), m_eyes(OPENED), m_cache_invalid(true), m_block(false), m_count(0)
{
    setMaximumSize(64,64);
    setDefaultPolicy();
}

QPixmap Boule::renderPixmaps(Etat etat, int blink, QSize size)
{
    if (etat < BOULE && etat > DAMN)
            return QPixmap();

    QString name("");
    if (etat == BOULE)
        name = ":/img/boule.svg";
    else if (etat == GOOD_JOB)
    {
        if (!blink)
            name = ":/img/good_job.svg";
        else
            name = ":/img/blink_good_job.svg";
    }
    else if (etat == WELL_WELL_JOB)
    {
        if (!blink)
            name = ":/img/well_well_job.svg";
        else
            name = ":/img/blink_well_well_job.svg";
    }
    else if (etat == BAD_JOB)
    {
        if (!blink)
            name = ":/img/bad_job.svg";
        else
            name = ":/img/blink_bad_job.svg";
    }
    else
        name = ":/img/damn.svg";

    QSvgRenderer renderer(name);
    QPixmap pixmap(size);
    pixmap.fill(QColor(255,255,255,0));
    QPainter painter(&pixmap);
    renderer.render(&painter);
    return pixmap;
}

void Boule::setDefaultPolicy()
{
    //QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
}

void Boule::setNum(int num)
{
    if (num < 1)
        m_num = 1;
    else
        m_num = num;
}

void Boule::setMasse(int masse)
{
    m_masse = masse;
}

void Boule::setEtat(Etat etat)
{
    if (etat < BOULE && etat > BAD_JOB)
        return;

    m_etat = etat;
    m_cache_invalid = true;
    if (m_etat != BOULE)
        updateEyes();
    else
        update();
}

int Boule::num()
{
    return m_num;
}

int Boule::masse()
{
    return m_masse;
}

void Boule::drawWidget(QPaintDevice *device, QSize size, bool update_cache)
{
    if (!size.isValid())
        size = Boule::size();

    QPainter painter(device);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (update_cache && (m_cache_invalid || m_pixmap.size() != size))
        m_pixmap = renderPixmaps(m_etat, m_eyes, size);

    if (m_block)
        painter.drawPixmap(0,0, renderPixmaps(m_etat, 1, size));
    else if (m_pixmap.size() == size)
        painter.drawPixmap(0,0, m_pixmap);
    else
        painter.drawPixmap(0,0, renderPixmaps(m_etat, m_eyes, size));

    if (m_etat == BOULE && m_num != 0)
        painter.drawText(0,0, size.width(), size.height(), Qt::AlignCenter, QString::number(m_num));
}

void Boule::paintEvent(QPaintEvent *)
{
    drawWidget(this, size(), true);
}

void Boule::updateEyes()
{
    if (m_etat == BOULE)
        return;

    m_cache_invalid = true;
    m_eyes = m_eyes ? OPENED : CLOSED;
    update();
    if (m_eyes)
        QTimer::singleShot(100, this, SLOT(updateEyes()));
    else
        QTimer::singleShot(qrand()%(7500-2000)+2000, this, SLOT(updateEyes()));
}


void Boule::mousePressEvent(QMouseEvent *event)
{
    if (m_etat != BOULE)
    {
        m_count++;
        if (m_count == 5)
            QMessageBox::information(this, "1.61803398875", "Pas besoin de te défouler sur moi !   :)");
        else if (m_count == 10)
            QMessageBox::information(this, "1.61803398875", "Tu veux ma peau ?   :o");
        else if (m_count == 20)
            QMessageBox::information(this, "1.61803398875", "C'est bon ! Ça suffit là !   >:(");
        else if (m_count == 30)
            QMessageBox::information(this, "1.61803398875", "La boutonnite, ça se soigne.   :)");
        else if (m_count == 40)
            QMessageBox::information(this, "1.61803398875", "Tu n'as rien d'autre à faire ?   :o");
        else if (m_count == 50)
        {
            QMessageBox::information(this, "1.61803398875", "Adieu !   >:(");
            std::terminate();
        }
    }
    if (event->button() == Qt::LeftButton)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *data = new QMimeData;
        Boule *obj = this;
        data->setData("Boule", QByteArray::fromRawData((char *)&obj, sizeof(obj)));
        drag->setMimeData(data);
        QPixmap pixmap(QSize(64, 64));
        pixmap.fill(QColor(255,255,255,0));
        drawWidget(&pixmap, QSize(64,64), false);
        drag->setPixmap(pixmap);
        drag->start();
        if (m_etat != BOULE && m_eyes && m_etat != DAMN)
        {
            m_block = true;
            QMessageBox::information(this, "3.141592654", "Je ne l'avais pas vu venir !   :)");
            m_etat = DAMN;
            update();
        }
    }
    else
        mouseDoubleClickEvent(event);
}

void Boule::mouseDoubleClickEvent(QMouseEvent *)
{
    if (m_conteneur)
    {
        QWidget *old_parent = parentWidget();
        if (old_parent == m_conteneur)
            return;

        m_conteneur->ajouterBoule(this);
        if (old_parent)
            reinterpret_cast<Conteneur*>(old_parent)->ajusterLayout();
    }
}

