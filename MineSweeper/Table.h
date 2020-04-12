#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>

using namespace std;

char BOMBA = 'X';
char PRAZNO = 'O';
char TEMP_BOMBA = 'B';

class Poz {		// pomocno klasa pozicija
public:
	int i, j;
	bool test;
	Poz(int i, int j, bool test = false) : i(i), j(j), test(test){}
};

bool postojiEl(vector<Poz>::iterator pocetak, vector<Poz>::iterator kraj, Poz traziPoz) {
	for (auto it = pocetak; it != kraj; it++) {
		if (it->i == traziPoz.i && it->j == traziPoz.j) {
			return true;
		}
	}
	return false;
}



class Tabla {
private:
	char **tabla;
	char** ispodTable;
	int N, M, brBOMBI;
	bool WIN;			// true ako pobedite igru, false je podrazumevana vrednost na pocetku

public:
	Tabla(int n, int m, int brBomb):N(n),M(m),brBOMBI(brBomb){
		WIN = false;
		tabla = new char* [N];
		ispodTable = new char* [N];
		for (int i = 0; i < N; i++) {
			tabla[i] = new char[M];
			ispodTable[i] = new char[M];
		}
		init(brBOMBI);
	}
	~Tabla() {
		for (int i = 0; i < N; i++) {
			delete[] tabla[i];
			delete[] ispodTable[i];
		}
		delete[] tabla;
		delete[] ispodTable;
	}

