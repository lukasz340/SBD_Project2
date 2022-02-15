#include <iostream>
#include <random>
#include <fstream> 
#include <string>
#include <time.h>

#define ALPHA 0.75
#define MAX_OVERFLOW 0.25
#define PAGE_SIZE 8
#define MAX_ELEMENT 10000

using namespace std;

struct index {
	int key;
	int pageNo;
};

struct area {
	int page = 0;
	int key = 0;
	char data[7] = "";
	int overflowPointer = NULL;
	bool czyUsuniety = false;
	bool czyPusty = false;
};
int numberOfKeysIndex = 0;
int numberOfKeysArea = 0;
int numberOfKeysOverflow = 0;
int keyIndexTest = 0;
int keyAreaTest = 0;
int keyOverflowTest = 0;
int deleted = 0;
int puste = 0;
int liczbaOperacjiDyskowych = 0;

char charTest[MAX_ELEMENT];
char charTest2[MAX_ELEMENT];
index indexTable[MAX_ELEMENT];

area areaTable[MAX_ELEMENT];
area overflowTable[MAX_ELEMENT];

long liczbaBajtow(string);
void wczytajIndex(string);
void wczytajArea(string);
void wczytajOverflow(string);
void wczytajZPlikuIndex(string);
void wczytajZPlikuArea(string);
void wczytajZPlikuOverflow(string);
void zapiszDoPlikuIndex(string);
void zapiszDoPlikuArea(string);
void zapiszDoPlikuOverflow(string);
void wczytajZPlikow();
void zapiszDoplikow();
void wyswietl();
void reorganizacja();
int wyznaczStrone(int);
void rownyIndeks(int, char*, area*);
int wyznaczElement(int, int);
void dodajDoPustego(int, int, char*, int);
int wyznaczElementOverflow(int, char*, int);
void dodajDoOverflow(int, char*, int);
void zerujPointery();
void dodajDoOverflowBezWskaznika(int, char*, int);
void ustawPointery();
void dodajElement(int, char*);
void doDodania();
void doDodaniaAktualizacja(char*);
void odczytRekordu();
char* usuniecieRekordu(int);
char* doUsuniecia();
void aktualizacja();
void menu();

