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
    int rowNum = 0, colNum = 0;                 //���к�

    if (e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::ShiftModifier)    //��Ctrl����
    {
        // �ַ�����
        if ((e->key() >= Qt::Key_Space && e->key() <= Qt::Key_AsciiTilde) || e->key() == Qt::Key_Tab)
        {
            if (cursor.hasSelection())          // ��ѡ�п���ɾ��
            {
                data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                cursor.removeSelectedText();
            }

            rowNum = cursor.blockNumber();
            colNum = cursor.columnNumber();
            data->Update(e->text().toStdString(), rowNum, colNum);   //���µ��ڴ�
            cursor.insertText(e->text());                            //�༭����ʾ
            isModified = true;
        }
        else
        {
            //������������
            if (e->modifiers() == Qt::NoModifier)
            {
                switch (e->key())                                    //����ֵ
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
                    if (cursor.hasSelection())                      //ɾ����
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }

                    rowNum = cursor.blockNumber();
                    colNum = cursor.columnNumber();
                    data->Enter(rowNum, colNum);
                    cursor.insertText(e->text());
                    isModified = true;
                    break;

                case Qt::Key_Delete:
                    if (cursor.hasSelection())
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }
                    else
                    {
                        rowNum = cursor.blockNumber();
                        colNum = cursor.columnNumber();
                        data->Delete(rowNum, colNum);
                        cursor.deleteChar();
                    }

                    isModified = true;
                    break;

                case Qt::Key_Backspace:
                    if (cursor.hasSelection())
                    {
                        data->Replace(cursor.selectionStart(), cursor.selectionEnd(), "");
                        cursor.removeSelectedText();
                    }
                    else
                    {
                        rowNum = cursor.blockNumber();
                        colNum = cursor.columnNumber();
                        data->Backspace(rowNum, colNum);
                        cursor.deletePreviousChar();
                    }

                    isModified = true;
                    break;

                default:
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