	void init(int brBomb) {
		// Setovanje nula
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				tabla[i][j] = PRAZNO;
				ispodTable[i][j] = PRAZNO;
			}
		}

		// Setovanje bombi u nevidljiv deo
		for (int i = 0; i < brBomb; i++) {
			ispodTable[ rand()%N ][ rand()%M ] = BOMBA;
		}

		// Setovanje brojeva u svako polje, koje predstavlja broj bombi u okolini 3x3 okolini
		int brBombiUOkolini = 0;

		// Za levi gornji kut
		if (ispodTable[0][0] != BOMBA) {
			brBombiUOkolini = 0;
			if (ispodTable[0][1] == BOMBA) brBombiUOkolini++;
			if (ispodTable[1][1] == BOMBA) brBombiUOkolini++;
			if (ispodTable[1][0] == BOMBA) brBombiUOkolini++;

			if (brBombiUOkolini >= 0) {
				ispodTable[0][0] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
			}
		}

		// Za levi donji kut
		if (ispodTable[N - 1][0] != BOMBA) {
			brBombiUOkolini = 0;
			if (ispodTable[N - 1][1] == BOMBA) brBombiUOkolini++;
			if (ispodTable[N - 2][1] == BOMBA) brBombiUOkolini++;
			if (ispodTable[N - 2][0] == BOMBA) brBombiUOkolini++;

			if (brBombiUOkolini >= 0) {
				ispodTable[N - 1][0] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
			}
		}

		// Za desni gornji kut
		if (ispodTable[0][M - 1] != BOMBA) {
			brBombiUOkolini = 0;
			if (ispodTable[0][M - 2] == BOMBA) brBombiUOkolini++;
			if (ispodTable[1][M - 2] == BOMBA) brBombiUOkolini++;
			if (ispodTable[1][M - 1] == BOMBA) brBombiUOkolini++;

			if (brBombiUOkolini >= 0) {
				ispodTable[0][M - 1] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
			}
		}

		// Za desni donji kut
		if (ispodTable[N - 1][M - 1] != BOMBA) {
			brBombiUOkolini = 0;
			if (ispodTable[N - 1][M - 2] == BOMBA) brBombiUOkolini++;
			if (ispodTable[N - 2][M - 2] == BOMBA) brBombiUOkolini++;
			if (ispodTable[N - 2][M - 1] == BOMBA) brBombiUOkolini++;

			if (brBombiUOkolini >= 0) {
				ispodTable[N - 1][M - 1] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
			}
		}


		for (int k = 1; k < N - 1; k++) {
			// Za levu stranu
			if (ispodTable[k][0] != BOMBA) {
				brBombiUOkolini = 0;
				if (ispodTable[k - 1][0] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k - 1][1] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k][1] == BOMBA)		brBombiUOkolini++;
				if (ispodTable[k + 1][0] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k + 1][1] == BOMBA)	brBombiUOkolini++;

				if (brBombiUOkolini >= 0) {
					ispodTable[k][0] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
				}
			}	

			// Za desnu srtanu
			if (ispodTable[k][M - 1] != BOMBA) {
				brBombiUOkolini = 0;
				if (ispodTable[k - 1][M - 1] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k - 1][M - 2] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k][M - 2] == BOMBA)		brBombiUOkolini++;
				if (ispodTable[k + 1][M - 1] == BOMBA)	brBombiUOkolini++;
				if (ispodTable[k + 1][M - 2] == BOMBA)	brBombiUOkolini++;

				if (brBombiUOkolini >= 0) {
					ispodTable[k][M - 1] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
				}
			}
		}

		for (int k = 1; k < M - 1; k++) { 
			// Za gornju stranu
			if (ispodTable[0][k] != BOMBA) {
				brBombiUOkolini = 0;
				if (ispodTable[0][k - 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[1][k - 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[1][k] == BOMBA) brBombiUOkolini++;
				if (ispodTable[1][k + 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[0][k + 1] == BOMBA) brBombiUOkolini++;

				if (brBombiUOkolini >= 0) {
					ispodTable[0][k] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
				}
			}

			// Za donju stranu
			if (ispodTable[N - 1][k] != BOMBA) {
				brBombiUOkolini = 0;
				if (ispodTable[N - 1][k - 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[N - 2][k - 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[N - 2][k] == BOMBA) brBombiUOkolini++;
				if (ispodTable[N - 2][k + 1] == BOMBA) brBombiUOkolini++;
				if (ispodTable[N - 1][k + 1] == BOMBA) brBombiUOkolini++;

				if (brBombiUOkolini >= 0) {
					ispodTable[N - 1][k] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
				}
			}
		}

		// Za unutrasnjost
		for (int i = 1; i < N-1; i++) {
			for (int j = 1; j < M-1; j++) {
				if (ispodTable[i][j] != BOMBA) {
					brBombiUOkolini = 0;
					if (ispodTable[i - 1][j - 1] == BOMBA)	brBombiUOkolini++;
					if (ispodTable[i - 1][j] == BOMBA)		brBombiUOkolini++;
					if (ispodTable[i - 1][j + 1] == BOMBA)	brBombiUOkolini++;
					if (ispodTable[i][j - 1] == BOMBA)		brBombiUOkolini++;
					if (ispodTable[i][j + 1] == BOMBA)		brBombiUOkolini++;
					if (ispodTable[i + 1][j - 1] == BOMBA)	brBombiUOkolini++;
					if (ispodTable[i + 1][j] == BOMBA)		brBombiUOkolini++;
					if (ispodTable[i + 1][j + 1] == BOMBA)	brBombiUOkolini++;
					
					if (brBombiUOkolini >= 0) {
						ispodTable[i][j] = char(brBombiUOkolini + 48);	// +48 jer je u ASCII 48 = '0'
					}
						
				}
			}
		}

	}

	void printHidenTable() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		system("cls");
		cout << "\t";
		char c = 'A';
		for (int i = 0; i < M; i++) {
			if (c == 91) c = 97;
			cout << c++ << " ";
		}
		cout << endl << endl;

		for (int i = 0; i < N; i++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << i << "\t";
			for (int j = 0; j < M; j++) {
				if (ispodTable[i][j] == '0') {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					cout << ispodTable[i][j] << " ";
				}
				else if (ispodTable[i][j] == PRAZNO) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					cout << ispodTable[i][j] << " ";
				}
				else if (ispodTable[i][j] == TEMP_BOMBA) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
					cout << ispodTable[i][j] << " ";
				}
				else if (ispodTable[i][j] == BOMBA) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					cout << ispodTable[i][j] << " ";
				}
				else {	// slucaj da ne neki broj
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					cout << ispodTable[i][j] << " ";
				}
				//cout << ispodTable[i][j] << " ";
			}
			cout << endl;
		}
	}

	void print() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		system("cls");
		cout << "\t";
		char c = 'A';
		for (int i = 0; i < M; i++) {
			if (c == 91) c = 97;
			cout << c++ << " ";
		}
		cout << endl << endl;

		for (int i = 0; i < N; i++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << i << "\t";
			for (int j = 0; j < M; j++) {
				if (tabla[i][j]=='0') {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					cout << tabla[i][j] << " ";
				}
				else if (tabla[i][j] == PRAZNO) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					cout << tabla[i][j] << " ";
				}
				else if (tabla[i][j] == TEMP_BOMBA) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
					cout << tabla[i][j] << " ";
				}
				else if (tabla[i][j] == BOMBA) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					cout << tabla[i][j] << " ";
				}
				else {	// slucaj da ne neki broj
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					cout << tabla[i][j] << " ";
				}
				//cout << tabla[i][j] << " ";
			}
			cout << endl;
		}
	}

	bool is_win() { return WIN; }

	bool testWin() {
		/*
			Prodje kroz celu tablu i ako nema vise slobodnih mesta kraj igre pobedili smo
		*/
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (ispodTable[i][j] == BOMBA) {		// trazimo bombe
					if (tabla[i][j] != TEMP_BOMBA) {	// gledamo dal je korisnik stavio tempBomb na to mesto ako nije onda nije kraj igrice
						return false;
					}
					
				}
			}
		}
		WIN = true;
		return true;
	}

	void TESTIRAJ(vector<Poz>::iterator itPOZ, vector<Poz>& NP) {
		int x = itPOZ->i;
		int y = itPOZ->j;
		
		if (!itPOZ->test) {	// ako nije testirano, trreba testirati
			itPOZ->test = true;
			
			if (x - 1 >= 0) {
				if (ispodTable[x - 1][y] != BOMBA) {
					Poz tempPoz(x - 1, y);
					if (ispodTable[x - 1][y] != '0') tempPoz.test = true;
					if (!postojiEl(NP.begin(), NP.end(), tempPoz)) {	// ako taj el ne postoji treba ga dodati
						NP.push_back(tempPoz);
						TESTIRAJ(prev(NP.end()), NP);
					}
				}
			}
			
			if (y - 1 >= 0) {
				if (ispodTable[x][y - 1] != BOMBA) {
					Poz tempPoz(x, y - 1);
					if (ispodTable[x][y - 1] != '0') tempPoz.test = true;
					if (!postojiEl(NP.begin(), NP.end(), tempPoz)) {	// ako taj el ne postoji treba ga dodati
						NP.push_back(tempPoz);
						TESTIRAJ(prev(NP.end()), NP);
					}
				}
			}
			
			if (y + 1 < M) {
				if (ispodTable[x][y + 1] != BOMBA) {
					Poz tempPoz(x, y + 1);
					if (ispodTable[x][y + 1] != '0') tempPoz.test = true;
					if (!postojiEl(NP.begin(), NP.end(), tempPoz)) {	// ako taj el ne postoji treba ga dodati
						NP.push_back(tempPoz);
						TESTIRAJ(prev(NP.end()), NP);
					}
				}

			}
			
			if (x + 1 < N) {
				if (ispodTable[x + 1][y] != BOMBA) {
					Poz tempPoz(x + 1, y);
					if (ispodTable[x + 1][y] != '0') tempPoz.test = true;
					if (!postojiEl(NP.begin(), NP.end(), tempPoz)) {	// ako taj el ne postoji treba ga dodati
						NP.push_back(tempPoz);
						TESTIRAJ(prev(NP.end()), NP);
					}
				}
			}
			

		}


	}

	
	bool otkrijPolje(int i, int j) {

		bool nagaziliSmoBombu = false;

		tabla[i][j] = ispodTable[i][j];

		if (ispodTable[i][j] == BOMBA) {
			nagaziliSmoBombu = true;
			cout << "BOMBAAAA!!!" << endl;
			return nagaziliSmoBombu;
		}

		vector<Poz> nulte_pozicije;	// ovo su polja koja imaju samo 0

		vector<Poz>::iterator iNP = nulte_pozicije.begin();
		
		Poz tempPoz(i, j);
		nulte_pozicije.push_back(tempPoz);
		
		if (ispodTable[i][j] == '0') {
			TESTIRAJ(nulte_pozicije.begin(), nulte_pozicije);
		}
		


		// prebacivanje polja koja treba da e prikazu
		for (iNP = nulte_pozicije.begin(); iNP != nulte_pozicije.end(); iNP++) {
			int x = iNP->i;
			int y = iNP->j;

			tabla[x][y] = ispodTable[x][y];
			// OBRISIIIIII
			//print();
			//Sleep(200);
			// OVAJ DEO SAMO VIZUALIZUJE ALGORITAM
		}

		return nagaziliSmoBombu;
	}

	


	bool interpret(string X, string Y, string OP) {
		// Komanda treba da ima dva zareza u sebi ,,
		// slovo,cifre,znak
		bool GAME = true;

		if (X == "end" || Y == "end" || OP == "end") {
			return false;
		}

		// Prikupljanje ulaza
		int x = 0;
		if (X[0] >= 65 && X[0] <= 91) x = int(X[0] - 65);
		else if (X[0] >= 97 && X[0] <= 122) x = int(X[0] - 71);		// treba 72 umesto 97 jer (a) sledi posle (Z)
		
		for (int i = 0; i < int(Y.size()); i++) {	// proverava dal smo uneli cifre kao argumente za Y
			if (Y[i] < '0' || Y[i] > '9') {
				return GAME;
			}
		}
		int y = stoi(Y);		// atoi(const_cast<char*>(Y.c_str())); // mogli smo i ovako
		
		char c = OP[0];
		// Ulazi su prikupljeni

		// Provera kordinata
		if (x >= M) return GAME;		// Doslo je do greske i treba uneti opet komandu
		if (y >= N) return GAME;		// Doslo je do greske i treba uneti opet komandu


		// Postavljanje B
		if (c == TEMP_BOMBA) {
			tabla[y][x] = TEMP_BOMBA;
			if (testWin()) return false;
			return GAME;
		}

		if (c == PRAZNO && tabla[y][x]==TEMP_BOMBA) {	// sklanjanje oznake 
			tabla[y][x] = PRAZNO;
			return GAME;
		}


		if (otkrijPolje(y, x)) return false;		// ako nagazimo na bombu kraj igre
		
		if (testWin()) return false;			// ako smo pobedili onda je kraj igre i saljemo false za nastavak igre

		return GAME;
	}

};

// 