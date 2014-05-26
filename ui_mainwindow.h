/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon May 26 10:25:37 2014
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "mytextedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *action_Open;
    QAction *action_Close;
    QAction *action_Save;
    QAction *action_SaveAs;
    QAction *action_Quit;
    QAction *action_Undo;
    QAction *action_Copy;
    QAction *action_Paste;
    QAction *action_Cut;
    QAction *action_Find;
    QAction *action_Author;
    QAction *action_Replace;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    MyTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QMenu *menu_E;
    QMenu *menu_H;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(448, 385);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon1);
        action_Close = new QAction(MainWindow);
        action_Close->setObjectName(QString::fromUtf8("action_Close"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Close->setIcon(icon2);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon3);
        action_SaveAs = new QAction(MainWindow);
        action_SaveAs->setObjectName(QString::fromUtf8("action_SaveAs"));
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        action_Undo = new QAction(MainWindow);
        action_Undo->setObjectName(QString::fromUtf8("action_Undo"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Undo->setIcon(icon4);
        action_Copy = new QAction(MainWindow);
        action_Copy->setObjectName(QString::fromUtf8("action_Copy"));
        action_Copy->setIcon(icon2);
        action_Paste = new QAction(MainWindow);
        action_Paste->setObjectName(QString::fromUtf8("action_Paste"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Paste->setIcon(icon5);
        action_Cut = new QAction(MainWindow);
        action_Cut->setObjectName(QString::fromUtf8("action_Cut"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Cut->setIcon(icon6);
        action_Find = new QAction(MainWindow);
        action_Find->setObjectName(QString::fromUtf8("action_Find"));
        action_Author = new QAction(MainWindow);
        action_Author->setObjectName(QString::fromUtf8("action_Author"));
        action_Replace = new QAction(MainWindow);
        action_Replace->setObjectName(QString::fromUtf8("action_Replace"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit = new MyTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setFocusPolicy(Qt::StrongFocus);
        textEdit->setLineWrapMode(QTextEdit::NoWrap);

        gridLayout->addWidget(textEdit, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 448, 22));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QString::fromUtf8("menu_F"));
        menu_E = new QMenu(menuBar);
        menu_E->setObjectName(QString::fromUtf8("menu_E"));
        menu_H = new QMenu(menuBar);
        menu_H->setObjectName(QString::fromUtf8("menu_H"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu_E->menuAction());
        menuBar->addAction(menu_H->menuAction());
        menu_F->addAction(action_New);
        menu_F->addAction(action_Open);
        menu_F->addAction(action_Close);
        menu_F->addAction(action_Save);
        menu_F->addAction(action_SaveAs);
        menu_F->addSeparator();
        menu_F->addAction(action_Quit);
        menu_E->addAction(action_Undo);
        menu_E->addAction(action_Copy);
        menu_E->addAction(action_Paste);
        menu_E->addAction(action_Cut);
        menu_E->addSeparator();
        menu_E->addAction(action_Find);
        menu_E->addAction(action_Replace);
        menu_H->addAction(action_Author);
        mainToolBar->addAction(action_New);
        mainToolBar->addAction(action_Open);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Save);
        mainToolBar->addAction(action_Copy);
        mainToolBar->addAction(action_Paste);
        mainToolBar->addAction(action_Cut);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Undo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)", 0, QApplication::UnicodeUTF8));
        action_New->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        action_Open->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", 0, QApplication::UnicodeUTF8));
        action_Open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        action_Close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255(&W)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Close->setToolTip(QApplication::translate("MainWindow", "\345\205\263\351\227\255(W)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Close->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230(&S)", 0, QApplication::UnicodeUTF8));
        action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        action_SaveAs->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(&A)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_SaveAs->setToolTip(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(&A)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_SaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+S", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&Q)", 0, QApplication::UnicodeUTF8));
        action_Quit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        action_Undo->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200(&Z)", 0, QApplication::UnicodeUTF8));
        action_Undo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        action_Copy->setText(QApplication::translate("MainWindow", "\345\244\215\345\210\266(&C)", 0, QApplication::UnicodeUTF8));
        action_Copy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        action_Paste->setText(QApplication::translate("MainWindow", "\347\262\230\350\264\264(&P)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Paste->setToolTip(QApplication::translate("MainWindow", "\347\262\230\350\264\264(C)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Paste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        action_Cut->setText(QApplication::translate("MainWindow", "\345\211\252\345\210\207(&V)", 0, QApplication::UnicodeUTF8));
        action_Cut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        action_Find->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276(&F)", 0, QApplication::UnicodeUTF8));
        action_Find->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        action_Author->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216(&F&1&2)", 0, QApplication::UnicodeUTF8));
        action_Author->setShortcut(QApplication::translate("MainWindow", "F12", 0, QApplication::UnicodeUTF8));
        action_Replace->setText(QApplication::translate("MainWindow", "\346\233\277\346\215\242(&R)", 0, QApplication::UnicodeUTF8));
        menu_F->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", 0, QApplication::UnicodeUTF8));
        menu_E->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", 0, QApplication::UnicodeUTF8));
        menu_H->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
