#include <iostream>
#include <fstream>
#include "voyager.h"
#include <string>
#include <vector>
#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"

int main(int argc, const char *argv[]) {

    Voyager* voyager = new Voyager();
    //voyager->centrality((int) voyager->GetAptDict().size(), voyager->GetAdjMatrix());
    std::cout << "Drawing "<< "worldMap.png" << std::endl;
    //Voyager *voyager = new Voyager();
    //std::map<int, std::unordered_set<int>*> adj_matrix;
    //adj_matrix.insert(std::make_pair(1, new std::unordered_set<int>({1,"Hangzhou Xiaoshan International Airport","HGH",30.22949982,120.4339981})));
    std::map<int, Voyager::Airport*> airport_dict;
    //Voyager::Airport* HGH = new Voyager::Airport("Hangzhou Xiaoshan International Airport", "HGH", 30.22949982, 120.4339981, 1);
    Voyager::Airport* CTU = new Voyager::Airport("Chengdu Shuangliu International Airport", "CTU", 30.57850075, 103.9469986, 4);
    Voyager::Airport* PVG = new Voyager::Airport("Shanghai Pudong International Airport", "PVG", 31.14340019,121.8050003, 2);
    Voyager::Airport* PEK = new Voyager::Airport("Beijing Capital International Airport", "PEK", 40.08010101,116.5849991, 3);
    airport_dict.insert(std::make_pair(0, new Voyager::Airport({"Hangzhou Xiaoshan International Airport", "HGH", 30.22949982, 120.4339981, 1})));
    airport_dict.insert(std::make_pair(1, CTU));
    airport_dict.insert(std::make_pair(2, PVG));
    airport_dict.insert(std::make_pair(3, PEK));


    std::map<int, std::unordered_set<int>*> map;
    map.insert(std::make_pair(0, new std::unordered_set<int>({2, 3})));
    map.insert(std::make_pair(1, new std::unordered_set<int>({2})));
    map.insert(std::make_pair(2, new std::unordered_set<int>({0, 1, 3})));
    map.insert(std::make_pair(3, new std::unordered_set<int>({2})));
    double * centrArray =voyager->centrality(airport_dict.size(), map);

    voyager->DrawGraph(airport_dict, centrArray, "map.png", "outMap.png", 1);
    return 0;
}