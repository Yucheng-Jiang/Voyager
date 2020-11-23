#include "voyager.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>

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

cs225::PNG* Voyager::DrawGraph( std::map<int, std::unordered_set<int>*>& map) {

    //TODO: Write your code here
    return nullptr;
}

double* Voyager::centrality( std::map<int, std::unordered_set<int>*>& map) {

    int SIZE = map.size();
    double* centrality = new double[SIZE];
    // iterate for each first and second vertex
    for (int firstVertex = 0; firstVertex < SIZE; firstVertex++) {
        for (int secondVertex = firstVertex + 1; secondVertex < SIZE; secondVertex++) {
            // if first and second vertex is neighbor, skip
            if (map.count(firstVertex) == 0 || map[firstVertex]->count(secondVertex) != 0) continue;
            // get shortest path
            int* stepCount = getStepCount(map, firstVertex, secondVertex);
            // back traversal to find shortest path
            int* pathCount = getPathCount(map, stepCount, firstVertex, secondVertex);
            // update centrality
            int totalPath = pathCount[firstVertex];
            for (int i = 0; i < SIZE; i++) {
                if (i == firstVertex || i == secondVertex) continue;
                centrality[i] += pathCount[i] / totalPath;
            }
            // delete step count array
            delete[] stepCount;
            delete[] pathCount;
        }
    }
    return centrality;
}

int* getStepCount(std::map<int, std::unordered_set<int>*>& map, int departureIndex, int destIndex) {

    int SIZE = map.size();
    int* stepCount = new int[SIZE];
    std::queue<int> queue;
    queue.push(departureIndex);
    bool isfound = false;
    // bfs mark steps from firstVertex to secondVertex
    while (!queue.empty() && !isfound) {
        // get first element in queue
        int index = queue.front();
        queue.pop();
        int step = stepCount[index] + 1;
        std::unordered_set<int>* set = map[index];
        for (int i = 0 ; i < set->size(); i++) {
            if (i == destIndex) isfound = true;
            if (stepCount[i] != 0 || stepCount[i] > step) {
                stepCount[i] = step;
            }
            queue.push(i);
        }
    }
    return stepCount;
}

int* getPathCount(std::map<int, std::unordered_set<int>*>& map, int* stepCount, int departureIndex, int destIndex) {

    int* pathCount = new int[map.size()];
    std::queue<int> queue;
    queue.push(destIndex);
    while (!queue.empty()) {
        int index = queue.front();
        queue.pop();
        int step = stepCount[index] - 1;
        pathCount[index]++;
        std::unordered_set<int>* set = map[index];
        int tempNodeCount = 0;
        for (int i = 0; i < set->size(); i++) {
            if (stepCount[i] == step)  queue.push(i);
        }
    }
    return pathCount;
}


void Voyager::DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y) {

    //TODO: Write your code here

}

std::map<int, Voyager::Airport*>& Voyager::getAptDict() {
    return airport_dict;
}