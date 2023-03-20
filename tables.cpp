#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

bool** maxTable;
int macCh;

//Обыкновенный вывод таблицы в командную строку.
void tableOutput(bool** table, int n, int m)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            if(table[i][j])
            cout << "- ";
            else
            cout << "+ ";
        }
        cout << endl;
    }
}


bool* changeSigns(bool* raw, int m)
{
    bool* newraw = new bool[m+1];
    for(int i = 0; i < m+1; i++)
        newraw[i] = !raw[i];
    delete[] raw;
    return newraw;
}


//Функция аналогична прибавлению к маске 1.
//Она ищет в ней первое ненулевое число,
//делает его ненулевым, и делает нулевыми
//все предыдущие числа.
int nextValue(bool** table, int m, int n)
{
    for(int i = 0;i < n; i++)
    {
        if(table[i][m] == 0)
        {
            for(int j = i;j >= 0; j--)
            {
                table[j] = changeSigns(table[j], m);
            }
            return 0;
        }  
    }
    return -1;
}

//Функция, которая возвращает производную таблицу
bool** makeDerTable(bool** table, int m, int n)
{
    bool** derTable = new bool*[n-1];
    for(int i = 0; i < n-1; i++)
    {
        derTable[i] = new bool[m+1];
        for(int j = 0; j < m; j++)
        {
            //В производной табилице стоит плюс (0), если соседние числа равны
            //и стоит - (1), если они не равны.
            derTable[i][j] = !(table[i][j] == table[i+1][j]);
        }
        derTable[i][m] = 0;
    }
    return derTable;
}

//Реализация алгоритма восстановления количества минусов в столбце
//из производного столбца
int getAmountFromDer(bool** dtable, int column, int n, int &nhalf)
{
    int c = 1;
    bool isCurrent = true;
    for(int i = 0; i < n-1; i++)
    {
        if(dtable[i][column] == 0 and isCurrent)
        c++;
        else if (dtable[i][column] == 1)
        {
            isCurrent = !isCurrent;
            if(isCurrent == true)
            c++;
        }
    }
    if(c > nhalf)
    c = n-c;

    return c;
}


int findCharacteristic(bool** table, int m, int n, int &valueCount, bool** derTable)
{
    //устанавливаем большое значение характеристики,
    // чтобы далее сравнивать его с гарантированно меньшими значеними*
    int charact = m*n; 
    int nhalf = floor(n/2); 
    int c = 0;
    //переменная valueCounter хранит, сколько раз к маске прибавили 1.
    int valueCounter;
    do
    {
        int amount = 0;
        for(int i = 0; i < m; i++)
        {
            amount += getAmountFromDer(derTable, i, n, nhalf);
        }
        if(amount < charact)
        {
            charact = amount;
            valueCounter = c;
            //Вместе с присвоением переменной charact минимальной 
            //на данный момент характеристики, переменной valueCounter
            //присваивается значение маски 
            //(ибо маска - двоичное число) для этой
            //минимальной характеристики,
            //чтобы потом по маске найти способ получить таблицу с 
            //минимальной характеристкой из исходной
        }
        c++;
    } while (nextValue(derTable, m, n-1) != -1);
    valueCount = valueCounter-1;
    return charact;
}


bool swapValues(int &a, int &b)
{
    if(a > b)
    {
        int temp = b;
        b = a;
        a = temp;
        return true;
    }
    return false;
}

//Функция ввода таблицы через командную строку. (mr и nr - ссылки на переменные m и n вне этой функции)
bool** inputTable(int &mr, int &nr) 
{
    int m, n;
    cout << "n (raws):"; cin >> n;
    cout << "m (columns):"; cin >> m;
    mr = m; nr = n;
    bool** table = new bool*[n];
    for(int i = 0; i < n; i++)
    {
        //m+1 столец таблицы -- ее маска (см. технические моменты)
        table[i] = new bool[m+1];
        for(int j = 0;j < m;j++)
        {
            char t;
            cin >> t;
            if(t == '-')
            table[i][j] = 1;
            else
            table[i][j] = 0;
        }
        table[i][m]=0;

        for(int i = 0; i < m; i++)
        cout << "__";
        cout << endl;
    }
    return table;
}

