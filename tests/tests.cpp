#include <string>
#include <vector>
#include <map>
#include <unordered_set>

#include "../catch/catch.hpp"
#include "../voyager.h"
#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

//TODO: Add more test and testing strategy

/*---------- TESTING STRATEGY -----------
1. TEST Init
    - 1 Test read airport database size.
    - 2 Test first/last/randomly select airport read accuracy.

2. TEST Betweeness Centrality

------------------ END ------------------*/

TEST_CASE("Test Init", "[apt-dict init][init]") {

    int APT_DAT_SIZE = 7698;
    Voyager* voyager = new Voyager();
    std::map<int, Voyager::Airport*> apt_dict = voyager->getAptDict();

    //randomly select apt to test
    //1,Goroka Airport,GKA,-6.081689835,145.3919983
    //3386,Hangzhou Xiaoshan International Airport,HGH,30.22949982,120.4339981
    //3395,Chengdu Shuangliu International Airport,CTU,30.57850075,103.9469986
    //3406,Shanghai Pudong International Airport,PVG,31.14340019,121.8050003
    //14110,Melitopol Air Base,\N,46.880001,35.305

    //REQUIRED
    Voyager::Airport* GKA = new Voyager::Airport("Goroka Airport", "GKA", -6.081689835, 145.3919983, 0);
    Voyager::Airport* HGH = new Voyager::Airport("Hangzhou Xiaoshan International Airport", "HGH", 30.22949982, 120.4339981, 3190);
    Voyager::Airport* CTU = new Voyager::Airport("Chengdu Shuangliu International Airport", "CTU", 30.57850075, 103.9469986, 3199);
    Voyager::Airport* AB = new Voyager::Airport("Melitopol Air Base", "\\N", 46.880001, 35.305, 7697);

    //ACTUAL
    Voyager::Airport *A_GKA;
    Voyager::Airport *A_HGH;
    Voyager::Airport *A_CTU;
    Voyager::Airport *A_AB;
    int size = 0;
    for (auto apt : apt_dict) {
        size++;
        switch (apt.first) {
        case 1:
            A_GKA = apt.second;
            break;
        case 3386:
            A_HGH = apt.second;
            break;
        case 3395:
            A_CTU = apt.second;
            break;
        case 14110:
            A_AB = apt.second;
            break;
        default:
            break;
        }
    }

    REQUIRE(size == APT_DAT_SIZE);
    REQUIRE(*GKA == *A_GKA);
    REQUIRE(*HGH == *A_HGH);
    REQUIRE(*CTU == *A_CTU);
    REQUIRE(*AB == *A_AB);
    //Clean up
    delete GKA;     GKA = nullptr;
    delete HGH;     HGH = nullptr;
    delete CTU;     CTU = nullptr;
    delete AB;      AB = nullptr;
    delete voyager; voyager = nullptr;
}

// src graph: ../tests/CentralityTest1 Elem.png
TEST_CASE("Test Centrality", "[centrality][elementary]") {

    Voyager *voyager = new Voyager();
    //init
    std::map<int, std::unordered_set<int>*> adj_matrix;
    int id = 0;
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({2})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({5})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({0, 5})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({4})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({3, 5, 9})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({1, 2, 4, 6, 7, 8})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({5, 7, 9})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({5, 6, 10})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({5, 11})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({4, 6, 12, 13})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({7, 11, 13, 14})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({8, 10, 14})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({9})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({9, 10, 14})));
    adj_matrix.insert(std::pair(id++, new std::unordered_set<int>({10, 11, 13})));
    id = 0;
    // Not normalized result
    double *expected = new double[id];
    expected[id++] = 0;
    expected[id++] = 0;
    expected[id++] = 13;
    expected[id++] = 0;
    expected[id++] = 19.33;
    expected[id++] = 47.5;      //highest ID = 5;
    expected[id++] = 8.33;
    expected[id++] = 10.33;
    expected[id++] = 8.5;
    expected[id++] = 23.17;
    expected[id++] = 9.33;
    expected[id++] = 5;
    expected[id++] = 0;
    expected[id++] = 9.5;
    expected[id++] = 2;
    double *res = voyager->centrality(adj_matrix);
    
    // Check result size
    REQUIRE(sizeof(expected) == sizeof(res));

    // Check elements
    bool same_arr = true;
    for (int i = 0; i < id; i++) {
        if (expected[i] != res[i]) {
            same_arr = false;
            break;
        }
    }
    REQUIRE(same_arr);


}