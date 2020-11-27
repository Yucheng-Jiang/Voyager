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
    // departure id, destination id
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

void Voyager::DrawGraph(std::map<int, Airport*>& airport_dict, double* centrality, std::string inputFile, std::string outputFile) {
    // print progress
    std::cout << "\n\nGenerating image...\n";
    std::cout << "\033[32m" << "Image loading completed." << "\033[39m" << "\n";
    // initialize file
    cs225::PNG inputimage;
    inputimage.readFromFile(inputFile);
    cs225::PNG outputimage(inputimage);
     // initialize constant
    double MIN_HUE = 0;
    double MAX_HUE = 270;
    int IMAGE_WIDTH = inputimage.width();
    int IMAGE_HEIGHT = inputimage.height();
    double MAX_DISTANCE = std::sqrt(IMAGE_WIDTH * IMAGE_HEIGHT) / 100;
    // find max centrality
    double max_centrality = 0;
    for (int i = 0; i < (int) airport_dict.size(); i++) {
        if (centrality[i] > max_centrality) max_centrality = centrality[i];
    }
    // printing progress variable
    int displayThreshold = 1;
    int count = 0;
    std::unordered_set<int> drawHistory;
    for (auto& curr_airport : airport_dict) {
        // print progress
        int percent = (100 * (count + 1)) / (int) airport_dict.size();
        count++;
		if (percent >= displayThreshold) {
			std::cout << "\r" << "[" << std::string(percent / 5, '-') << ">" << std::string(100 / 5 - percent / 5, ' ') << "]  ";
			std::cout << percent << "%";
			std::cout.flush();
			displayThreshold += 1;
		}
        // skip airport with centrality equals to 0
        int curr_index = curr_airport.second->index;
        if (centrality[curr_index] == 0) continue;
        // get base pixel location
        int base_x = ConvertToX(inputimage, curr_airport.second->longi_);
        int base_y = ConvertToY(inputimage, curr_airport.second->lati_);
        // normalize airport centrality and get basic hue
        double base_hue = (1 - std::sqrt(centrality[curr_index] / max_centrality)) * MAX_HUE;
        // bfs add heat map color to png
        std::queue<int> queue;
        std::unordered_set<int> visited;
        queue.push(base_x + base_y * IMAGE_WIDTH);
        while (!queue.empty()) {
            int currPoint = queue.front();
            queue.pop();
            //calculate current hue based on distance
            int curr_x = currPoint % IMAGE_WIDTH;
            int curr_y = currPoint / IMAGE_WIDTH;
            //if already visited or out of bounds, skip
            if (visited.count(currPoint) != 0) continue;
            if (curr_x < 0 || curr_y < 0 || curr_x >= IMAGE_WIDTH || curr_y >= IMAGE_HEIGHT) continue;
            visited.insert(currPoint);
            double curr_hue = base_hue + ((curr_x - base_x) * (curr_x - base_x) + (curr_y - base_y) * (curr_y - base_y)) 
                                        / (MAX_DISTANCE * MAX_DISTANCE) * MAX_HUE;
            // if distance too far, skip
            if (curr_hue > MAX_HUE) continue;
            // update color
            cs225::HSLAPixel &pixel = outputimage.getPixel(curr_x, curr_y);
            if (drawHistory.count(currPoint) != 0 && pixel.h < curr_hue) continue;
            drawHistory.insert(currPoint);
            pixel.h = curr_hue;
            pixel.s = 0.5;
            pixel.l = 0.5;
            pixel.a = 1;
            if (curr_x + 1 < IMAGE_WIDTH) queue.push(currPoint + 1);
            if (curr_x - 1 >= 0) queue.push(currPoint - 1);
            if (curr_y + 1 < IMAGE_HEIGHT) queue.push(currPoint + IMAGE_WIDTH);
            if (curr_y - 1 >= 0) queue.push(currPoint - IMAGE_WIDTH);
        }
    }
    outputimage.writeToFile(outputFile);
    std::cout << "\n" << "\033[32m" << "Image Gereration Complete" << "\033[39m" << "\n";
    std::cout << "=================================\n";
}


double* Voyager::CalculateCentrality(int SIZE, std::map<int, std::unordered_set<int>*>& map) {

    std::cout << "\n=================================\n";
    std::cout << "Calculating centrality...\n";
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
    // print out progress
    std::cout << "\n" << "\033[32m" << "Calculation Finished" << "\033[39m" << "\n";
    return centrality;
}



double Voyager::ConvertToX(cs225::PNG png, double longi) {
    double x = std::fmod((png.width()*(180+longi)/360), (png.width()+(png.width()/2)));
    return x;
}

double Voyager::ConvertToY(cs225::PNG png, double lati) {
    double PI = 3.14159265359;
    double latRad = lati * PI / 180;
    double mapProjc = std::log(tan((PI / 4) + (latRad / 2)));
    double y = (png.height() / 2) - (png.width() * mapProjc / (2 * PI));
    return y;
}

std::map<int, Voyager::Airport*>& Voyager::GetAptDict() {
    return airport_dict;
}

std::map<int, std::unordered_set<int>*>& Voyager::GetAdjMatrix() {
    return map_adj;
}