#include "softkeyboard.h"
#include <QGridLayout>
#include <QPushButton>

SoftKeyboard::SoftKeyboard(QWidget *parent) : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = i*3 + j + 1;
            QPushButton* btn = new QPushButton(this);
            btn->setText(QString::number(num));
            connect(btn, &QPushButton::clicked, this, &SoftKeyboard::charClicked);
            layout->addWidget(btn, i, j, 1, 1);
        }
    }
    QPushButton* dotBtn = new QPushButton(".", this);
    QPushButton* zeroBtn = new QPushButton("0", this);
    QPushButton* backspaceBtn = new QPushButton("DEL", this);
    connect(dotBtn, &QPushButton::clicked, this, &SoftKeyboard::charClicked);
    connect(zeroBtn, &QPushButton::clicked, this, &SoftKeyboard::charClicked);
    connect(backspaceBtn, &QPushButton::clicked, this, &SoftKeyboard::backspacePressed);

    layout->addWidget(dotBtn, 3, 0, 1, 1);
    layout->addWidget(zeroBtn, 3, 1, 1, 1);
    layout->addWidget(backspaceBtn, 3, 2, 1, 1);

    for (int i = 0; i < 3; i++) {
        layout->setColumnStretch(i, 1);
    }

    setLayout(layout);
}

void SoftKeyboard::charClicked()
{
    QPushButton* btn = qobject_cast<QPushButton *>(sender());
    mLineEdit->setText(mLineEdit->text().append(btn->text()));
}

QLineEdit *SoftKeyboard::lineEdit() const
{
    return mLineEdit;
}

void SoftKeyboard::setLineEdit(QLineEdit *lineEdit)
{
    mLineEdit = lineEdit;
}

void SoftKeyboard::backspacePressed()
{
    QString text = mLineEdit->text();
    text.chop(1);
    mLineEdit->setText(text);
}
