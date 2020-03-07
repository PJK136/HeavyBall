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

#include "conteneur.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QLayoutItem>
#include <QQueue>

Conteneur::Conteneur(QWidget *parent) :
    QGroupBox(parent), m_column_count(6), m_dragged_boule(nullptr)
{
    m_layout.setMargin(0);
    m_layout.setContentsMargins(0, 0, 0, 0);
    m_layout.setSpacing(0);
    setLayout(&m_layout);
    setAcceptDrops(true);
}

void Conteneur::setColumnCount(unsigned int column_count)
{
    m_column_count = column_count;
    ajusterLayout();
}

void Conteneur::ajouterBoule(Boule *boule)
{
    m_layout.addWidget(boule);
    ajusterLayout();
}

void Conteneur::deplacerBoules(Conteneur *b)
{
    QQueue<QWidget*> queue;
    for (int i = 0; i < m_layout.count(); i++)
        queue.enqueue(m_layout.itemAt(i)->widget());

    for (QWidget *widget : queue)
        b->ajouterBoule(reinterpret_cast<Boule*>(widget));

    emit conteneurChanged();
}

void Conteneur::ajusterLayout()
{
    if (!m_layout.count())
    {
        emit conteneurChanged();
        return;
    }

    QQueue<QWidget*> queue;
    for (int i = 0; i < m_layout.count(); i++)
        queue.enqueue(m_layout.itemAt(i)->widget());

    for (QWidget *widget : queue)
    {
        Boule *boule = reinterpret_cast<Boule*>(widget);
        m_layout.addWidget(widget, (boule->num()-1)/m_column_count, (boule->num()-1)%m_column_count);
    }

    emit conteneurChanged();
}

bool Conteneur::hasBoules()
{
    return !m_layout.isEmpty();
}

QString Conteneur::toString()
{
    QString ret = "";
    for (int i = 0; i < m_layout.count(); i++)
    {
        if (i)
            ret += ",";
        ret += QString::number(reinterpret_cast<Boule*>(m_layout.itemAt(i)->widget())->num());
    }
    return ret;
}

void Conteneur::showEvent(QShowEvent *event)
{
    QGroupBox::showEvent(event);
    ajusterLayout();
}

void Conteneur::dragEnterEvent(QDragEnterEvent *event)
{
    Boule *boule = *(Boule**)event->mimeData()->data("Boule").data();
    if (boule->parentWidget() == this)
    {
        event->ignore();
        return;
    }
    m_dragged_boule = boule;
    m_dragged_boule_conteneur = reinterpret_cast<Conteneur*>(boule->parentWidget());
    ajouterBoule(boule);
    m_dragged_boule_conteneur->ajusterLayout();
    event->acceptProposedAction();
}

void Conteneur::dragLeaveEvent(QDragLeaveEvent *)
{
    if (m_dragged_boule)
    {
        m_dragged_boule_conteneur->ajouterBoule(m_dragged_boule);
        m_dragged_boule = nullptr;
    }
    ajusterLayout();
}

void Conteneur::dropEvent(QDropEvent *)
{
    m_dragged_boule = nullptr;
    m_dragged_boule_conteneur = nullptr;
}

