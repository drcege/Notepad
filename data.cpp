#include "data.h"
#include <string>
#include <list>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;
//---------------------------���캯��-----------------------------------------//
Data::Data()
{
    firstline = NULL;
    clip = "";
}
//----------------------------------------------------------------------------//

//---------------------------��������-----------------------------------------//
Data::~Data()
{
}
//----------------------------------------------------------------------------//

//--------------------------���ĵ�------------------------------------------//
bool Data::Load(string s)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int i = 0;//��¼�ַ����±�
    int t = 0;//��¼��ǰ�����ַ�����
    bool change_line = true;//�ж��Ƿ���

    if (s[i] != '\0')//�ǿ��ĵ�����ͷָ��
    {
        firstline = new Line;
        firstline->nextBlock = NULL;
        firstline->nextLine = NULL;
        currentline = firstline;
    }

    while (s[i] != '\0')//���α���ÿһ���ַ�
    {
        if (change_line)//���л��в������½�һ�У�����ʼ��
        {
            change_line = false;
            currentline->nextBlock = new Block;
            currentblock = currentline->nextBlock;
            currentblock->L = 0;
            currentblock->nextBlock = NULL;
        }
        else if (t == 0)//��ǰ��Ϊ�µĿ飬�½��飬��ʼ��
        {
            Data::create_block(currentblock);
            currentblock = currentblock->nextBlock;
        }

        currentblock->block[currentblock->L] = s[i];//����ǰ�ַ��������
        currentblock->L++;//����Ԫ��������һ
        t++;//��ǰ����Ԫ��������һ

        if (s[i] == '\n')//�����س���
        {
            t = 0;//������ʹ�γ��µĿ�
            change_line = true;//�������б�־

            if (s[i + 1] != '\0')//����Ϊ�ļ�ĩβ�Ļس�������л��в���
            {
                Data::create_line(currentline);//�½�һ�У���ʼ��
                currentline = currentline->nextLine;//����ָ��
            }
        }
        else if (t == 80)//��ǰ��������
        {
            t = 0;//�γ��¿�
        }

        i++;//�ַ��������±��ۼ�
    }

    return true;
}
//----------------------------------------------------------------------------//

//----------------------------��������----------------------------------------//
bool Data::Copy(int beg, int end)
{
    int sl = 0, sc = 0, el = 0, ec = 0;//�ֱ�洢��ʼλ���кţ��кţ�����λ���к��к�
    find_pos(beg, sl, sc);//��ÿ�ʼλ��
    find_pos(end - 1, el, ec); //��ý���λ��
    Line *currentline = get_line(sl); //��ÿ�ʼλ�õ���ָ��
    clip = "";//���а����
    Block *currentblock = get_block(currentline, sc); //��Ŀ�ʼλ�õĿ�ָ��

    if (sl == el) //���Ƶ�������ͬһ��
    {
        int t = get_block_pos(sc); //��ÿ�ʼλ���ڿ��ڵ��±�

        for (int i = sc; i <= ec; i++) //�������
        {
            clip += currentblock->block[t];
            t++;//�ж��Ƿ��ѱ��������һ��λ��

            if (t == 80) //���������һ��λ���������һ����
            {
                t = 0;
                currentblock = currentblock->nextBlock;
            }
        }
    }
    else//Ҫ���Ƶ����ݿ���
    {
        int length = 0;
        clip += copy_line(currentblock, sc, length); //�ֽ���һ�д���ʼ��ĩβ���Ƶ����а���
        currentline = currentline->nextLine; //ָ��������һ��
        currentblock = currentline->nextBlock; //��ָ��ֻ����һ�еĵ�һ����

        for (int i = sl + 1; i <= (el - 1); i++) //�����м�������Ҫ���ƵĲ���
        {
            while (currentblock != NULL) //����ÿ����
            {
                for (int j = 0; j < currentblock->L; j++)
                {
                    clip += currentblock->block[j];    //���ƿ�����������
                }

                currentblock = currentblock->nextBlock; //������һ��
            }

            currentline = currentline->nextLine; //������һ��
            currentblock = currentline->nextBlock; //������ָ��
        }

        int t = 0; //�жϵ�ǰ����Ԫ���Ƿ�ȫ��������

        for (int i = 0; i <= ec; i++) //���ƽ����е�Ԫ��
        {
            clip += currentblock->block[t];
            t++;

            if (t == 80) //��ǰ���Ѹ����꣬������һ��
            {
                t = 0;
                currentblock = currentblock->nextBlock;
            }
        }
    }

    return true;
}
//----------------------------------------------------------------------------//

