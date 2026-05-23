#include <iostream>
#include <windows.h> 

using namespace std;

struct Cell {
    bool alive = 0;
    short int neighs = 0;
};

Cell** next(Cell** pole, int rows, int cols) {
    Cell** tempField = new Cell * [rows];
    for (int i = 0; i < rows; i++) {
        tempField[i] = new Cell[cols];
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int live_neighs = 0;
            for (int il = i - 1; il <= i + 1; il++) {
                for (int jl = j - 1; jl <= j + 1; jl++) {
                    if (il >= 0 && il < rows && jl >= 0 && jl < cols) {
                        if (!(il == i && jl == j) && pole[il][jl].alive == 1) {
                            live_neighs++;
                        }
                    }
                }
            }

            tempField[i][j].neighs = live_neighs;
            if (pole[i][j].alive == 1) {
                if (live_neighs < 2 || live_neighs > 3) {
                    tempField[i][j].alive = 0;
                }
                else {
                    tempField[i][j].alive = 1;
                }
            }
            else {
                if (live_neighs == 3) {
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
    const int maxHistory = 60;
    int currentStep = 0;
    bool movingForward = true;
    Cell*** history = new Cell * *[maxHistory];
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

    Cell** NxtFrame = new Cell * [rows];
    for (int i = 0; i < rows; i++) {
        NxtFrame[i] = new Cell[cols];
    }

    while (true) {
        system("cls");

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << (gametble[i][j].alive == 1 ? '1' : '.');
            }
            cout << endl;
        }

        cout << "\nКрок: " << currentStep << " / " << maxHistory - 1;
        if (movingForward) {
            cout << " [ Процес: ВПЕРЕД ]" << endl;
        }
        else {
            cout << " [ Процес: НАЗАД (Зворотній) ]" << endl;
        }

        Sleep(150);

        if (movingForward) {
            if (currentStep >= maxHistory - 1) {
                movingForward = false;
                cout << "\n--- РЕВЕРС! Запуск зворотнього процесу ---" << endl;
                Sleep(1000);
                continue;
            }

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    NxtFrame[i][j] = gametble[i][j];
                }
            }

            NxtFrame = next(NxtFrame, rows, cols);
            currentStep++;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    history[currentStep][i][j] = NxtFrame[i][j];
                    gametble[i][j] = NxtFrame[i][j];
                }
            }
        }
        else {
            if (currentStep <= 0) {
                movingForward = true;
                cout << "\n--- Початок достигнуто! Запуск спочатку ---" << endl;
                Sleep(1000);
                continue;
            }

            currentStep--;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    gametble[i][j] = history[currentStep][i][j];
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        delete[] NxtFrame[i];
    }
    delete[] NxtFrame;

    for (int h = 0; h < maxHistory; h++) {
        for (int i = 0; i < rows; i++) {
            delete[] history[h][i];
        }
        delete[] history[h];
    }
    delete[] history;
}
void inFile(Cell*** History, int tbles, int rows, int cols) {


};
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

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
        cout << "Введіть кількість столбців поля: ";
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