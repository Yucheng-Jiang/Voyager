#include <iostream>
#include "voyager.h"
#include <string>


int main(int argc, const char *argv[]) {
    std::string inputFile = "";
    std::string outputFile = "";
    int frameCount = 0;
    if (argc < 3) {
        inputFile = "image/worldMap-min.png";
        outputFile = "outMap.png";
    } else if (argc < 4) {
        inputFile = argv[1];
        outputFile = argv[2];
    } else {
        inputFile = argv[1];
        outputFile = argv[2];
        frameCount = std::stoi(argv[3]);
    }
    Voyager* voyager = new Voyager();
    double* centrArray = voyager->CalculateCentrality((int) voyager->GetAptDict().size(), voyager->GetAdjMatrix());
    voyager->DrawAnimation(voyager->GetAptDict(), centrArray, inputFile, outputFile, frameCount > 0, frameCount);
    delete[] centrArray;
    delete voyager;
}