#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "mytextedit.h"
#include <QCursor>
#include <QTextEdit>
#include <QKeyEvent>
#include <qobject.h>

bool first_find = true;
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
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(do_cursorChanged()));
}

bool MainWindow::eventFilter(QObject* target ,QEvent* event )
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->modifiers() == Qt::ControlModifier)
            {
                switch(keyEvent->key())
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
                //QMessageBox::warning(this, tr("copy"), tr("ctrl c"));
                return true;
            }

        }
    }
    return false;
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
}

// �˳�����
void MainWindow::on_action_Quit_triggered()
{
    // ��ִ�йرղ���
    on_action_Close_triggered();
    // ���˳�ϵͳ��qApp��ָ��Ӧ�ó����ȫ��ָ��
    qApp->quit();

}

// ��������
void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

// ���в���
void MainWindow::on_action_Cut_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(cursor.hasSelection())
    {
        if(data.Cut(cursor.selectionStart(), cursor.selectionEnd()))
        {
            //QString s = "Test";
            //cursor.insertText(s);
            //ui->textEdit->setTextCursor(cursor);
            second_statusLabel->setText(tr("���гɹ���"));
        }
        else
            second_statusLabel->setText(tr("����ʧ�ܣ�"));
    }
    //ui->textEdit->cut();
}

// ���Ʋ���
void MainWindow::on_action_Copy_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();

    if(cursor.hasSelection())
    {
        if(data.Copy(cursor.selectionStart(), cursor.selectionEnd()))  // ���ı���ͷ����,���ұ�
        {
            QString c = QString::fromStdString(data.Clip());
            second_statusLabel->setText(tr("���Ƴɹ���%1").arg(c));
            //cursor.insertText("Hello World");
        }
        else
            second_statusLabel->setText(tr("����ʧ�ܣ�"));

        //        QString s1 = QString::number(cursor.selectionStart(), 10);
        //        QString s2 = QString::number(cursor.selectionEnd(), 10);
        //        QString s = s1 + " to " + s2;
        //        second_statusLabel->setText(s);

    }
    //ui->textEdit->copy();


}

// ճ������
void MainWindow::on_action_Paste_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(data.Paste(cursor.blockNumber(), cursor.columnNumber()))   //�����������������
    {
        QString c = QString::fromStdString(data.Clip());
        cursor.insertText(c);
        //cursor.insertText("test");
        second_statusLabel->setText(tr("ճ���ɹ���%1").arg(c));
    }
    else
        second_statusLabel->setText(tr("ճ��ʧ�ܣ�"));
    //ui->textEdit->paste();
}

void MainWindow::init_statusBar()
{
    // ��ȡ״̬��
    QStatusBar* bar = ui->statusBar;
    // �½���ǩ
    first_statusLabel = new QLabel;
    // ���ñ�ǩ��С�ߴ�
    first_statusLabel->setMinimumSize(150,20);
    // ���ñ�ǩ��״
    first_statusLabel->setFrameShape(QFrame::WinPanel);
    // ���ñ�ǩ��Ӱ
    first_statusLabel->setFrameShadow(QFrame::Sunken);
    second_statusLabel = new QLabel;
    second_statusLabel->setMinimumSize(150,20);
    second_statusLabel->setFrameShape(QFrame::WinPanel);
    second_statusLabel->setFrameShadow(QFrame::Sunken);
    bar->addWidget(first_statusLabel);
    bar->addWidget(second_statusLabel);
    first_statusLabel->setText(tr("��ӭʹ��"));
    //second_statusLabel->setText(tr("�������!"));

}

void MainWindow::do_cursorChanged()
{
    //int rowNum = ui->textEdit->document()->blockCount();
    int rowNum;
    const QTextCursor cursor = ui->textEdit->textCursor();
    // ��ȡ��������е��к�
    int colNum = cursor.columnNumber();
    // ��ȡ��������е��к�
    rowNum = cursor.blockNumber();
    // ��״̬����ʾ���λ��
    first_statusLabel->setText(tr("%1�� %2��").arg(rowNum).arg(colNum));
}


void MainWindow::on_action_Find_triggered()
{
    QTextCursor cur = ui->textEdit->textCursor();
    ui->textEdit->setFocus();
    cur.setPosition(0);

    first_find = true;

    // �½�һ���Ի������ڲ��Ҳ�����this�������ĸ�������MainWindow��
    QDialog *findDlg = new QDialog(this);
    // ���öԻ���ı���
    findDlg->setWindowTitle(tr("����"));
    // ���б༭�����뵽�½��Ĳ��ҶԻ�����
    find_textLineEdit = new QLineEdit(findDlg);
    if(cur.hasSelection())
    {
        find_textLineEdit->setText(cur.selectedText());
    }

    // ����һ��"������һ��"�İ�ť
    QPushButton *find_Btn = new QPushButton(tr("������һ��"),findDlg);

    // �½�һ����ֱ���ֹ������������б༭���Ͱ�ť��������
    QVBoxLayout* layout = new QVBoxLayout(findDlg);
    layout->addWidget(find_textLineEdit);
    layout->addWidget(find_Btn);


    // ��ʾ�Ի���
    findDlg->show();

    // ����"������һ��"��ť�ĵ����¼�����ۺ����Ĺ���
    connect(find_Btn,SIGNAL(clicked()),this,SLOT(show_findText()));
}



