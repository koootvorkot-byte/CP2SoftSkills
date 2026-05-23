#include <iostream>
#include <fstream>
#include <clocale>
#include <string>
#include <windows.h>

using namespace std;

struct Cell {
    bool alive = 0;
    short int neighs = 0;
};

void inFile(Cell*** book, int page, int rows, int cols){
    cin.ignore();
    string filename;
    cout << "Введіть назву файлу:";
    getline(cin, filename);
    ofstream outfs(filename);
    if(!outfs){
        cout << "Сталась помикла, зміни не будуть збережені.";
        return;
    } else{
        for(int i = 0; i < page; i++){
            for(int j = 0; j < rows; j++){
                for(int k = 0; k < cols; k++){
                    outfs << (book[i][j][k].alive == 0 ? '.' : '1');
                }
                outfs << endl;
            }
            outfs << "\n\n" << endl;
        }
    }
    outfs.close();
    return;
}

Cell** next(Cell** pole, int rows, int cols){
 Cell** tempField = new Cell * [rows];
    for (int i = 0; i < rows; i++) {
        tempField[i] = new Cell[cols];
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            tempField[i][j].neighs = 0;

            for (int il = i - 1; il <= i + 1; il++) {
                for (int jl = j - 1; jl <= j + 1; jl++) {
                    if (il >= 0 && il < rows && jl >= 0 && jl < cols) {
                        if (!(il == i && jl == j) && pole[il][jl].alive == 1) {
                            tempField[i][j].neighs++;
                        }
                    }
                }
            }

            if (pole[i][j].alive == 1) {
                if (tempField[i][j].neighs < 2 || tempField[i][j].neighs > 3) {
                    tempField[i][j].alive = 0;
                }
                else {
                    tempField[i][j].alive = 1;
                }
            }
            else {
                if (tempField[i][j].neighs == 3) {
                    tempField[i][j].alive = 1;
                }
                else {
                    tempField[i][j].alive = 0;
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pole[i][j] = tempField[i][j];
        }
        delete[] tempField[i];
    }
    delete[] tempField;

    return pole;
}

void Render(Cell** gametble, int rows, int cols) {
    const int maxHistory = 200;
    int currentStep = 0;

    Cell*** history = new Cell **[maxHistory];
    for (int h = 0; h < maxHistory; h++) {
        history[h] = new Cell * [rows];
        for (int i = 0; i < rows; i++) {
            history[h][i] = new Cell[cols];
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            history[0][i][j] = gametble[i][j];
        }
    }

    while (true) {
        ++currentStep;
        int check = 0;
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(gametble[i][j].alive == 0){
                    ++check;
                }
            }
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j< cols; j++){
                cout << (gametble[i][j].alive == 1 ? '1' : '.');
                history[currentStep][i][j] = gametble[i][j];
            }
            cout << endl;
        }
		gametble = next(gametble, rows, cols);
        if(check == rows*cols){
            cout << "Game over" << endl;
            break;
        }
        cout << "\n\n\n\n\n\n\n\n\n" << endl;

        Sleep(700);
    }
    char ans = 0;
    cout << "Would you like to save your grid? (y/n)";
    cin >> ans;
    if(ans == 'y' || ans == 'Y'){
        inFile(history, maxHistory, rows, cols);
    }

    for (int h = 0; h < maxHistory; h++) {
        for (int i = 0; i < rows; i++) {
            delete[] history[h][i];
        }
        delete[] history[h];
    }
    delete[] history;
}

int main() {
    setlocale(LC_ALL, "ua_UK.UTF-8");
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);

    int inrows, incols;

    while (true) {
        cout << "Введіть кількість строк поля: ";
        if (cin >> inrows && inrows > 0) {
            break;
        }
        cout << "Помилка! Введіть коректне число більше 0.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    while (true) {
        cout << "Введіть кількість столбцов поля: ";
        if (cin >> incols && incols > 0) {
            break;
        }
        cout << "Помилка! Введіть коректне число більше 0.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    Cell** fiela = new Cell * [inrows];
    for (int i = 0; i < inrows; i++) {
        fiela[i] = new Cell[incols];
    }

    for (int i = 0; i < inrows; i++) {
        for (int j = 0; j < incols; j++) {
            fiela[i][j].alive = 0;
            fiela[i][j].neighs = 0;
        }
    }

    int initialCells;
    while (true) {
        cout << "Введіть кількість початкових живих клітин (розмір фігури): ";
        if (cin >> initialCells && initialCells >= 0) {
            if (initialCells <= inrows * incols) {
                break;
            }
            cout << "Помилка! Клітин не може бути більше, ніж розмір самого поля (" << inrows * incols << ").\n";
        }
        else {
            cout << "Помилка! Введіть коректне число (0 або більше).\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    for (int k = 0; k < initialCells; k++) {
        int r, c;
        while (true) {
            cout << "Введіть координати клітини " << k + 1 << " (строка та стовбець через пробіл от 1 до максимуму): ";
            if (cin >> r >> c) {
                if (r >= 1 && r <= inrows && c >= 1 && c <= incols) {
                    fiela[r - 1][c - 1].alive = 1;
                    break;
                }
                else {
                    cout << "Помилка! Вихід за межі поля. Спробуйте ще раз.\n";
                }
            }
            else {
                cout << "Помилка! Вводьте тільки числа.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
    }

    Render(fiela, inrows, incols);

    for (int i = 0; i < inrows; i++) {
        delete[] fiela[i];
    }
    delete[] fiela;

    return 0;
}