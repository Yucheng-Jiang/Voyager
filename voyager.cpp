#include "voyager.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"

Voyager::Voyager() {
    ReadAirport("dataset/airports.dat");
    ReadRoute("dataset/routes.dat");
}

Voyager::Voyager(std::string AptPath, std::string RoutePath) {
    ReadAirport(AptPath);
    ReadRoute(RoutePath);
}

Voyager::~Voyager() {
    // delete all airport
    for(auto it = airport_dict.begin(); it != airport_dict.end(); ++it) {
        delete (*it).second;
    }
    // delete adjacent matrix
    for(auto it = map_adj.begin(); it != map_adj.end(); ++it) {
        delete (*it).second;
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
            if (!map_adj.count(airport_dict.at(std::stoi(vec[0]))->index)) {
                std::unordered_set<int>* adj_vec = new std::unordered_set<int>;
                adj_vec->insert(airport_dict.at(std::stoi(vec[1]))->index);
                map_adj[airport_dict.at(std::stoi(vec[0]))->index] = adj_vec;
            } else {
                map_adj[airport_dict.at(std::stoi(vec[0]))->index]->insert(airport_dict.at(std::stoi(vec[1]))->index);
            }
        }
    }  
}

void Voyager::DrawGraph(std::map<int, Airport*>& airport_dict, double* centrality, std::string inputFile, std::string outputFile, int topN) {

    //TODO: Write your code here
    std::vector<double> cArray;
    std::vector<double> tempArray;
    cs225::PNG inputimage;
    inputimage.readFromFile(inputFile);
    cs225::PNG outputimage(inputimage);
    for (unsigned i = 0; i < airport_dict.size(); i++) {
        cArray.push_back(centrality[i]);
        tempArray.push_back(centrality[i]);
    }
    std::sort(tempArray.begin(), tempArray.end(), std::greater<double>());
    double topNAirport = tempArray[topN];
    int index = 0;
    for (auto& c : airport_dict) {
        int airportID = c.first;
        double lati = (airport_dict.find(airportID)->second)->lati_;
        double longi = (airport_dict.find(airportID)->second)->longi_;
        int x = convertToX(inputimage, lati, longi);
        int y = convertToY(inputimage, lati, longi);
        int length = 2;
        if (cArray[index] >= topNAirport) {
            for (int i = x - length; i < x + length; x++) {
                for (int j = y - length; j < y + length; y++) {
                    outputimage.getPixel(i, j).h = 0;
                    outputimage.getPixel(i, j).s = 1;
                    outputimage.getPixel(i, j).l = 0.5;
                    outputimage.getPixel(i, j).a = 1;
                }
            }
        } else {
            for (int i = x - length; i < x + length; x++) {
                for (int j = y - length; j < y + length; y++) {
                    outputimage.getPixel(i, j).h = 255;
                    outputimage.getPixel(i, j).s = 1;
                    outputimage.getPixel(i, j).l = 0.5;
                    outputimage.getPixel(i, j).a = 1;
                }
            }
        }
        index+=1;
    }
    outputimage.writeToFile(outputFile);
}

