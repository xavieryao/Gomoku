#ifndef SOFTKEYBOARD_H
#define SOFTKEYBOARD_H

#include <QWidget>
#include <QLineEdit>

class SoftKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit SoftKeyboard(QWidget *parent = 0);

    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *lineEdit);

signals:

public slots:
private slots:
    void charClicked();
    void backspacePressed();

private:
    QLineEdit* mLineEdit;
};

#endif // SOFTKEYBOARD_H