//------------------------���ҵ�posλ��Ԫ����������---------------------------//
void Data::find_pos(int pos, int &x, int &y)
{
    Line *currentline = firstline;
    Block *currentblock = firstline->nextBlock;
    int t = 0;//��¼�ѱ������ٸ�Ԫ��
    int before_t = 0;//��¼��ֹ����һ�й��ж��ٸ�Ԫ��
    pos++;
    bool flag = true;

    while (flag)
    {
        while (currentblock != NULL)//��ǰ��ǿգ�����
        {
            t += currentblock->L;//�ۼ�

            if (t >= pos)//���ҵ�
            {
                y = pos - before_t - 1;//�õ��к�
                flag = false;
                break;
            }

            currentblock = currentblock->nextBlock;//������һ��
        }

        if (!flag)
        {
            break;
        }

        if (currentline->nextLine == NULL) //���У���ǰλ��Ϊ�ı����һ��λ��
        {
            y = t + 1;
            break;
        }

        x++;//������һ��
        before_t = t;//����
        currentline = currentline->nextLine;
        currentblock = currentline->nextBlock;
    }
}
//----------------------------------------------------------------------------//

//---------------------------���к���-----------------------------------------//
bool Data::Cut(int beg, int end)
{
    bool flag = Copy(beg, end); //����Ҫ���е�����
    int l = 0, c = 0;
    find_pos(beg, l, c); //�õ���ʼλ�õ��к��к�

    for (int i = beg; i < end; i++)
    {
        flag = flag && Delete(l, c); //���ɾ��Ҫ���е�Ԫ��
    }

    return flag;
}
//----------------------------------------------------------------------------//

//-----------------------------------ճ������---------------------------------//
bool Data::Paste(int l, int c)
{
    int i = 0; //��¼���а���Ԫ���±�
    bool flag = true;

    while (clip[i] != '\0') //���а�ǿ�1
    {
        if (clip[i] == '\n') //���а嵱ǰԪ��Ϊ�س���
        {
            flag = flag && Enter(l, c); //����س�
            l++;//�����к�
            c = 0; //�����к�
        }
        else
        {
            string s = "";
            s += clip[i];
            flag = flag && Update(s, l, c); //�ڵ�ǰλ�ÿ�����ǰԪ��
            c++;
        }

        i++;//�±����
    }

    return flag;
}
//----------------------------------------------------------------------------//

