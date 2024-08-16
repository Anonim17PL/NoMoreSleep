// NMS-Patcher.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "resource1.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
using namespace std;

const char filelist[][60] = {
	"OMSI.EXE",
	"d3dx9.dll",
	"dxerr9.dll",
	"BaseType.dll",
	"dxerr9.dll",
	"fontsPr.dll",
	"qtintf.dll",
	"qtintf70.dll"
};

void printHelp() {
	cout << "NMSPATCHER [/ODE] [/Y]" << endl;
	cout << endl;
	cout << "Description:" << endl;
	cout << "This tool allows you to patch files to use NoMoreSleep." << endl;
	cout << endl;
	cout << "Parameter List:" << endl;
	cout << "/ODE\tDisable the prompt to confirm whether to update Open Dynamics Engine." << endl;
	cout << "/Y\tSuppresses the prompt to confirm whether to patch files.\n\tBy default it will not update ODE unless the /ODE switch is added." << endl;
	cout << endl;
	cout << "List of patched files:" << endl;
	for (int i = 0; i < sizeof(filelist)/sizeof(filelist[0]); i++)
		cout << filelist[i] << endl;
	exit(0);
}

bool ODEPatch = false;
bool noprompt = false;

void parseOneArg(char argv[]) {
	if (_stricmp(argv, "/ODE") == 0)
		ODEPatch = true;
	if (_stricmp(argv, "/Y") == 0)
		noprompt = true;
	if (_stricmp(argv, "/H") == 0)
		printHelp();
	if (_stricmp(argv, "/?") == 0)
		printHelp();
	if (_stricmp(argv, "/HELP") == 0)
		printHelp();
}

bool warnInfo() {
	if (noprompt)
		return false;

	cout << "WARNING: The program modifies files without making a backup copy.\n"
		"Make a backup copy of the files to avoid a troublesome repair/reinstall.\n\n"
		"You can find a list of files by typing NMSPATCHER /? or NMSPATCHER /help.\n\n"
		"Continue anyway? (Y/N)" << endl;
	char enterstr[100] = { 0 };
	cin.getline(enterstr, 100);
	return (_stricmp(enterstr, "Y") == 0 || _stricmp(enterstr, "YES") == 0) ? false : true;
}


int patchFile(const char filename[]) {
	cout << filename << ": ";
	FILE* hfile = NULL;
	fopen_s(&hfile, filename, "r+b");
	if (hfile == NULL) {
		cout << "NOT EXIST" << endl;
		return -1;
	}

	//Search "KERNEL32.DLL"
	const char tofind[13] = "KERNEL32.DLL";
	const char toreplace[13] = "KRNL32.DLL\0\0";
	char buffer[13] = { 0 };

	int count = 0;
	int dotcount = 0;

	while (!feof(hfile)) {
		size_t readed = fread(buffer, 1, 13, hfile);
		//cout << readed << endl;
		if (dotcount) {
			dotcount = 0;
			cout << ".";
		}
		if (readed < 13)
			break;
		buffer[12] = 0;
		if (_stricmp(buffer, tofind) == 0) {
			fseek(hfile, -13, SEEK_CUR);
			fwrite(toreplace, 13, 1, hfile);
			count++;
			dotcount++;
		}
		else {
			if (feof(hfile))
				break;
			fseek(hfile, -12, SEEK_CUR);
		}
	}

	fclose(hfile);
	cout << " OK (" << count << " imports)" << endl;
}

void patchAllFiles() {
	cout << "Patching files..." << endl;
	int countok = 0;
	for (int i = 0;i < sizeof(filelist) / 60;i++) {
		int ok = patchFile(filelist[i]);
		if (ok > 0)
			countok++;
	}
	cout << "Patched " << countok << " files." << endl;
}

void unpackNMS() {
	HRSRC hResInfo = FindResource(NULL,MAKEINTRESOURCE(NMS_DLL),RT_RCDATA);
	HGLOBAL hglb = LoadResource(NULL, hResInfo);
	const void* dll = LockResource(hglb);
	DWORD datasize = SizeofResource(NULL, hResInfo);
	FILE* hfile = NULL;
	fopen_s(&hfile, "KRNL32.DLL", "wb");
	if (hfile == NULL) {
		cout << "Failed to unpack KRNL32.DLL" << endl;
		return;
	}
	cout << "Unpacking KRNL32.DLL..." << endl;
	fwrite(dll, datasize, 1, hfile);
	fclose(hfile);
}

void unpackODE() {
	HRSRC hResInfo = FindResource(NULL,MAKEINTRESOURCE(ODE_DLL),RT_RCDATA);
	HGLOBAL hglb = LoadResource(NULL, hResInfo);
	const void* dll = LockResource(hglb);
	DWORD datasize = SizeofResource(NULL, hResInfo);
	FILE* hfile = NULL;
	fopen_s(&hfile, "ODE.DLL", "wb");
	if (hfile == NULL) {
		cout << "Failed to unpack KRNL32.DLL" << endl;
		return;
	}
	cout << "Unpacking ODE.DLL..." << endl;
	fwrite(dll, datasize, 1, hfile);
	fclose(hfile);
}

int main(int argc, char* argv[]) {
	cout << "No More Sleep AutoPatcher - OMSI 2 version" << endl;
	cout << endl;
	for (int i = 1; i < argc; i++)
		parseOneArg(argv[i]);
	cout << "Starting..." << endl;
	if (warnInfo())
		exit(1);
	unpackNMS();
	if (ODEPatch)
		unpackODE();
	patchAllFiles();
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
