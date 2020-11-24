#include "voyager.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
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
        int length = 10;
        if (cArray[index] >= topNAirport) {
            for (int i = x - length; i < x + length; x++) {
                for (int j = y - length; j < y + length; y++) {
                    cs225::HSLAPixel &pixel = inputimage.getPixel(i, j);
                    pixel.h = 0;
                    pixel.s = 1;
                    pixel.l = 0.5;
                    pixel.a = 1;
                }
            }
        } else {
            for (int i = x - length; i < x + length; x++) {
                for (int j = y - length; j < y + length; y++) {
                    cs225::HSLAPixel &pixel = inputimage.getPixel(i, j);
                    pixel.h = 105;
                    pixel.s = 1;
                    pixel.l = 0.5;
                    pixel.a = 1;
                }
            }
        }
        index+=1;
    }
    outputimage.writeToFile(outputFile);
}

double* Voyager::centrality(int SIZE, std::map<int, std::unordered_set<int>*>& map) {

    double* centrality = new double[SIZE]{0};
    int displayThreshold = 1;
    for (int s = 0; s < SIZE; s++) {
        int percent = (100 * (s + 1)) / SIZE ;
		if (percent >= displayThreshold) {
			std::cout << "\r" << "[" << std::string(percent / 5, '-') << ">" << std::string(100 / 5 - percent / 5, ' ') << "]  ";
			std::cout << percent << "%";
			std::cout.flush();
			displayThreshold += 1;
		}

        std::stack<int> stack;
        std::map<int, std::vector<int>> predecessor;
        std::queue<int> queue;
        double* delta = new double[SIZE]{0};
        int* sigma = new int[SIZE]{0};
        int* distance = new int[SIZE]{};
        std::fill_n(distance, SIZE, -1);
        sigma[s] = 1;
        distance[s] = 0;
        queue.push(s);

        while (!queue.empty()) {
            int cur = queue.front();
            queue.pop();
            stack.push(cur);
            if (!map.count(cur)) break;
            for (int neighbor : *map[cur]) {
                if (distance[neighbor] < 0) {
                    queue.push(neighbor);
                    distance[neighbor] = distance[cur] + 1;
                }
                if (distance[neighbor] == distance[cur] + 1) {
                    sigma[neighbor] += sigma[cur];
                    if (predecessor.count(neighbor) == 0) {
                        std::vector<int> vec = {cur};
                        predecessor[neighbor] = vec;
                    } else {
                        predecessor[neighbor].push_back(cur);
                    }
                }
            }
        }

        while (!stack.empty()) {
            int cur = stack.top();
            stack.pop();
            if (predecessor.count(cur) == 0) break;
            for (int pred : predecessor[cur]) {
                delta[pred] += ((double) sigma[pred] / sigma[cur]) * (1.0 + delta[cur]);
            }
            if (cur != s) centrality[cur] += delta[cur];
        }

        delete[] delta;
        delete[] sigma;
        delete[] distance;
    }

    for (int i = 0; i < SIZE; i++) {
        centrality[i] /= 2;
    }

    return centrality;
}



int Voyager::convertToX(const cs225::PNG& png, double lati, double longi) {
    int x = std::fmod((png.width() * (180 + longi)/ 360), (png.width() + (png.width() / 2)));
    return x;
}

int Voyager::convertToY(const cs225::PNG& png, double lati, double longi) {
    double PI = 3.14159265359;
    double latRad = lati * PI / 180;
    double mapProjc = std::log(tan((PI / 4) + (latRad / 2)));
    int y = (png.height() / 2) - (png.width() * mapProjc / (2 * PI));
    return y;
}

std::map<int, Voyager::Airport*>& Voyager::GetAptDict() {
    return airport_dict;
}

std::map<int, std::unordered_set<int>*>& Voyager::GetAdjMatrix() {
    return map_adj;
}