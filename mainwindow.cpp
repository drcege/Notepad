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
    // ��ʼ���ļ�Ϊδ�����״̬
    isSaved = false;
    // ��ʼ���ļ���Ϊ"δ����.txt"
    curFile = tr("δ����.txt");
    // ��ʼ�������ڵı���
    setWindowTitle(curFile);
    // ��ʼ��״̬��
    init_statusBar();
    // ����
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(do_cursorChanged()));
}

bool MainWindow::eventFilter(QObject* target , QEvent* event)         // �¼�����
{
    if (target == ui->textEdit)                                       //���˱༭���İ�������
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

            if (keyEvent->modifiers() == Qt::ControlModifier)         //ctrl����
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

void MainWindow::closeEvent(QCloseEvent *event)                       //�������˳���ʾ
{
    on_action_Quit_triggered();
    event->accept();
}

// �½�����
void MainWindow::on_action_New_triggered()
{
    do_file_New();
}

// �������
void MainWindow::on_action_Save_triggered()
{
    do_file_Save();
}

// ���Ϊ����
void MainWindow::on_action_SaveAs_triggered()
{
    do_file_SaveAs();
}

// �򿪲���
void MainWindow::on_action_Open_triggered()
{
    do_file_Open();
}

// �رղ���
void MainWindow::on_action_Close_triggered()
{
    do_file_SaveOrNot();
    ui->textEdit->setVisible(false);
    bar->clearMessage();
}

// �˳�����
void MainWindow::on_action_Quit_triggered()
{
    // ��ִ�йرղ���
    on_action_Close_triggered();
    // ���˳�ϵͳ��qApp��ָ��Ӧ�ó����ȫ��ָ��
    qApp->quit();
}

// ���в���
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
            bar->showMessage(bar->currentMessage() + tr("    ���гɹ���"));
            ui->textEdit->isModified = true;
        }
        else
        {
            bar->showMessage(bar->currentMessage() + tr("    ����ʧ�ܣ�"));
        }
    }

    //ui->textEdit->cut();
}

// ���Ʋ���
void MainWindow::on_action_Copy_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();

    if (cursor.hasSelection())
    {
        if (data.Copy(cursor.selectionStart(), cursor.selectionEnd())) // ���ı���ͷ����,���ұ�
        {
            bar->showMessage(bar->currentMessage() + tr("    ���Ƴɹ���"));
        }
        else
        {
            bar->showMessage(bar->currentMessage() + tr("    ����ʧ�ܣ�"));
        }
    }

    //ui->textEdit->copy();
}

// ճ������
void MainWindow::on_action_Paste_triggered()
{
    if (data.Clip().empty())  //���а�������
    {
        return ;
    }

    QTextCursor cursor = ui->textEdit->textCursor();
    int rowNum = 0, colNum = 0;
    rowNum = cursor.blockNumber();
    colNum = cursor.columnNumber();

    if (data.Paste(rowNum, colNum))      //�����������������
    {
        QString qs = QString::fromStdString(data.Clip());
        cursor.insertText(qs);
        bar->showMessage(bar->currentMessage() + tr("    ճ���ɹ���"));
        ui->textEdit->isModified = true;
    }
    else
    {
        bar->showMessage(bar->currentMessage() + tr("    ճ��ʧ�ܣ�"));
    }

    //ui->textEdit->paste();
}

void MainWindow::init_statusBar()
{
    // ��ȡ״̬��
    bar = ui->statusBar;
    bar->showMessage(tr("��ӭʹ��"));
}

void MainWindow::do_cursorChanged()
{
    //int rowNum = ui->textEdit->document()->blockCount();
    const QTextCursor cursor = ui->textEdit->textCursor();
    int rowNum = 0, colNum = 0;
    rowNum = cursor.blockNumber();
    colNum = cursor.columnNumber();
    bar->showMessage(tr("%1�� %2��").arg(rowNum).arg(colNum));
}

