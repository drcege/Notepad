#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "mytextedit.h"
#include <QMovie>
#include <QCursor>
#include <QTextEdit>
#include <QKeyEvent>
#include <qobject.h>

int find_pos;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->data = &data;
    ui->textEdit->installEventFilter(this);
    // 初始化文件为未保存过状态
    isSaved = false;
    // 初始化文件名为"未命名.txt"
    curFile = tr("未命名.txt");
    // 初始化主窗口的标题
    setWindowTitle(curFile);
    // 初始化状态栏
    init_statusBar();
    // 链接
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(do_cursorChanged()));
}

bool MainWindow::eventFilter(QObject* target , QEvent* event)         // 事件过滤
{
    if (target == ui->textEdit)                                       //过滤编辑区的按键操作
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

            if (keyEvent->modifiers() == Qt::ControlModifier)         //ctrl修饰
            {
                switch (keyEvent->key())
                {
                case Qt::Key_C:
                    on_action_Copy_triggered();
                    break;

                case Qt::Key_V:
                    on_action_Paste_triggered();
                    break;

                case Qt::Key_X:
                    on_action_Cut_triggered();
                    break;

                default:
                    return false;
                }

                return true;
            }
        }
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)                       //主窗口退出提示
{
    on_action_Quit_triggered();
    event->accept();
}

// 新建操作
void MainWindow::on_action_New_triggered()
{
    do_file_New();
}

// 保存操作
void MainWindow::on_action_Save_triggered()
{
    do_file_Save();
}

// 另存为操作
void MainWindow::on_action_SaveAs_triggered()
{
    do_file_SaveAs();
}

// 打开操作
void MainWindow::on_action_Open_triggered()
{
    do_file_Open();
}

// 关闭操作
void MainWindow::on_action_Close_triggered()
{
    do_file_SaveOrNot();
    ui->textEdit->setVisible(false);
    bar->clearMessage();
}

// 退出操作
void MainWindow::on_action_Quit_triggered()
{
    // 先执行关闭操作
    on_action_Close_triggered();
    // 再退出系统，qApp是指向应用程序的全局指针
    qApp->quit();
}

// 剪切操作
void MainWindow::on_action_Cut_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();

    if (cursor.hasSelection())
    {
        if (data.Cut(cursor.selectionStart(), cursor.selectionEnd()))
        {
            QString s = "";
            cursor.insertText(s);
            ui->textEdit->setTextCursor(cursor);
            bar->showMessage(bar->currentMessage() + tr("    剪切成功！"));
            ui->textEdit->isModified = true;
        }
        else
        {
            bar->showMessage(bar->currentMessage() + tr("    剪切失败！"));
        }
    }

    //ui->textEdit->cut();
}

// 复制操作
void MainWindow::on_action_Copy_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();

    if (cursor.hasSelection())
    {
        if (data.Copy(cursor.selectionStart(), cursor.selectionEnd())) // 从文本开头计数,左开右闭
        {
            bar->showMessage(bar->currentMessage() + tr("    复制成功！"));
        }
        else
        {
            bar->showMessage(bar->currentMessage() + tr("    复制失败！"));
        }
    }

    //ui->textEdit->copy();
}

// 粘贴操作
void MainWindow::on_action_Paste_triggered()
{
    if (data.Clip().empty())  //剪切板无内容
    {
        return ;
    }

    QTextCursor cursor = ui->textEdit->textCursor();
    int rowNum = 0, colNum = 0;
    rowNum = cursor.blockNumber();
    colNum = cursor.columnNumber();

    if (data.Paste(rowNum, colNum))      //光标所在行数和列数
    {
        QString qs = QString::fromStdString(data.Clip());
        cursor.insertText(qs);
        bar->showMessage(bar->currentMessage() + tr("    粘贴成功！"));
        ui->textEdit->isModified = true;
    }
    else
    {
        bar->showMessage(bar->currentMessage() + tr("    粘贴失败！"));
    }

    //ui->textEdit->paste();
}

void MainWindow::init_statusBar()
{
    // 获取状态栏
    bar = ui->statusBar;
    bar->showMessage(tr("欢迎使用"));
}