//----------------------------���Һ���----------------------------------------//
bool Data::Find(int &pos, string s)
{
    string CLIP = "";//���ڴ�Ŵ����Ҵ�
    int l = 0, c = 0;//��¼�к��к�
    int length = 0;
    int POS = pos;//��¼����λ��
    string ::size_type r = 0;//���Һ�������ֵ
    find_pos(pos, l, c);//��Ϊ��ʼλ������
    Line *currentline = NULL;
    Block *currentblock = NULL;
    currentline = get_line(l);//��λ��ʼλ����ָ��

    while (currentline != NULL)
    {
        currentblock = get_block_no_create(currentline, c);//��λ��ָ��
        CLIP = "";

        if (currentblock != NULL)
        {
            CLIP = copy_line(currentblock, c, length);    //����һ��֮��Ĳ��ָ��Ƶ�CLIP�У��������в���
        }

        if (CLIP == "")
        {
            r = string::npos;    //�մ�
        }
        else
        {
            r = CLIP.find(s, 0);    //����
        }

        if (r != string::npos)//�ҵ�������pos����
        {
            POS += int(r);
            pos = POS;
            return true;
        }

        POS = Data::find_start(l);//δ�ҵ�������POS
        c = 0;//����һ�е�0�п�ʼ
        l++;//��һ��
        length = 0;
        currentline = currentline->nextLine;
    }

    //���ı�ĩβ��δ�ҵ����򷵻ؿ�ͷ��ʼ���ң�����ͨͬ�ϲ�����ͬ
    c = 0;
    l = 0;
    POS = 0;
    currentline = firstline;

    while (POS < pos)
    {
        currentblock = get_block_no_create(currentline, c);
        CLIP = "";

        if (currentblock != NULL)
        {
            CLIP = copy_line(currentblock, c, length);
        }

        if (CLIP == "")
        {
            r = string::npos;
        }
        else
        {
            r = CLIP.find(s, 0);
        }

        if (r != string::npos)
        {
            POS += int(r);
            pos = POS;
            return true;
        }

        POS = find_start(l);
        c = 0;
        l++;
        length = 0;
        currentline = currentline->nextLine;
    }

    return false;
}
//----------------------------------------------------------------------------//

//-------------------�ӵ�0�е���l��Ԫ�ظ���-----------------------------------//
int Data::find_start(int l)
{
    int s = 0;
    Line *currentline = firstline;
    Block *currentblock = currentline->nextBlock;

    for (int i = 0; i <= l; i++)//����ÿһ��
    {
        currentblock = currentline->nextBlock;

        while (currentblock != NULL)//������ǰ��ÿһ��
        {
            s += currentblock->L;
            currentblock = currentblock->nextBlock;
        }

        currentline = currentline->nextLine;
    }

    return s;
}
//----------------------------------------------------------------------------//

//---------------------------------�滻����-----------------------------------//
bool Data::Replace(int beg, int end, string s)
{
    int l = 0, c = 0;
    bool flag = true;
    find_pos(beg, l, c);//�õ��滻����ʼλ�����к�

    for (int i = beg; i < end; i++)//�Ƚ����滹����ɾ��
    {
        flag = flag && Delete(l, c);
    }

    if (s != "")//�ǿ���ճ��
    {
        string CLIP = clip;
        clip = s;
        flag = flag && Paste(l, c);
        clip = CLIP;
    }

    return flag;
}
//----------------------------------------------------------------------------//

//--------------------------ɾ������------------------------------------------//
bool Data::Delete(int l, int c)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);//�õ���ǰ��
    currentblock = get_block(currentline, c);//�õ���ǰ��
    int C = get_block_pos(c);//�õ�����λ��

    if (currentblock->nextBlock == NULL && currentline->nextLine == NULL && currentblock->L == (C + 1))//ɾ��λ��Ϊ�ı�ĩβ
    {
        if (currentblock->L != 0)
        {
            currentblock->L--;
        }

        return true;
    }

    if (currentblock->nextBlock == NULL && currentblock->L == (C + 1))//ɾ��λ��Ϊ��ĩ�س���
    {
        Line *temp = currentline->nextLine;//��ʱָ������һ�е�ַ
        int length = 0;
        string CLIP = "";

        if (temp->nextBlock != NULL)
        {
            CLIP = copy_line(temp->nextBlock, 0, length);    //������һ��
        }

        currentblock->L = C; //������ǰ����Ԫ�ظ���
        insert_line(currentblock, CLIP, length);//����һ�в���
        currentline->nextLine = temp->nextLine;//����ָ��
    }
    else//����ɾ��
    {
        int length = 0;
        Block *temp = get_block(currentline, c + 1);//�õ������ַ���
        string CLIP = copy_line(temp, c + 1, length); //���������ַ�
        delete_block(currentblock->nextBlock);//�ͷ��ڴ�
        currentblock->nextBlock = NULL; //��ʼ��
        (currentblock->L) = C; //����
        insert_line(currentblock, CLIP, length);//���²���
    }

    return true;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