void MainWindow::on_action_Find_triggered()
{
    QTextCursor cur = ui->textEdit->textCursor();
    //ui->textEdit->setFocus();
    // �½�һ���Ի������ڲ��Ҳ�����this�������ĸ�������MainWindow��
    QDialog *findDlg = new QDialog(this);
    // ���öԻ���ı���
    findDlg->setWindowTitle(tr("����"));
    // ���б༭�����뵽�½��Ĳ��ҶԻ�����
    find_textLineEdit = new QLineEdit(findDlg);

    if (cur.hasSelection())
    {
        find_textLineEdit->setText(cur.selectedText());
    }

    // ����һ��"������һ��"�İ�ť
    QPushButton *find_Btn = new QPushButton(tr("������һ��"), findDlg);
    // �½�һ����ֱ���ֹ������������б༭���Ͱ�ť��������
    QVBoxLayout* layout = new QVBoxLayout(findDlg);
    layout->addWidget(find_textLineEdit);
    layout->addWidget(find_Btn);
    // ��ʾ�Ի���
    findDlg->show();
    // ����"������һ��"��ť�ĵ����¼�����ۺ����Ĺ���
    connect(find_Btn, SIGNAL(clicked()), this, SLOT(show_findText()));
}

void MainWindow::on_action_Replace_triggered()
{
    // �½�һ���Ի��������滻������this�������ĸ�������MainWindow��
    QDialog *replaceDlg = new QDialog(this);
    replaceDlg->setWindowTitle(tr("�滻"));    // ���öԻ���ı���
    find_textLineEdit = new QLineEdit(replaceDlg);    // ���б༭�����뵽�½����滻�Ի�����
    replace_textLineEdit = new QLineEdit(replaceDlg);
    QPushButton *find_Btn = new QPushButton(tr("������һ��"), replaceDlg);  // ����һ��"������һ��"�İ�ť
    QPushButton *replace_Btn = new QPushButton(tr("�滻"), replaceDlg);
    QVBoxLayout* layout1 = new QVBoxLayout();     // �½�һ����ֱ���ֹ������������б༭���Ͱ�ť��������
    layout1->addWidget(find_textLineEdit);
    layout1->addWidget(replace_textLineEdit);
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget(find_Btn);
    layout2->addWidget(replace_Btn);
    QHBoxLayout* layout = new QHBoxLayout(replaceDlg);   // �½�һ��ˮƽ���ֹ�����
    layout->addLayout(layout1);
    layout->addLayout(layout2);
    replaceDlg ->show();    // ��ʾ�Ի���
    // ����"�滻"��ť�ĵ����¼�����ۺ����Ĺ���
    connect(find_Btn, SIGNAL(clicked()), this, SLOT(show_findText()));
    connect(replace_Btn, SIGNAL(clicked()), this, SLOT(replace_findText()));
}


// "������һ��"��ť�Ĳۺ���
void MainWindow::show_findText()
{
    // ��ȡ�б༭���е�����
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
        QMessageBox::warning(this, tr("����"), tr("δ�ҵ� %1��").arg(findText));
    }
}

//"�滻"��ť�Ĳۺ���
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
        QMessageBox::warning(this, tr("�滻"), tr("���Ȳ��ң�"));
    }
}

// �����ļ����ݣ���Ϊ���ܱ���ʧ�ܣ����Ծ��з���ֵ���������Ƿ񱣴�ɹ�
bool MainWindow::saveFile(const QString& fileName)
{
    QFile file(fileName);

    // ��ֻд��ʽ���ļ��������ʧ���򵯳���ʾ�򲢷���
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        // %1,%2��ʾ���������arg������ֵ
        QMessageBox::warning(this, tr("�����ļ�"),
                             tr("�޷������ļ� %1:\n %2��").arg(fileName).arg(file.errorString()));
        return false;
    }

    // �½�������ָ��ѡ�����ļ�
    QTextStream out(&file);
    // ���ı��༭����������Դ��ı���ʽ�������������
    string all = data.Text();
    out << QString::fromStdString(all);
    isSaved = true;
    ui->textEdit->isModified = false;
    // ����ļ��ı�׼·��
    curFile = QFileInfo(fileName).canonicalFilePath();
    // ���������Ƹ�Ϊ���ڴ��ڵ�·��
    setWindowTitle(curFile);
    bar->showMessage(bar->currentMessage() + tr("    �����ļ��ɹ���"));
    file.close();
    return true;
}