int main() {
	for (int i = 0; i < MAX_ELEMENT; i++)
		indexTable[i].pageNo = i;
	wczytajZPlikuIndex("index.txt");
	wczytajZPlikuArea("area.txt");
	wyswietl();

	zapiszDoplikow();
	liczbaOperacjiDyskowych += 3;

	while (true) {
		menu();

		//cout << "LICZBA OPERACJI DYSKOWYCH: " << liczbaOperacjiDyskowych << endl;
	}
}
long liczbaBajtow(string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

void wczytajIndex(string sciezka) {

	string linia;
	ifstream write_file;
	write_file = ifstream(sciezka, ios::out | ios::binary);

	for (int i = 1; i < liczbaBajtow(sciezka) / 4 + 1; i++) {
		write_file >> indexTable[i].key;
		numberOfKeysIndex++;
	}

	ofstream file;
	file.open(sciezka, ios::out | ios::binary);
	file.close();
}

void wczytajArea(string sciezka) {

	int klucz;
	ifstream write_file;
	write_file = ifstream(sciezka, ios::out | ios::binary);
	write_file >> klucz;
	for (int i = 0; i < liczbaBajtow(sciezka) / 15; i++) {
		write_file >> areaTable[i].key >> areaTable[i].data;
		areaTable[i].data[0] = charTest[i];
		write_file >> areaTable[i].overflowPointer;
		numberOfKeysArea++;
	}

	ofstream file;
	file.open(sciezka, ios::out | ios::binary);
	file.close();
}

void wczytajOverflow(string sciezka) {

	int klucz;

	ifstream write_file;
	write_file = ifstream(sciezka, ios::out | ios::binary);
	write_file >> klucz;
	for (int i = 0; i < liczbaBajtow(sciezka) / 15; i++) {
		write_file >> overflowTable[i].key >> overflowTable[i].data;
		overflowTable[i].data[0] = charTest2[i];
		write_file >> overflowTable[i].overflowPointer;
		numberOfKeysOverflow++;
	}
	indexTable[1].key = keyIndexTest;

	ofstream file;
	file.open(sciezka, ios::out | ios::binary);
	file.close();
}

void wczytajZPlikuIndex(string sciezka) {
	int liczba;
	int licznik = 0;

	ifstream plik(sciezka);

	if (plik.good()) {

		while (!plik.eof())
		{
			plik >> liczba;
			indexTable[licznik].key = liczba;
			numberOfKeysIndex++;
			licznik++;
		}
		keyIndexTest = indexTable[1].key;
		plik.close();
	}
}

void wczytajZPlikuArea(string sciezka) {

	int klucz;
	char wartosc[7];
	int wskaznikOverflow;
	int licznik = 0;

	ifstream plik(sciezka);

	if (plik.good()) {
		while (!plik.eof())
		{
			plik >> klucz;
			for (int i = 0; i < 7; i++) {
				plik >> wartosc[i];
			}
			plik >> wskaznikOverflow;
			areaTable[licznik].key = klucz;
			for (int i = 0; i < 7; i++)
				areaTable[licznik].data[i] = wartosc[i];
			areaTable[licznik].overflowPointer = wskaznikOverflow;
			numberOfKeysArea++;
			licznik++;
		}
		plik.close();
	}
}

void wczytajZPlikuOverflow(string sciezka) {

	int klucz;
	char wartosc[7];
	int wskaznikOverflow;
	int licznik = 0;

	ifstream plik(sciezka);

	if (plik.good()) {
		while (!plik.eof())
		{
			plik >> klucz;
			for (int i = 0; i < 7; i++)
				plik >> wartosc[i];
			plik >> wskaznikOverflow;

			overflowTable[licznik].key = klucz;
			overflowTable[licznik].overflowPointer = wskaznikOverflow;
			numberOfKeysOverflow++;
			licznik++;
		}
		plik.close();
	}
}

void zapiszDoPlikuIndex(string sciezka)
{
	ofstream plik(sciezka);
	ofstream write_file;

	write_file = ofstream(sciezka, ios::out | ios::binary);

	for (int i = 0; i < numberOfKeysIndex; i++)
	{
		write_file.write((char*)&indexTable[i].key, sizeof(indexTable[i].key));
	}
	keyIndexTest = indexTable[1].key;

	numberOfKeysIndex = 0;
	write_file.close();
}

void zapiszDoPlikuArea(string sciezka) {

	ofstream plik(sciezka);
	ofstream write_file;

	write_file = ofstream(sciezka, ios::out | ios::binary);

	for (int i = 0; i < numberOfKeysArea; i++)
	{
		write_file.write((char*)&areaTable[i].key, 4);
		write_file.write(areaTable[i].data, 7);
		write_file.write((char*)&areaTable[i].overflowPointer, 4);
		charTest[i] = areaTable[i].data[0];
	}
	numberOfKeysArea = 0;
	write_file.close();
}

void zapiszDoPlikuOverflow(string sciezka) {

	ofstream plik(sciezka);
	ofstream write_file;

	write_file = ofstream(sciezka, ios::out | ios::binary);

	for (int i = 0; i < numberOfKeysOverflow; i++)
	{
		write_file.write((char*)&overflowTable[i].key, 4);
		write_file.write(overflowTable[i].data, 7);
		write_file.write((char*)&overflowTable[i].overflowPointer, 4);
		charTest2[i] = overflowTable[i].data[0];
	}
	numberOfKeysOverflow = 0;
	write_file.close();
}

void wczytajZPlikow() {
	wczytajIndex("index.dat");
	wczytajArea("area.dat");
	wczytajOverflow("overflow.dat");
}

void zapiszDoplikow() {
	zapiszDoPlikuArea("area.dat");
	zapiszDoPlikuIndex("index.dat");
	zapiszDoPlikuOverflow("overflow.dat");
}

void wyswietl() {


	cout << "INDEX" << endl;
	for (int i = 0; i < numberOfKeysIndex; i++) {
		cout << "\tKEY: " << indexTable[i].key << "  PAGENO: " << i + 1 << endl;
	}
	cout << "PRIMARY AREA" << endl;
	for (int i = 0; i < numberOfKeysArea + puste; i++) {
		if (i % PAGE_SIZE == 0)
			cout << "Strona: " << i / PAGE_SIZE + 1 << endl;
		if (areaTable[i].czyPusty == false) {
			cout << "\tKEY: " << areaTable[i].key << "  DATA: " << areaTable[i].data << "  OVERFLOW: " << areaTable[i].overflowPointer;
			if (areaTable[i].czyUsuniety == true)
				cout << "[DELETED]" << endl;
			else
				cout << '\n';
		}
		else {
			cout << "EMPTY" << endl;
		}
	}
	cout << "OVERFLOW AREA" << endl;
	for (int i = 0; i < numberOfKeysOverflow; i++) {
		if (i % PAGE_SIZE == 0)
			cout << "Strona: " << i / PAGE_SIZE + 1 << endl;
		cout << "\tKEY: " << overflowTable[i].key << "  DATA: " << overflowTable[i].data << "  OVERFLOW: " << overflowTable[i].overflowPointer;
		if (overflowTable[i].czyUsuniety == true)
			cout << "[DELETED]" << endl;
		else
			cout << '\n';
	}
	liczbaOperacjiDyskowych += 3;
}

void reorganizacja() {
	wczytajZPlikow();
	liczbaOperacjiDyskowych += 3;

	area* reorgTable = new area[numberOfKeysOverflow + numberOfKeysArea - deleted];

	int indeks = 0;
	for (int i = 0; i < numberOfKeysArea + puste; i++) {
		if (areaTable[i].czyPusty == true || areaTable[i].czyUsuniety == true || areaTable[i].key == 0)
		{
			continue;
		}

		reorgTable[indeks] = areaTable[i];
		indeks++;
		if (areaTable[i].overflowPointer == 0)
			continue;

		else {
			int indeksOverflow = 0;
			while (overflowTable[areaTable[i].overflowPointer - 1 + indeksOverflow].overflowPointer != 0) {
				reorgTable[indeks] = overflowTable[areaTable[i].overflowPointer - 1 + indeksOverflow];
				indeksOverflow++;
				indeks++;
			}
			reorgTable[indeks] = overflowTable[areaTable[i].overflowPointer - 1 + indeksOverflow];
			indeks++;
			indeksOverflow = 0;
		}
	}

	indeks = 0;
	int areaRange = ALPHA * PAGE_SIZE;
	puste = 0;
	for (int i = 0; i <= numberOfKeysOverflow + numberOfKeysArea - deleted; i++) {
		while (indeks % PAGE_SIZE >= areaRange) {
			areaTable[indeks].czyPusty = true;
			areaTable[indeks].czyUsuniety = false;
			areaTable[indeks].key = 0;
			areaTable[indeks].overflowPointer = 0;
			puste++;
			indeks++;
		}
		areaTable[indeks] = reorgTable[i];
		areaTable[indeks].czyPusty = false;
		areaTable[indeks].czyUsuniety = false;
		areaTable[indeks].overflowPointer = 0;
		indeks++;
	}
	indeks--;
	while (indeks % PAGE_SIZE != 0) {
		areaTable[indeks].czyPusty = true;
		areaTable[indeks].czyUsuniety = false;
		areaTable[indeks].key = 0;
		areaTable[indeks].overflowPointer = 0;
		puste++;
		indeks++;
	}
	cout << indeks;
	int i = 0;
	while (true) {
		indexTable[i].key = areaTable[i * PAGE_SIZE].key;
		indexTable[i].pageNo = i + 1;
		numberOfKeysIndex = i + 1;
		i++;
		if (areaTable[i * PAGE_SIZE].key == 0)
			break;
	}

	delete[]reorgTable;

	numberOfKeysArea = numberOfKeysArea + numberOfKeysOverflow - deleted;
	numberOfKeysOverflow = 0;
	deleted = 0;
	for (int i = 0; i < numberOfKeysArea; i++) {
		areaTable[i].overflowPointer = 0;
	}
	wyswietl();
	zapiszDoplikow();
	liczbaOperacjiDyskowych += 3;
}

int wyznaczStrone(int indeks) {
	int numerStrony;
	for (int i = 0; i < numberOfKeysIndex; i++) {
		if (indeks == indexTable[i].key)
		{
			numerStrony = i;
			break;
		}
		if (indeks < indexTable[i].key) {
			numerStrony = i - 1;
			break;
		}
		if (i == numberOfKeysIndex - 1)
			numerStrony = i;

	}
	return numerStrony;
}

void rownyIndeks(int ktoryElement, char*wartosc, area* table) {
	if (table[ktoryElement].czyUsuniety == true) {
		table[ktoryElement].czyUsuniety = false;
		deleted--;
		for (int i = 0; i < 7; i++)
			table[ktoryElement].data[i] = wartosc[i];


		wyswietl();
		zapiszDoplikow();

		return;
	}
	else {
		cout << "istnieje juz taki element" << endl;
		wyswietl();

		zapiszDoplikow();

		return;
	}
}

int wyznaczElement(int ktoryElement, int indeks) {
	for (int i = 0; i < PAGE_SIZE; i++) {

		if (areaTable[ktoryElement].czyPusty == true) {
			ktoryElement--;
			break;
		}
		if (areaTable[ktoryElement].key < indeks) {
			ktoryElement++;
		}
		else {
			if (areaTable[ktoryElement].key == indeks)
				break;
			else {
				ktoryElement--;
				break;
			}
		}
		if (i == PAGE_SIZE - 1)
		{
			if (areaTable[ktoryElement].key != indeks)
				ktoryElement--;
		}
	}
	return ktoryElement;
}

void dodajDoPustego(int ktoraStrona, int indeks, char*wartosc, int ktoryElement) {
	if (areaTable[(ktoraStrona)*PAGE_SIZE].key == indeks) {
		if (areaTable[(ktoraStrona)*PAGE_SIZE].czyUsuniety == true) {
			rownyIndeks(ktoraStrona * PAGE_SIZE, wartosc, areaTable);
			return;
		}
	}
	ktoryElement++;
	int poz = 0;

	for (int i = 0; i < PAGE_SIZE; i++)
	{
		if (areaTable[(ktoraStrona - 1) * PAGE_SIZE + i].czyPusty == true)
			break;
		if (indeks < areaTable[(ktoraStrona - 1) * PAGE_SIZE + i].key)
			break;
		poz++;
	}

	for (int i = PAGE_SIZE - 1; i > poz; i--) {
		areaTable[(ktoraStrona - 1) * PAGE_SIZE + i] = areaTable[(ktoraStrona - 1) * PAGE_SIZE + i - 1];
	}
	areaTable[(ktoraStrona - 1) * PAGE_SIZE + poz].key = indeks;
	areaTable[(ktoraStrona - 1) * PAGE_SIZE + poz].czyPusty = false;
	for (int i = 0; i < 7; i++)
		areaTable[(ktoraStrona - 1) * PAGE_SIZE + poz].data[i] = wartosc[i];
	puste--;
	numberOfKeysArea++;
	wyswietl();
	zapiszDoplikow();
}

int wyznaczElementOverflow(int ktoryElement, char*wartosc, int indeks) {
	ktoryElement = areaTable[ktoryElement].overflowPointer - 1;
	while (ktoryElement < numberOfKeysOverflow) {
		if (indeks == overflowTable[ktoryElement].key) {
			rownyIndeks(ktoryElement, wartosc, overflowTable);
			return -1;
		}
		else if (indeks < overflowTable[ktoryElement].key) {
			ktoryElement++;
			break;
		}
		ktoryElement++;
	}
	return ktoryElement;
}

void dodajDoOverflow(int ktoryElement, char*wartosc, int indeks) {
	for (int i = numberOfKeysOverflow; i > ktoryElement; i--) {
		overflowTable[i] = overflowTable[i - 1];
	}
	overflowTable[ktoryElement].key = indeks;
	for (int i = 0; i < 7; i++)
		overflowTable[ktoryElement].data[i] = wartosc[i];
	if (overflowTable[ktoryElement].czyUsuniety == true) {
		overflowTable[ktoryElement].czyUsuniety = false;
		deleted--;
	}
}

void zerujPointery() {
	for (int i = 0; i < numberOfKeysArea + puste; i++)
		areaTable[i].overflowPointer = NULL;
	for (int i = 0; i < numberOfKeysOverflow; i++)
		overflowTable[i].overflowPointer = NULL;
}

void dodajDoOverflowBezWskaznika(int indeks, char*wartosc, int ktoryElement) {
	int pozycja = 0;
	for (int i = 0; i < numberOfKeysOverflow; i++) {
		if (overflowTable[i].key > indeks) {
			break;
		}
		else if (overflowTable[i].key == indeks)
		{
			rownyIndeks(i, wartosc, overflowTable);
			return;
		}
		else
			pozycja++;
	}

	for (int i = numberOfKeysOverflow; i > pozycja; i--) {
		overflowTable[i] = overflowTable[i - 1];
	}

	overflowTable[pozycja].key = indeks;
	for (int i = 0; i < 7; i++)
		overflowTable[pozycja].data[i] = wartosc[i];
	areaTable[ktoryElement - 1].overflowPointer = pozycja + 1;
}

void ustawPointery() {
	for (int i = 0; i < numberOfKeysOverflow; i++) {
		int max = 0;
		int indexOverflow;
		bool overflow = true;

		if (i != 0)
		{
			max = overflowTable[i - 1].key;
			indexOverflow = i - 1;

		}

		int indeksPomocniczy = 0;
		for (int j = 0; j < numberOfKeysIndex; j++) {
			if (indexTable[j].key > overflowTable[i].key) {
				indeksPomocniczy--;
				break;
			}
			indeksPomocniczy++;

		}
		if (indeksPomocniczy == numberOfKeysIndex)
			indeksPomocniczy--;
		for (int j = 0; j < PAGE_SIZE; j++) {
			if (areaTable[PAGE_SIZE * indeksPomocniczy + j].key == NULL || areaTable[PAGE_SIZE * indeksPomocniczy + j].key > overflowTable[i].key)
				break;
			if (areaTable[PAGE_SIZE * indeksPomocniczy + j].key<overflowTable[i].key && areaTable[PAGE_SIZE * indeksPomocniczy + j].key>max) {
				max = areaTable[PAGE_SIZE * indeksPomocniczy + j].key;
				indexOverflow = PAGE_SIZE * indeksPomocniczy + j;
				overflow = false;
			}

		}
		if (overflow == true)
			overflowTable[i - 1].overflowPointer = i + 1;
		else {
			areaTable[indexOverflow].overflowPointer = i + 1;
		}
	}
}

void dodajElement(int indeks, char* wartosc) {

	wczytajZPlikow();
	liczbaOperacjiDyskowych += 2;

	int najblizszyindex = 0;
	int ktoryElement = -1;
	bool wskaznikNaOverflow = false;
	int ktoraStrona = wyznaczStrone(indeks);
	ktoraStrona++;
	ktoryElement = (ktoraStrona - 1) * PAGE_SIZE;
	bool czyPuste = false;
	ktoryElement = wyznaczElement(ktoryElement, indeks);

	for (int i = 0; i < PAGE_SIZE; i++) {
		if (areaTable[(ktoraStrona - 1)*PAGE_SIZE + i].czyPusty == true) {
			czyPuste = true;
		}
	}
	if (areaTable[ktoryElement].key == indeks) {
		rownyIndeks(ktoryElement, wartosc, areaTable);
		return;
	}

	if (areaTable[ktoryElement].overflowPointer != 0) {
		wskaznikNaOverflow = true;
		ktoryElement = wyznaczElementOverflow(ktoryElement, wartosc, indeks);
		if (ktoryElement == -1)
			return;
	}
	ktoryElement--;

	if (wskaznikNaOverflow == true) {
		dodajDoOverflow(ktoryElement, wartosc, indeks);
		liczbaOperacjiDyskowych += 2;
	}

	else {
		if (czyPuste == true) {
			dodajDoPustego(ktoraStrona, indeks, wartosc, ktoryElement);
			return;
		}
		dodajDoOverflowBezWskaznika(indeks, wartosc, ktoryElement);
	}
	numberOfKeysOverflow++;

	zerujPointery();
	ustawPointery();

	cout << numberOfKeysArea << endl << numberOfKeysOverflow << endl;
	if (numberOfKeysArea * MAX_OVERFLOW <= numberOfKeysOverflow)
		reorganizacja();
	else {
		wyswietl();

		zapiszDoplikow();
		liczbaOperacjiDyskowych += 2;

	}
}

void doDodania() {
	int indeks;
	char wartosc[7];
	cout << "Podaj indeks:";
	cin >> indeks;
	cout << "Podaj wartosc:";
	for (int i = 0; i < 7; i++)
		cin >> wartosc[i];
	dodajElement(indeks, wartosc);
}

void doDodaniaAktualizacja(char* data) {
	if (data != NULL) {
		int indeks;
		cout << "Podaj indeks:";
		cin >> indeks;
		dodajElement(indeks, data);
	}
	else {
		cout << "nie ma takiego rekordu" << endl;
	}
}

void odczytRekordu() {
	wczytajZPlikow();
	liczbaOperacjiDyskowych += 2;
	cout << "Podaj wartosc indeksu" << endl;
	int indeks;
	cin >> indeks;
	int numerStrony = wyznaczStrone(indeks);
	int element = PAGE_SIZE * numerStrony;

	for (int i = 0; i < PAGE_SIZE; i++) {
		if (areaTable[element + i].key == indeks)
		{
			cout << "Wyszukano element o indeksie " << indeks << ": " << areaTable[element + i].data << endl;
			zapiszDoplikow();
			liczbaOperacjiDyskowych += 2;
			return;
		}
		if (areaTable[element + i].key > indeks) {
			liczbaOperacjiDyskowych++;
			element = areaTable[element + i - 1].overflowPointer;
			if (overflowTable[element - 1].key == indeks) {
				cout << "Wyszukano element o indeksie " << indeks << ": " << overflowTable[element - 1].data << endl;
				zapiszDoplikow();
				liczbaOperacjiDyskowych += 3;
				return;
			}
		}
	}
	cout << "nie znaleziono elementu" << endl;
	zapiszDoplikow();
	liczbaOperacjiDyskowych += 3;
}

char* usuniecieRekordu(int indeks) {
	wczytajZPlikow();
	liczbaOperacjiDyskowych += 2;

	int numerStrony = wyznaczStrone(indeks);

	int element = PAGE_SIZE * numerStrony;

	for (int i = 0; i < PAGE_SIZE; i++) {
		if (areaTable[element + i].key == indeks)
		{
			areaTable[element + i].czyUsuniety = true;
			deleted++;
			wyswietl();
			zapiszDoplikow();
			liczbaOperacjiDyskowych += 2;
			return areaTable[element + i].data;
		}
		liczbaOperacjiDyskowych++;
		if (overflowTable[areaTable[element + i].overflowPointer - 1].key == indeks) {
			element = areaTable[element + i].overflowPointer;
			if (overflowTable[element - 1].key == indeks) {
				overflowTable[element - 1].czyUsuniety = true;
				deleted++;

				wyswietl();
				zapiszDoplikow();
				liczbaOperacjiDyskowych += 3;
				return overflowTable[element - 1].data;
			}
		}
		else if (i == 2 && overflowTable[areaTable[element + i].overflowPointer - 1].key < indeks)
		{
			for (int j = areaTable[element + i].overflowPointer - 1; j < numberOfKeysOverflow; j++) {
				if (overflowTable[j].key == indeks) {
					overflowTable[j].czyUsuniety = true;
					deleted++;

					wyswietl();
					zapiszDoplikow();
					liczbaOperacjiDyskowych += 3;
					return overflowTable[j].data;
				}
			}
		}
	}
	cout << "nie znaleziono elementu" << endl;
	wyswietl();
	zapiszDoplikow();
	liczbaOperacjiDyskowych += 3;
	return NULL;
}

char* doUsuniecia() {
	cout << "Podaj indeks do usuniecia" << endl;
	int indeks;
	cin >> indeks;
	char*data = usuniecieRekordu(indeks);
	return data;
}

void aktualizacja() {
	char* data = doUsuniecia();
	doDodaniaAktualizacja(data);
}

void menu() {
	char wybor;
	cout << "\nWybierz Operacje:\nW - Wstaw element\nR - Reorganizacja\nO - Odczyt\nU - Usuń rekord\nA - Aktualizacja" << endl;
	cin >> wybor;
	switch (wybor) {
	case 'W':
		doDodania();
		break;
	case'O':
		odczytRekordu();
		break;
	case'U':
		doUsuniecia();
		break;
	case'A':
		aktualizacja();
		break;
	case'R':
		reorganizacja();
		break;
	default:
		break;
	}
}
