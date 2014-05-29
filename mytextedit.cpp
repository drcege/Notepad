#include "mytextedit.h"
#include "ui_mytextedit.h"
#include <QKeyEvent>
#include <QMessageBox>

MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::MyTextEdit)
{
    ui->setupUi(this);
}

MyTextEdit::~MyTextEdit()
{
    delete ui;
}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cursor = textCursor();
    if(e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::ShiftModifier)
    {
        if((e->key() >= Qt::Key_Space && e->key() <= Qt::Key_AsciiTilde) || e->key() == Qt::Key_Tab)
        {
            cursor.insertText(e->text());
            data->Update(e->text().toStdString(), cursor.blockNumber(), cursor.columnNumber());
        }
        else
        {
            if(e->modifiers() == Qt::NoModifier)
            {
                switch(e->key())
                {
                case Qt::Key_Up:
                    cursor.movePosition(QTextCursor::Up);
                    setTextCursor(cursor);
                    break;
                case Qt::Key_Down:
                    cursor.movePosition(QTextCursor::Down);
                    setTextCursor(cursor);
                    break;
                case Qt::Key_Left:
                    cursor.movePosition(QTextCursor::Left);
                    setTextCursor(cursor);
                    break;
                case Qt::Key_Right:
                    cursor.movePosition(QTextCursor::Right);
                    setTextCursor(cursor);
                    break;
                case Qt::Key_Enter:
                    cursor.insertText(e->text());
                    data->Enter(cursor.blockNumber(), cursor.columnNumber());
                    break;
                case Qt::Key_Delete:
                    cursor.deleteChar();
                    data->Delete(cursor.blockNumber(), cursor.columnNumber());
                    break;
                case Qt::Key_Backspace:
                    cursor.deletePreviousChar();
                    data->Backspace(cursor.blockNumber(), cursor.columnNumber());
                    break;
                default:
                    //data->putchar(e->text()[0].unicode()); // TODO: ignore other keys or add more keys
                    QTextEdit::keyPressEvent(e);
                    break;
                } // end of switch
            } // end of if
            else
                QTextEdit::keyPressEvent(e);
        } //end of else
    } //end of if
    else
        QTextEdit::keyPressEvent(e);
}

