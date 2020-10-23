// szg_bizt_proj.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CAFF.h"
#include "CIFF.h"

int main()
{
    Rgb* pixelarray = new Rgb[4];
    for (int i = 0; i < 4; i++)
    {
        pixelarray[i].setRGB(i, i, i);
    }
    CIFF* kepek = new CIFF[2];
    CIFF kep = CIFF(10, 10, 2, 2, pixelarray, "valami", "megvalami");
    CIFF kep2 = CIFF(10, 10, 2, 2, pixelarray, "valami", "megvalami");
    kepek[0] = kep;
    kepek[1] = kep2;
    CAFF cafi = CAFF(10, 2, 1990, 1, 2, 3, 4, 7, "Zsolti", 10, kepek);
    Rgb masodikpixel = kep[1];
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
