#include "voyager.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"

Voyager::Voyager() {
    ReadAirport("dataset/airports.dat");
    ReadRoute("dataset/routes.dat");
}

Voyager::~Voyager() {
    // delete all airport
    for(auto it = airport_dict.begin(); it != airport_dict.end(); ++it) {
        delete (*it).second;
    }
    // delete adjacent matrix
    if (matrix_adj) {
        int rowCount = sizeof(matrix_adj) / sizeof(matrix_adj[0]);
        for (int i = 0; i < rowCount; i++) {
            delete[] matrix_adj[i];
        }
    }
}

void Voyager::ReadAirport(std::string filePath) {

    std::ifstream infile(filePath);
    string line;
    int count = 0;
    // data format: id, name, IATA, lat, long
    if (infile.is_open()) {
        while (getline(infile, line)) {
            std::vector<string> vec;
            string str = "";
            bool isquote = false;
            // if there's a quote, ignore ','
            // otherwise separate string based on ','
            for (auto& c : line) {
                switch (c) {
                    case '\"':
                        isquote = !isquote;
                        break;
                    case ',':
                        if (!isquote) {
                            vec.push_back(str);
                            str = "";
                        }
                        break;
                    default:
                        str += c;
                }
            }
            vec.push_back(str);
            airport_dict[std::stoi(vec[0])] = new Airport(vec[1], vec[2], std::stod(vec[3]), std::stod(vec[4]), count);
            count++;
        }
    }
    // initialize adjecent matrix
    matrix_adj = new short*[count];
    for (int i = 0; i < count; i++) {
        matrix_adj[i] = new short[count];
        for (int j = 0; j < count; j++) {
            matrix_adj[i][j] = 0;
        }
    }
}

void Voyager::ReadRoute(std::string filePath) {

    std::ifstream infile(filePath);
    string line;
    // data format: id, name, IATA, lat, long
    if (infile.is_open()) {
        while (getline(infile, line)) {
            std::vector<string> vec;
            string str = "";
            for (auto& c : line) {
                switch (c) {
                    case ',':
                        vec.push_back(str);
                        str = "";
                        break;
                    default:
                        str += c;
                }
            }
            vec.push_back(str);
            // if airport ID is not found in dictionary, discard this route
            if (!airport_dict.count(std::stoi(vec[0])) || !airport_dict.count(std::stoi(vec[1]))) continue;
            // update adjacency matrix
            matrix_adj[airport_dict.at(std::stoi(vec[0]))->index][airport_dict.at(std::stoi(vec[1]))->index]++;
        }
    }  
}

cs225::PNG* Voyager::DrawGraph(short** matrix) {

    //TODO: Write your code here
    return nullptr;
}

int* Voyager::centrality(short** matrix) {

    //TODO: Write your code here
    return nullptr;
}


void Voyager::DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y) {

    //TODO: Write your code here

}

std::vector<std::string> Voyager::getApt() {
    std::vector<std::string> res;
    for (auto apt : airport_dict) {
        res.push_back(apt.second->name);
    }
    return res;
}