bool Data::Backspace(int l, int c)
{
    Line *currentline = NULL;

    if (l == 0 && c == 0)//λ�����ı��ʼ����Ч����
    {
        return true;
    }

    if (c != 0)//����ת��Ϊdelete
    {
        return Data::Delete(l, c - 1);
    }

    //����backspace����Ҫ�ϲ�����
    l--;
    currentline = get_line(l);
    int C = 0;

    for (Block* temp = currentline->nextBlock; temp != NULL; temp = temp->nextBlock)
    {
        C += temp->L;
    }

    return Data::Delete(l, C - 1);
}
//----------------------------------------------------------------------------//

//-------------------------------�س�����-------------------------------------//
bool Data::Enter(int l, int c)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int length = 0;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    string CLIP = copy_line(currentblock, c, length);//���ƻس�����������
    delete_block(currentblock->nextBlock);//�ͷ��ڴ�
    currentblock->nextBlock = NULL;
    currentblock->L = get_block_pos(c) + 1; //��������Ԫ������
    currentblock->block[currentblock->L - 1] = '\n';//��ӻس���
    Line *temp = currentline->nextLine;
    currentline->nextLine = NULL;
    create_line(currentline);
    currentline = currentline->nextLine;
    currentline->nextLine = temp;
    currentblock = currentline->nextBlock;

    if (currentblock == NULL && length != 0)//�ı�ĩβ�س�����Ҫ�����µ���
    {
        currentline->nextBlock = new Block;
        currentblock = currentline->nextBlock;
        currentblock->L = 0;
        currentblock->nextBlock = NULL;
    }

    insert_line(currentblock, CLIP, length);//�����������
    return true;
}
//----------------------------------------------------------------------------//

//---------------------------------���º���-----------------------------------//
bool Data::Update(string s, int l, int c)
{
    int length = 0;
    string CLIP = "";
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    CLIP = copy_line(currentblock, c, length);//�������λ�õĺ�������
    delete_block(currentblock->nextBlock);//�ͷ��ڴ�
    currentblock->nextBlock = NULL;
    currentblock->L = get_block_pos(c);//��������
    CLIP = s + CLIP;//��������

    if (length == 0)//��������Ϊ��
    {
        CLIP = s;
    }

    length++;
    insert_line(currentblock, CLIP, length);//��ȫ�����ݲ���
    return true;
}
//----------------------------------------------------------------------------//

//------------------------------------�ͷź���--------------------------------//
bool Data::Clear()
{
    delete_line(firstline);
    firstline = NULL;
    clip = "";
    return true;
}
//----------------------------------------------------------------------------//

