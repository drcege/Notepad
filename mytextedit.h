#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include "data.h"

namespace Ui
{
class MyTextEdit;
}

class MyTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit MyTextEdit(QWidget *parent = 0);
    ~MyTextEdit();

public:
    Data *data;
    bool isModified;
    bool wrap;
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MyTextEdit *ui;
};

#endif // MYTEXTEDIT_H
