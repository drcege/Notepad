/********************************************************************************
** Form generated from reading UI file 'mytextedit.ui'
**
** Created: Tue May 13 17:11:39 2014
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYTEXTEDIT_H
#define UI_MYTEXTEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyTextEdit
{
public:

    void setupUi(QWidget *MyTextEdit)
    {
        if (MyTextEdit->objectName().isEmpty())
            MyTextEdit->setObjectName(QString::fromUtf8("MyTextEdit"));
        MyTextEdit->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MyTextEdit->sizePolicy().hasHeightForWidth());
        MyTextEdit->setSizePolicy(sizePolicy);

        retranslateUi(MyTextEdit);

        QMetaObject::connectSlotsByName(MyTextEdit);
    } // setupUi

    void retranslateUi(QWidget *MyTextEdit)
    {
        MyTextEdit->setWindowTitle(QApplication::translate("MyTextEdit", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyTextEdit: public Ui_MyTextEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYTEXTEDIT_H