void MainWindow::on_action_Replace_triggered()
{
    first_find = true;

    // �½�һ���Ի��������滻������this�������ĸ�������MainWindow��
    QDialog *replaceDlg = new QDialog(this);

    replaceDlg->setWindowTitle(tr("�滻"));    // ���öԻ���ı���

    find_textLineEdit = new QLineEdit(replaceDlg);    // ���б༭�����뵽�½����滻�Ի�����
    replace_textLineEdit = new QLineEdit(replaceDlg);

    QPushButton *find_Btn = new QPushButton(tr("������һ��"),replaceDlg);   // ����һ��"������һ��"�İ�ť
    QPushButton *replace_Btn = new QPushButton(tr("�滻"),replaceDlg);

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
    connect(find_Btn,SIGNAL(clicked()),this,SLOT(show_findText()));
    connect(replace_Btn,SIGNAL(clicked()),this,SLOT(replace_findText()));
}


// "������һ��"��ť�Ĳۺ���
void MainWindow::show_findText()
{

    // ��ȡ�б༭���е�����
    QString findText = find_textLineEdit->text();
    QTextCursor cur = ui->textEdit->textCursor();
    find_pos = cur.position();

    /*
    if(first_find)
    {
        first_find = false;
    }
    if(data.Find(find_pos, findText.toStdString()))
    {
        cur.setPosition(find_pos);
        cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, findText.length());
        ui->textEdit->setTextCursor(cur);
    }
    else
    {
        QMessageBox::warning(this,tr("����"),tr("δ�ҵ� %1").arg(findText));
    }
*/

    if(!ui->textEdit->find(findText,QTextDocument::FindBackward))
    {
        QMessageBox::warning(this,tr("����"),tr("δ�ҵ� %1").arg(findText));
    }
    else
    {
    }
}


//"�滻"��ť�Ĳۺ���
void MainWindow::replace_findText()
{
    QString findText = find_textLineEdit->text();
    QString replaceText = replace_textLineEdit->text();

    QTextCursor cur = ui->textEdit->textCursor();
    //show_findText();
    if(cur.hasSelection())
    {
        data.Replace(cur.selectionStart(), cur.selectionEnd(), replaceText.toStdString());
        cur.removeSelectedText();
        cur.insertText(replaceText);
    }
    else
    {
        QMessageBox::warning(this,tr("�滻"),tr("δ�ҵ� %1").arg(findText));
    }

}

// �����ļ����ݣ���Ϊ���ܱ���ʧ�ܣ����Ծ��з���ֵ���������Ƿ񱣴�ɹ�
bool MainWindow::saveFile(const QString& fileName)
{
    QFile file(fileName);

    // ��ֻд��ʽ���ļ��������ʧ���򵯳���ʾ�򲢷���
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        // %1,%2��ʾ���������arg������ֵ
        QMessageBox::warning(this,tr("�����ļ�"),
                             tr("�޷������ļ� %1:\n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    // �½�������ָ��ѡ�����ļ�
    QTextStream out(&file);
    // ���ı��༭����������Դ��ı���ʽ�������������
    string all = data.Text();
    //string all = "a\ntest";
    out << QString::fromStdString(all);
    //out << ui->textEdit->toPlainText();

    // ����ļ��ı�׼·��
    isSaved = true;
    curFile = QFileInfo(fileName).canonicalFilePath();
    // ���������Ƹ�Ϊ���ڴ��ڵ�·��
    setWindowTitle(curFile);

    second_statusLabel->setText(tr("�����ļ��ɹ�"));
    file.close();

    return true;
}

// �ļ����Ϊ
void MainWindow::do_file_SaveAs()
{
    // ����ļ���
    QString fileName = QFileDialog::getSaveFileName(this,tr("���Ϊ"),curFile);
    // ����ļ�����Ϊ�գ��򱣴��ļ�����
    if(!fileName.isEmpty())
    {
        saveFile(fileName);
    }
}

// �����ļ�
void MainWindow::do_file_Save()
{
    // ����ļ��Ѿ����������ֱ�ӱ����ļ�
    if(isSaved)
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
    if(ui->textEdit->document()->isModified())
    {
        QMessageBox box;
        box.setWindowTitle(tr("����"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr("��δ���棬�Ƿ񱣴�?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        // ���ѡ�񱣴��ļ�����ִ�б������
        if(box.exec() == QMessageBox::Yes)
        {
            do_file_Save();
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
    if(!fileName.isEmpty())
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
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("��ȡ�ļ�"),tr("�޷���ȡ  �ļ� %1:\n%2.").arg(fileName).arg(file.errorString()));
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

    second_statusLabel->setText(tr("���ļ��ɹ�"));

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
    QLabel * p_author = new QLabel();
    p_author->resize(200, 100);
    p_author->setWindowTitle(tr("����"));
    p_author->setText(tr("���ߣ���� ���� ������"));
    p_author->setAlignment(Qt::AlignCenter);
    p_author->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
