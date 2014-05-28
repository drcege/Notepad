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


    //Ϊtrueʱ��־�ļ��Ѿ����棬Ϊfalseʱ��־�ļ���δ����
    bool isSaved;
    //���ڱ��浱ǰ�ļ����ļ���
    QString curFile;
    QLineEdit *find_textLineEdit;
    QLineEdit *replace_textLineEdit;
    //����������ǩ��������ʾ״̬��Ϣ
    QLabel* first_statusLabel;
    QLabel* second_statusLabel;
    //��ʼ��״̬������
    void init_statusBar();
    //�½��ļ�����
    void do_file_New();
    //ѯ���޸Ĺ����ļ��Ƿ񱣴溯��
    void do_file_SaveOrNot();
    //�����ļ�����
    void do_file_Save();
    //�ļ����Ϊ����
    void do_file_SaveAs();
    //�洢�ļ�����
    bool saveFile(const QString& fileName);
    //���ļ�����
    void do_file_Open();
    //��ȡ�ļ�����
    bool do_file_Load(const QString& fileName);
};

#endif
// MAINWINDOW_H