double* Voyager::centrality(std::map<int, std::unordered_set<int>*>& map) {
    int SIZE = map.size();
    double* centrality = new double[SIZE]{0};
    // iterate for each first and second vertex
    for (int firstVertex = 0; firstVertex < SIZE; firstVertex++) {
        for (int secondVertex = firstVertex + 1; secondVertex < SIZE; secondVertex++) {
            // if first and second vertex is neighbor, skip
            if (map.count(firstVertex) == 0 || map[firstVertex]->count(secondVertex) != 0) continue;
            // get shortest path
            int* stepCount = GetStepCount(map, firstVertex, secondVertex);
            // back traversal to find shortest path
            int* pathCount = GetPathCount(map, stepCount, firstVertex, secondVertex);
            // update centrality
            int totalPath = pathCount[firstVertex];
            for (int i = 0; i < SIZE; i++) {
                if (i == firstVertex || i == secondVertex || totalPath == 0) continue;
                centrality[i] += (double) pathCount[i] / totalPath;
            }
            // delete step count array
            delete[] stepCount;
            delete[] pathCount;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        std::cout << centrality[i] << ", ";
    }
    std::cout << "\n";
    return centrality;
}

int* Voyager::GetStepCount(std::map<int, std::unordered_set<int>*>& map, int departureIndex, int destIndex) {
   
    int SIZE = map.size();
    // initialzie all steps to -1
    int* stepCount = new int[SIZE];
    std::fill_n(stepCount, SIZE, -1);
    // set departure step = 0
    stepCount[departureIndex] = 0;
    std::queue<int> queue;
    queue.push(departureIndex);
    bool isfound = false;
    // bfs mark steps from firstVertex to secondVertex
    while (!queue.empty()) {
        // if reach destination, break bfs
        if (isfound) break;
        // get first element in queue
        int index = queue.front();
        queue.pop();
        // new step count
        int step = stepCount[index] + 1;
        // visit all neighbor node, if stepcount is not initialized or new step is smaller
        // update step count
        std::unordered_set<int>* set = map[index];
        for (std::unordered_set<int>::iterator it = set->begin(); it != set->end(); it++) {
            if (*it == destIndex) isfound = true;
            if (*it != departureIndex && (stepCount[*it] == -1 || stepCount[*it] > step)) {
                stepCount[*it] = step;
            }
            queue.push(*it);
        }
    }
    return stepCount;
}

int* Voyager::GetPathCount(std::map<int, std::unordered_set<int>*>& map, int* stepCount, int departureIndex, int destIndex) {
    
    int* pathCount = new int[map.size()]{0};
    pathCount[destIndex] = 1;
    std::map<int, std::vector<std::vector<int>>> parent;
    parent[destIndex] = std::vector<std::vector<int>>{{destIndex}};
    std::queue<int> queue;
    queue.push(destIndex);
    // find number of path
    while (!queue.empty()) {
        int index = queue.front();
        queue.pop();
        // next required step count
        int step = stepCount[index] - 1;
        // for all neighbors, find those steps matches with required step count
        std::unordered_set<int>* set = map[index];
        for (std::unordered_set<int>::iterator it = set->begin(); it != set->end(); it++) {
            // copy all parent nodes from parents to self
            if (stepCount[*it] == step) {
                queue.push(*it);
                if (parent.count(*it) == 0) {
                    std::vector<std::vector<int>> vec;
                    for (int i = 0; i < (int) parent[index].size(); i++) {
                        std::vector<int> temp = parent[index][i];
                        temp.push_back(*it);
                        vec.push_back(temp);
                    }
                    parent[*it] = vec;
                } else {
                    for (int i = 0; i < (int) parent[index].size(); i++) {
                        std::vector<int> temp = parent[index][i];
                        temp.push_back(*it);
                        parent[*it].push_back(temp);
                    }
                }
            }
        }
    }
    // count all shortest path
    for (auto& vec : parent[departureIndex]) {
        for (auto& i : vec) {
            pathCount[i]++;
        }
    }

    return pathCount;
}

int Voyager::convertToX(cs225::PNG& png, double lati, double longi) {
    int x = std::fmod((png.width() * (180 + longi)/ 360), (png.width() + (png.width() / 2)));
    return x;
}

int Voyager::convertToY(cs225::PNG& png, double lati, double longi) {
    double PI = 3.14159265359;
    double latRad = lati * PI / 180;
    double mapProjc = std::log(tan((PI / 4) + (latRad / 2)));
    int y = (png.height() / 2) - (png.width() * mapProjc / (2 * PI));
    return y;
}

std::map<int, Voyager::Airport*>& Voyager::getAptDict() {
    return airport_dict;
}