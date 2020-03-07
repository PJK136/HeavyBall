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

#include "plateau.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QQueue>
#include <QDebug>
#include <boule.h>

Plateau::Plateau(QWidget *parent) :
    Conteneur(parent)
{
    m_layout.setAlignment(Qt::AlignBottom);
}

int Plateau::getRowCount()
{
    return ((m_layout.count()-1)/6)+1;
}

void Plateau::ajusterLayout()
{
    if (!m_layout.count())
    {
        emit conteneurChanged();
        return;
    }

    QQueue<QWidget*> queue;
    for (int i = 0; i < m_layout.count(); i++)
        queue.enqueue(m_layout.itemAt(i)->widget());

    int ligne = getRowCount();
    int count = 0;
    for (QWidget *widget : queue)
    {
        if (count % 6 == 0)
            ligne--;
        m_layout.addWidget(widget, ligne, count % 6, Qt::AlignCenter|Qt::AlignBottom);
        count++;
    }

    emit conteneurChanged();
}

int Plateau::getOptimalHeight()
{
    return m_layout.count() ? qMax(64, getRowCount()*32)+2 : 67;
}

int Plateau::getMasse()
{
    int masse = 0;
    for (int i = 0; i < m_layout.count(); i++)
        masse += reinterpret_cast<Boule *>(m_layout.itemAt(i)->widget())->masse();

    return masse;
}

bool Plateau::operator >(Plateau &b)
{
    if (hasBoules() && !b.hasBoules())
        return true;
    else if (!hasBoules() && b.hasBoules())
        return false;
    else
        return getMasse() > b.getMasse();
}

bool Plateau::operator <(Plateau &b)
{
    if (hasBoules() && !b.hasBoules())
        return false;
    else if (!hasBoules() && b.hasBoules())
        return true;
    else
        return getMasse() < b.getMasse();
}

bool Plateau::operator ==(Plateau &b)
{
    if (hasBoules() && b.hasBoules())
        return getMasse() == b.getMasse();
    else
        return false;
}