//Функция возвращает таблицу со случайно заданными знаками
bool** randomTable(int m, int n)
{
    bool** table = new bool*[n];
    for(int i = 0; i < n; i++)
    {
        table[i] = new bool[m+1];
        for(int j = 0;j < m;j++)
        {
            table[i][j] = rand() % 2;
        }
        table[i][m]=0;
    }
    return table;
}

int setTableProgram_()
{
    int m, n;
    int valueCounter;
    bool* mask;
    bool** table = inputTable(m, n);
    bool** derTable = makeDerTable(table, m, n);;
    cout << "CHARACHTERISTIC: " << findCharacteristic(table, m, n, valueCounter, derTable) << endl;
    for(int i = 0; i < n-1;i++)
    {
        derTable[i] = changeSigns(derTable[i], m);
    }
    cout  << "FOR DERRIVATIVE TABLE: \n";
    tableOutput(derTable, n-1, m);
    for(int i = 0; i <= valueCounter; i++)
    {
        nextValue(derTable, m, n-1);
    }
    cout << "WITH MASK:";
    for(int i = 0; i < n-1; i++)
    cout << derTable[i][m];
    cout << endl;
    return 0;
}

int randTableProgram_(int amountOfTables, bool isInput, int mr, int nr, int* gotChar)
{
    int m, n;
    bool** derTable;
    if(isInput)
    {    
    cout << "n(raws):"; cin >> n;
    cout << "m(columns):"; cin >> m;
    }
    else
    {
        m = mr;
        n = nr;
    }

    int valueCounter;
    bool* mask;
    int maxChar = 0;
    for(int q = 0; q < amountOfTables; q++)
    {
        bool** table = randomTable(m, n);
        derTable = makeDerTable(table, m, n);
        int valueCounter;
        int c = findCharacteristic(table, m, n, valueCounter, derTable);
        //cout << q << endl;
        if(c > maxChar)
        {
            maxChar = c;
            maxTable = new bool*[n];
            for(int i = 0; i < n; i++)
            {
                maxTable[i] = new bool[m];
                for(int j = 0; j < m; j++)
                {
                    maxTable[i][j] = table[i][j];
                }
            }
        }
        
    }
    cout << "FOUND TABLE: \n";
    tableOutput(maxTable, n, m);
    if(isInput)
    {
        cout << "WITH CHARACTERISTIC:" << maxChar << endl;
    }
    else
    *gotChar = maxChar;
    return 0;
}


int goThroughTables_(int nmax, int mmax)
{
    ofstream my_file;
	my_file.open("tabledata.txt");
    cout << "CREATED FILE" << endl;
        for(int i = 2; i < mmax; i++)
        {
            my_file << i;
            for(int j = 4; j < nmax; j++) 
            {
                int* charr_ = new int;
                randTableProgram_(3000, false, i, j, charr_);
                int charr = *charr_;
                delete charr_;
                my_file << " " << charr;
                cout << "WENT TRHOUGH  " << i << "x" << j << " TABLES" << endl;
            }  
            my_file << endl;
        }
    my_file.close();
    return 0;
}

int main()
{
    int option;
    cout << "1 GET MAX CHARACTERISTIC OF RANDOM TABLE \n";
    cout << "2 GET MAX CHARACTERISTIC OF INPUT TABLE \n";
    cout << "3 GET FILE WITH MAX CHARACTERISTIC FOR TABLES (from 2 to m)x(from 4 to n)\n"; 
    cin >> option;
    switch (option)
    {
    case 1:
        return randTableProgram_(1000, true, 0, 0, nullptr);
    
    case 2:
        return setTableProgram_();

    case 3:
    {
        int n, m;
        cout << "N:"; cin >> n;
        cout << "M:"; cin >> m;
        return goThroughTables_(n, m);
    }

    default:
        cout << "WRONG INPUT\n";
        break;
    }
    srand(time(NULL));
    
    //return goThroughTables_(10, 70);
    //return setTableProgram_();
}