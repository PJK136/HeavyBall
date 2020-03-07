#include "resultat.h"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QShowEvent>
#include <QTimer>
#include "boule.h"

Resultat::Resultat(int etat, QString message, QWidget *parent) :
    QDialog(parent), m_etat(etat)
{
    QLayout* layout = new QVBoxLayout;
    setLayout(layout);
    m_smiley = new QSvgWidget(":/img/good_job.svg");
    m_smiley->setFixedSize(300,300);
    m_label = new QLabel("");
    m_label->setAlignment(Qt::AlignCenter);
    m_label_message = new QLabel(message);
    QFont font = m_label->font();
    font.setPixelSize(25);
    m_label->setFont(font);
    font.setPixelSize(12);
    m_label_message->setFont(font);
    m_label_message->setVisible(false);
    m_button = new QDialogButtonBox(QDialogButtonBox::Ok);
    m_button->setVisible(false);
    layout->addWidget(m_smiley);
    layout->setAlignment(m_smiley, Qt::AlignCenter);
    layout->addWidget(m_label);
    layout->setAlignment(m_label, Qt::AlignCenter);
    layout->addWidget(m_label_message);
    layout->setAlignment(m_label_message, Qt::AlignCenter);
    layout->addWidget(m_button);
    setWindowTitle("Résultat");
    connect(m_button, SIGNAL(accepted()), this, SLOT(accept()));
    setFixedSize(450, 450);
}

void Resultat::showEvent(QShowEvent *)
{
    m_count = 0;
    m_label->setText("");
    m_label_message->setVisible(false);
    m_button->setVisible(false);
    updateSmiley();
}

void Resultat::updateSmiley()
{
    if (m_count % 3 == 0)
    {
        m_smiley->load(QString(":/img/good_job.svg"));
        m_label->setText("Gagné !");
    }
    else if ((m_count+1) % 3 == 0)
    {
        m_smiley->load(QString(":/img/well_well_job.svg"));
        m_label->setText("Presque !");
    }
    else
    {
        m_smiley->load(QString(":/img/bad_job.svg"));
        m_label->setText("Perdu !");
    }

    if (m_count > 11 &&
       ((m_count % 3 == 0 && m_etat == Boule::GOOD_JOB) ||
       ((m_count+1) % 3 == 0 && m_etat == Boule::WELL_WELL_JOB) ||
       ((m_count+2) % 3 == 0 && m_etat == Boule::BAD_JOB)))
    {
        m_label_message->setVisible(true);
        m_button->setVisible(true);
        if (m_etat == Boule::GOOD_JOB)
            setWindowTitle("Bravo !");
        else if (m_etat == Boule::WELL_WELL_JOB)
            setWindowTitle("Presque !");
        else
            setWindowTitle("Dommage !");
        updateBlink();
        return;
    }

    m_count++;
    QTimer::singleShot(100, this, SLOT(updateSmiley()));
}


void Resultat::updateBlink()
{
    m_blink = m_blink ? 0 : 1;

    if (m_etat == Boule::GOOD_JOB)
    {
        if (!m_blink)
            m_smiley->load(QString(":/img/good_job.svg"));
        else
            m_smiley->load(QString(":/img/blink_good_job.svg"));
    }
    else if (m_etat == Boule::WELL_WELL_JOB)
    {
        if (!m_blink)
            m_smiley->load(QString(":/img/well_well_job.svg"));
        else
            m_smiley->load(QString(":/img/blink_well_well_job.svg"));
    }
    else
    {
        if (!m_blink)
            m_smiley->load(QString(":/img/bad_job.svg"));
        else
            m_smiley->load(QString(":/img/blink_bad_job.svg"));
    }

    update();
    if (m_blink)
        QTimer::singleShot(100, this, SLOT(updateBlink()));
    else
        QTimer::singleShot(qrand()%(7500-2000)+2000, this, SLOT(updateBlink()));
}

