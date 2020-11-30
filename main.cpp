#include <iostream>
#include "voyager.h"
#include <string>


int main(int argc, const char *argv[]) {
    std::string inputFile = "";
    std::string outputFile = "";
    if (argc < 3) {
        inputFile = "image/worldMap-min.png";
        outputFile = "outMap.png";
    } else {
        inputFile = argv[1];
        outputFile = argv[2];
    }

    std::cout << "output is " << outputFile;
    Voyager* voyager = new Voyager();
    double* centrArray = voyager->CalculateCentrality((int) voyager->GetAptDict().size(), voyager->GetAdjMatrix());
    voyager->DrawGraph(voyager->GetAptDict(), centrArray, inputFile, outputFile);
    delete[] centrArray;
    delete voyager;
}