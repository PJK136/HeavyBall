#ifndef RESULTAT_H
#define RESULTAT_H

#include <QDialog>
#include <QSvgWidget>
#include <QLabel>
#include <QDialogButtonBox>

class Resultat : public QDialog
{
    Q_OBJECT
public:
    explicit Resultat(int etat, QString message, QWidget *parent = 0);
signals:

public slots:

protected:
    void showEvent(QShowEvent *event);
    QSvgWidget *m_smiley;
    QLabel *m_label;
    QLabel *m_label_message;
    QDialogButtonBox *m_button;
    int m_etat;
    int m_count;
    int m_blink;

protected slots:
    void updateSmiley();
    void updateBlink();
};

#endif // RESULTAT_H