// �ļ����Ϊ
void MainWindow::do_file_SaveAs()
{
    // ����ļ���
    QString fileName = QFileDialog::getSaveFileName(this, tr("���Ϊ"), curFile);

    // ����ļ�����Ϊ�գ��򱣴��ļ�����
    if (!fileName.isEmpty())
    {
        saveFile(fileName);
    }
}

// �����ļ�
void MainWindow::do_file_Save()
{
    // ����ļ��Ѿ����������ֱ�ӱ����ļ�
    if (isSaved)
    {
        saveFile(curFile);
    }
    // ����ļ��ǵ�һ�α��棬��ô�������Ϊ
    else
    {
        do_file_SaveAs();
    }
}

// �����Ƿ񱣴��ļ��Ի���
void MainWindow::do_file_SaveOrNot()
{
    // ����ļ������Ĺ�����������Ի���
    if (ui->textEdit->isModified)
    {
        QMessageBox box;
        box.setWindowTitle(tr("����"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr("��δ���棬�Ƿ񱣴�?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        // ���ѡ�񱣴��ļ�����ִ�б������
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

// ���ļ�
void MainWindow::do_file_Open()
{
    // �Ƿ���Ҫ���������ļ�
    do_file_SaveOrNot();
    // ���Ҫ�򿪵��ļ�������
    QString fileName = QFileDialog::getOpenFileName(this);

    // ����ļ�����Ϊ��
    if (!fileName.isEmpty())
    {
        do_file_Load(fileName);
    }

    // �ı��༭���ɼ�
    ui->textEdit->setVisible(true);
}

// ��ȡ�ļ�
bool MainWindow::do_file_Load(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("��ȡ�ļ�"), tr("�޷���ȡ  �ļ� %1:\n%2��").arg(fileName).arg(file.errorString()));
        // ������ļ�ʧ�ܣ������Ի��򣬲�����
        return false;
    }

    QTextStream in(&file);
    // ���ļ��е��������ݶ�д���ı��༭����
    QString alltext = in.readAll();
    ui->textEdit->setText(alltext);
    data.Load(alltext.toStdString());
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    bar->showMessage(bar->currentMessage() + tr("    ���ļ��ɹ���"));
    file.close();
    return true;
}

// ʵ���½��ļ��Ĺ���
void MainWindow::do_file_New()
{
    do_file_SaveOrNot();
    isSaved = false;
    curFile = tr("δ����.txt");
    setWindowTitle(curFile);
    // ����ı��༭��
    ui->textEdit->clear();
    data.Clear();
    // �ı��༭���ɼ�
    ui->textEdit->setVisible(true);
}

// ��ʾ������Ϣ
void MainWindow::on_action_Author_triggered()
{
    QDialog *about = new QDialog(this);
    about->setWindowTitle(tr("����"));
    QLabel* label_gif = new QLabel(about);
    label_gif->setScaledContents(true);
    QMovie* movie = new QMovie(QString::fromUtf8(":/images/images/cat.gif"));
    movie->start();
    label_gif->setMovie(movie);
    QLabel* label_info = new QLabel(tr("���ߣ����  ����  ������\n���䣺1334527295@qq.com"), about);
    label_info->setScaledContents(true);
    label_info->setAlignment(Qt::AlignCenter);
    QFont ft(tr("΢���ź�"), 13);
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