void MainWindow::do_cursorChanged()
{
    //int rowNum = ui->textEdit->document()->blockCount();
    const QTextCursor cursor = ui->textEdit->textCursor();
    int rowNum = 0, colNum = 0;
    rowNum = cursor.blockNumber();
    colNum = cursor.columnNumber();
    bar->showMessage(tr("%1行 %2列").arg(rowNum).arg(colNum));
}

void MainWindow::on_action_Find_triggered()
{
    QTextCursor cur = ui->textEdit->textCursor();
    //ui->textEdit->setFocus();
    // 新建一个对话框，用于查找操作，this表明它的父窗口是MainWindow。
    QDialog *findDlg = new QDialog(this);
    // 设置对话框的标题
    findDlg->setWindowTitle(tr("查找"));
    // 将行编辑器加入到新建的查找对话框中
    find_textLineEdit = new QLineEdit(findDlg);

    if (cur.hasSelection())
    {
        find_textLineEdit->setText(cur.selectedText());
    }

    // 加入一个"查找下一个"的按钮
    QPushButton *find_Btn = new QPushButton(tr("查找下一个"), findDlg);
    // 新建一个垂直布局管理器，并将行编辑器和按钮加入其中
    QVBoxLayout* layout = new QVBoxLayout(findDlg);
    layout->addWidget(find_textLineEdit);
    layout->addWidget(find_Btn);
    // 显示对话框
    findDlg->show();
    // 设置"查找下一个"按钮的单击事件和其槽函数的关联
    connect(find_Btn, SIGNAL(clicked()), this, SLOT(show_findText()));
}

void MainWindow::on_action_Replace_triggered()
{
    // 新建一个对话框，用于替换操作，this表明它的父窗口是MainWindow。
    QDialog *replaceDlg = new QDialog(this);
    replaceDlg->setWindowTitle(tr("替换"));    // 设置对话框的标题
    find_textLineEdit = new QLineEdit(replaceDlg);    // 将行编辑器加入到新建的替换对话框中
    replace_textLineEdit = new QLineEdit(replaceDlg);
    QPushButton *find_Btn = new QPushButton(tr("查找下一个"), replaceDlg);  // 加入一个"查找下一个"的按钮
    QPushButton *replace_Btn = new QPushButton(tr("替换"), replaceDlg);
    QVBoxLayout* layout1 = new QVBoxLayout();     // 新建一个垂直布局管理器，并将行编辑器和按钮加入其中
    layout1->addWidget(find_textLineEdit);
    layout1->addWidget(replace_textLineEdit);
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget(find_Btn);
    layout2->addWidget(replace_Btn);
    QHBoxLayout* layout = new QHBoxLayout(replaceDlg);   // 新建一个水平布局管理器
    layout->addLayout(layout1);
    layout->addLayout(layout2);
    replaceDlg ->show();    // 显示对话框
    // 设置"替换"按钮的单击事件和其槽函数的关联
    connect(find_Btn, SIGNAL(clicked()), this, SLOT(show_findText()));
    connect(replace_Btn, SIGNAL(clicked()), this, SLOT(replace_findText()));
}


// "查找下一个"按钮的槽函数
void MainWindow::show_findText()
{
    // 获取行编辑器中的内容
    QString findText = find_textLineEdit->text();

    if (findText.size() == 0)
    {
        return ;
    }

    QTextCursor cur = ui->textEdit->textCursor();
    find_pos = cur.position();

    //bar->showMessage(bar->currentMessage() + tr("    %1").arg(find_pos));
    if (data.Find(find_pos, findText.toStdString()))
    {
        this->activateWindow();
        cur.setPosition(find_pos);
        cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, findText.length());
        ui->textEdit->setTextCursor(cur);
        //bar->showMessage(bar->currentMessage() + tr("    %1").arg(find_pos));
        //bar->showMessage(bar->currentMessage() + tr("    %1").arg(findText));
    }
    else
    {
        QMessageBox::warning(this, tr("查找"), tr("未找到 %1！").arg(findText));
    }
}

//"替换"按钮的槽函数
void MainWindow::replace_findText()
{
    QString findText = find_textLineEdit->text();
    QString replaceText = replace_textLineEdit->text();
    QTextCursor cur = ui->textEdit->textCursor();

    //show_findText();
    if (cur.hasSelection())
    {
        data.Replace(cur.selectionStart(), cur.selectionEnd(), replaceText.toStdString());
        this->activateWindow();
        cur.removeSelectedText();
        cur.insertText(replaceText);
    }
    else
    {
        QMessageBox::warning(this, tr("替换"), tr("请先查找！"));
    }
}

