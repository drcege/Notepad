#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <qobject.h>
#include "data.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject*,QEvent*);

private slots:

    void on_action_Author_triggered();
    void on_action_Save_triggered();
    void on_action_New_triggered();
    void on_action_SaveAs_triggered();
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Quit_triggered();
    void on_action_Undo_triggered();
    void on_action_Copy_triggered();
    void on_action_Cut_triggered();
    void on_action_Paste_triggered();
    void on_action_Find_triggered();
    void on_action_Replace_triggered();
    void show_findText();
    void replace_findText();
    void do_cursorChanged();

private:
    Ui::MainWindow *ui;
    Data data;


    //为true时标志文件已经保存，为false时标志文件尚未保存
    bool isSaved;
    //用于保存当前文件的文件名
    QString curFile;
    QLineEdit *find_textLineEdit;
    QLineEdit *replace_textLineEdit;
    //声明两个标签，用来显示状态信息
    QLabel* first_statusLabel;
    QLabel* second_statusLabel;
    //初始化状态栏函数
    void init_statusBar();
    //新建文件函数
    void do_file_New();
    //询问修改过的文件是否保存函数
    void do_file_SaveOrNot();
    //保存文件函数
    void do_file_Save();
    //文件另存为函数
    void do_file_SaveAs();
    //存储文件函数
    bool saveFile(const QString& fileName);
    //打开文件函数
    void do_file_Open();
    //读取文件函数
    bool do_file_Load(const QString& fileName);
};

#endif
// MAINWINDOW_H
