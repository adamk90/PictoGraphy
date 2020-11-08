#include <iostream>

#include "ciff.h"
#include "caff.h"
#include <dirent.h>

using namespace std;

void outputBmpToFile(const string& filePath, shared_ptr<byte> bmp, ull bmpSize) {
    ofstream ofile;
    ofile.open(filePath, ofstream::out | ofstream::trunc | ofstream::binary);
    ofile.write(reinterpret_cast<const char *>(bmp.get()), bmpSize);
    ofile.close();
}

void dumpCaff(const Caff& c1, const string& previewName) {
    cout << "Creator: " << c1.getCreator() << endl;
    cout << "Year: " << c1.getYear();
    cout << " Month: " << c1.getMonth();
    cout << " Day: " << c1.getDay();
    cout << " Hour: " << c1.getHour();
    cout << " Minute: " << c1.getMinute() << endl;
    vector<Animation> anims = c1.getAnimations();
    for (ull i = 0; i < anims.size(); ++i) {
        cout << "Animation " << i + 1 << " duration: " << anims[i].duration << endl;
        ull bmpSize = 0;
        auto bmpBytes = anims[i].img.getBMP(bmpSize);
        string name = "previews/" + previewName + "_" + to_string(i) + ".bmp";
        outputBmpToFile(name, bmpBytes, bmpSize);
    }
}

void emptyPreviewsDir() {
	DIR *theFolder = opendir("./previews");
    struct dirent *next_file;
    char filepath[272];
    while ( (next_file = readdir(theFolder)) != NULL )
    {
        // build the path for each file in the folder
        sprintf(filepath, "%s/%s", "path/of/folder", next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		Caff caff = Caff::parse(cin);
		emptyPreviewsDir();
		dumpCaff(caff, "preview");
	} else if (argc == 2) {
		string filePath{argv[1]};
		ifstream ifs;
		try {
			ifs.open(filePath, ifstream::in);
			Caff caff = Caff::parse(ifs);
			emptyPreviewsDir();
			dumpCaff(caff, "preview");
		} catch (exception& e) {
			cout << e.what() << endl;
			ifs.close();
			return 2;
		}
		ifs.close();
	} else {
		printf("Usage: %s <caff-file.caff>\n", argv[0]);
		return 1;
	}
	return 0;
}