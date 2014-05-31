#include "mytextedit.h"
#include "ui_mytextedit.h"
#include <QKeyEvent>
#include <QMessageBox>

MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::MyTextEdit)
{
    ui->setupUi(this);
    isModified = false;
}

MyTextEdit::~MyTextEdit()
{
    delete ui;
}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cursor = textCursor();

    if (e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::ShiftModifier)
    {
        if ((e->key() >= Qt::Key_Space && e->key() <= Qt::Key_AsciiTilde) || e->key() == Qt::Key_Tab)
        {
            if(cursor.hasSelection())
            {
                data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                cursor.removeSelectedText();
            }
            data->Update(e->text().toStdString(), cursor.blockNumber(), cursor.columnNumber());
            cursor.insertText(e->text());
            isModified = true;
        }
        else
        {
            if (e->modifiers() == Qt::NoModifier)
            {

                switch (e->key())
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

                case Qt::Key_Return:
                    if(cursor.hasSelection())
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }
                    data->Enter(cursor.blockNumber(), cursor.columnNumber());
                    cursor.insertText(e->text());
                    isModified = true;
                    break;

                case Qt::Key_Delete:
                    if(cursor.hasSelection())
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }
                    else
                    {
                        data->Delete(cursor.blockNumber(), cursor.columnNumber());
                        cursor.deleteChar();
                    }
                    isModified = true;
                    break;

                case Qt::Key_Backspace:
                    if(cursor.hasSelection())
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }
                    else
                    {
                        data->Backspace(cursor.blockNumber(), cursor.columnNumber());
                        cursor.deletePreviousChar();
                    }
                    isModified = true;
                    break;

                default:
                    //data->putchar(e->text()[0].unicode()); // TODO: ignore other keys or add more keys
                    QTextEdit::keyPressEvent(e);
                    break;
                } // end of switch
            } // end of if
            else
            {
                QTextEdit::keyPressEvent(e);
            }
        } //end of else
    } //end of if
    else
    {
        QTextEdit::keyPressEvent(e);
    }
}