// 保存文件内容，因为可能保存失败，所以具有返回值，来表明是否保存成功
bool MainWindow::saveFile(const QString& fileName)
{
    QFile file(fileName);

    // 以只写方式打开文件，如果打开失败则弹出提示框并返回
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        // %1,%2表示后面的两个arg参数的值
        QMessageBox::warning(this, tr("保存文件"),
                             tr("无法保存文件 %1:\n %2！").arg(fileName).arg(file.errorString()));
        return false;
    }

    // 新建流对象，指向选定的文件
    QTextStream out(&file);
    // 将文本编辑器里的内容以纯文本的式输出到流对象中
    string all = data.Text();
    out << QString::fromStdString(all);
    isSaved = true;
    ui->textEdit->isModified = false;
    // 获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    // 将窗口名称改为现在窗口的路径
    setWindowTitle(curFile);
    bar->showMessage(bar->currentMessage() + tr("    保存文件成功！"));
    file.close();
    return true;
}

// 文件另存为
void MainWindow::do_file_SaveAs()
{
    // 获得文件名
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), curFile);

    // 如果文件名不为空，则保存文件内容
    if (!fileName.isEmpty())
    {
        saveFile(fileName);
    }
}

// 保存文件
void MainWindow::do_file_Save()
{
    // 如果文件已经被保存过，直接保存文件
    if (isSaved)
    {
        saveFile(curFile);
    }
    // 如果文件是第一次保存，那么调用另存为
    else
    {
        do_file_SaveAs();
    }
}

// 弹出是否保存文件对话框
void MainWindow::do_file_SaveOrNot()
{
    // 如果文件被更改过，弹出保存对话框
    if (ui->textEdit->isModified)
    {
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr("尚未保存，是否保存?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        // 如果选择保存文件，则执行保存操作
        if (box.exec() == QMessageBox::Yes)
        {
            do_file_Save();
        }
        else
        {
            ui->textEdit->isModified = false;
        }
    }
}

// 打开文件
void MainWindow::do_file_Open()
{
    // 是否需要保存现有文件
    do_file_SaveOrNot();
    // 获得要打开的文件的名字
    QString fileName = QFileDialog::getOpenFileName(this);

    // 如果文件名不为空
    if (!fileName.isEmpty())
    {
        do_file_Load(fileName);
    }

    // 文本编辑器可见
    ui->textEdit->setVisible(true);
}

// 读取文件
bool MainWindow::do_file_Load(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("读取文件"), tr("无法读取  文件 %1:\n%2！").arg(fileName).arg(file.errorString()));
        // 如果打开文件失败，弹出对话框，并返回
        return false;
    }

    QTextStream in(&file);
    // 将文件中的所有内容都写到文本编辑器中
    QString alltext = in.readAll();
    ui->textEdit->setText(alltext);
    data.Load(alltext.toStdString());
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    bar->showMessage(bar->currentMessage() + tr("    打开文件成功！"));
    file.close();
    return true;
}

// 实现新建文件的功能
void MainWindow::do_file_New()
{
    do_file_SaveOrNot();
    isSaved = false;
    curFile = tr("未命名.txt");
    setWindowTitle(curFile);
    // 清空文本编辑器
    ui->textEdit->clear();
    data.Clear();
    // 文本编辑器可见
    ui->textEdit->setVisible(true);
}

// 显示作者信息
void MainWindow::on_action_Author_triggered()
{
    QDialog *about = new QDialog(this);
    about->setWindowTitle(tr("关于"));
    QLabel* label_gif = new QLabel(about);
    label_gif->setScaledContents(true);
    QMovie* movie = new QMovie(QString::fromUtf8(":/images/images/cat.gif"));
    movie->start();
    label_gif->setMovie(movie);
    QLabel* label_info = new QLabel(tr("作者：戈策  何心  邹晓悦\n邮箱：1334527295@qq.com"), about);
    label_info->setScaledContents(true);
    label_info->setAlignment(Qt::AlignCenter);
    QFont ft(tr("微软雅黑"), 13);
    label_info->setFont(ft);
    QVBoxLayout* layout = new QVBoxLayout(about);
    layout->addWidget(label_gif);
    layout->addWidget(label_info);
    about->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
