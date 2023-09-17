#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	string fileName;
	ifstream binFile;

        cout << "Enter file path: ";
        cin >> fileName;

	binFile.open(fileName, ios::binary);
	if (!binFile.is_open()) {
		cerr << "ERROR! Could not open the given file" << endl:
		return 1;
	}
        
	char buffer[100];
	binFile.read(buffer, sizeOf(buffer));

	binFile.close();
	return 0;
}
