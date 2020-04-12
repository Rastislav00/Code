// MineSweper.cpp 

#include "Table.h"

int main()
{
    srand(time(NULL));

    
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    /*
        10 zelena
        12 crvena
        13 ljubicasta
        14 zuta
        7 standard
    */


    int N, M, B;
    cout << "Dobro dosli u MineSwaper igru" << endl;
    /*
    cout << "Unesite broj redova N: "; cin >> N;
    cout << "Unesite broj kolona M: "; cin >> M;
    cout << "Unesite broj bombi B: "; cin >> B;

    if (N < 8 || N>52) N = 20;
    if (M < 8 || M>52) M = 20;
    if (B > (N * M) || B < 1) B = (N*M)/10;
    */

    cout << "Igra se igra tako sto se unose 3 komande koje je potrebno odvojiti jednim spejsom" << endl;
    cout << "Prva komanda je kordinata oznacena sa slovima" << endl;
    cout << "Druga komanda je kordinata oznacena sa brojevima" << endl;
    cout << "Trece komanda je opcija kojom setujemo polje:"<<endl; 
    cout << "S - Otvori polje " << endl;
    cout << "B - oznaci potencionalnu bombu" << endl;
    cout << "O - iskljuci oznaku za potencionalnu bombu " << endl << endl;

    cout << "Izaberi tezinu: " << endl;
    cout << "1. 10x10 Najlakse" << endl;
    cout << "2. 20x20 Srednje" << endl;
    cout << "3. 30x20 Najteze" << endl;
    cout << "-> ";
    int broj;
    cin >> broj;
    switch (broj) {
    case 3:
        N = 20; M = 30; B = 100;
        break;
    case 2:
        N = 20; M = 20; B = 40;
        break;
    default:
        N = 10; M = 10; B = 10;
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);


    Tabla tabla(N, M, B);
    tabla.print();

    string X;
    string Y;
    string OP;
    

    bool GAME = true;
    while (GAME) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << endl << "Unesi komandu: X Y Z     za kraj uneti end" << endl;
        //cin >> X >> Y >> OP;
        cin >> X; if (X == "end") break;
        cin >> Y; if (Y == "end") break;
        cin >> OP; if (OP == "end") break;
        GAME = tabla.interpret(X,Y,OP);
        tabla.print();
    }
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << endl << "Kraj igre" << endl;
    if (tabla.is_win()) {
        cout << "Pobedili ste" << endl;
    }
    else {
        cout << "Na zalost izgubili ste" << endl;
    }

    Sleep(5000);
    tabla.printHidenTable();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    char c;
    cin >> c;
    return 0;
}