//-----------------------------------�ͷ���-----------------------------------//
void Data::delete_line(Line *currentline)
{
    if (currentline != NULL)//���ݷ��ͷ�
    {
        delete_block(currentline->nextBlock);
        delete_line(currentline->nextLine);
        delete(currentline);
    }
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
string Data::Clip()                          //���ؼ��а�����
{
    return clip;
}
//----------------------------------------------------------------------------//

//--------------------------------�����ı�����--------------------------------//
string Data::Text()
{
    string CLIP;

    //���α���ÿ��û�У�����һ���ַ���
    for (Line *currentline = firstline; currentline != NULL; currentline = currentline->nextLine)
    {
        for (Block *currentblock = currentline->nextBlock; currentblock != NULL; currentblock = currentblock->nextBlock)
        {
            for (int i = 0; i < currentblock->L; ++i)
            {
                CLIP += currentblock->block[i];
            }
        }
    }

    return CLIP;
}
//----------------------------------------------------------------------------//

//---------------------------�½���-------------------------------------------//
void Data::create_line(Line * &currentline)
{
    currentline->nextLine = new Line;
    currentline->nextLine->nextBlock = NULL;
    currentline->nextLine->nextLine = NULL;
}
//----------------------------------------------------------------------------//

//---------------------------�õ���l�е�ָ��----------------------------------//
Line* Data::get_line(int l)
{
    if (firstline == NULL)
    {
        firstline = new Line;
        firstline->nextLine = NULL;
        firstline->nextBlock = NULL;
    }

    Line *currentline = firstline;

    for (int i = 0; i < l; i++)
    {
        if (currentline->nextLine == NULL)
        {
            create_line(currentline);
        }

        currentline = currentline->nextLine;
    }

    return currentline;
}
//----------------------------------------------------------------------------//

//------------------------��������--------------------------------------------//
string Data::copy_line(Block *currentblock, int beg, int &le)
{
    string CLIP = "";
    beg = get_block_pos(beg);//�õ�����λ��

    for (int i = beg; i < currentblock->L; i++)//���ȸ��Ƶ�ǰ��������
    {
        le++;
        CLIP += currentblock->block[i];
    }

    currentblock = currentblock->nextBlock;

    while (currentblock != NULL)//���ƺ������������
    {
        for (int i = 0; i < currentblock->L; i++)
        {
            le++;
            CLIP += currentblock->block[i];
        }

        currentblock = currentblock->nextBlock;
    }

    return CLIP;
}
//----------------------------------------------------------------------------//

//------------------------�ڵ�ǰ������CLIP----------------------------------//
void Data::insert_line(Block *currentblock, string CLIP, int length)
{
    int i = 0;

    while (i < length)//�������
    {
        if (currentblock->L == 80)//��ǰ�����������½�
        {
            if (currentblock->nextBlock == NULL)
            {
                create_block(currentblock);
            }

            currentblock = currentblock->nextBlock;
        }

        currentblock->block[currentblock->L] = CLIP[i];//����
        currentblock->L++;//����
        i++;
    }
}
//----------------------------------------------------------------------------//

//--------------------------����������----------------------------------------//
void Data::create_block(Block * &currentblock)
{
    currentblock->nextBlock = new Block;
    currentblock->nextBlock->L = 0;
    currentblock->nextBlock->nextBlock = NULL;
}
//----------------------------------------------------------------------------//

//-------------------------�õ���ǰ���ַ-------------------------------------//
Block* Data::get_block(Line *currentline, int c)
{
    Block *currentblock = currentline->nextBlock;

    if (currentline->nextBlock == NULL)
    {
        currentline->nextBlock = new Block;
        currentblock = currentline->nextBlock;
        currentblock->L = 0;
        currentblock->nextBlock = NULL;
    }

    for (int i = 79; i < c; i += 80)//��������
    {
        if (currentblock->nextBlock == NULL)
        {
            create_block(currentblock);
        }

        currentblock = currentblock->nextBlock;
    }

    return currentblock;
}
//----------------------------------------------------------------------------//

//----------------�õ���ǰ���ַ�����Ҳ������½�����--------------------------//
Block* Data::get_block_no_create(Line *currentline, int c)
{
    Block *currentblock = currentline->nextBlock;

    if (currentblock == NULL)
    {
        return NULL;
    }

    for (int i = 79; i < c; i += 80)
    {
        if (currentblock->nextBlock == NULL)
        {
            return NULL;
        }

        currentblock = currentblock->nextBlock;
    }

    return currentblock;
}
//----------------------------------------------------------------------------//

//--------------------------�����ͷ�һ���е�ȫ����----------------------------//
void Data::delete_block(Block *currentblock)
{
    if (currentblock != NULL)
    {
        delete_block(currentblock->nextBlock);
        delete(currentblock);
    }
}
//----------------------------------------------------------------------------//

//-------------------------------�õ�����λ��---------------------------------//
int Data::get_block_pos(int x)
{
    while (x >= 80)
    {
        x -= 80;
    }

    return x;
}
//----------------------------------------------------------------------------//
