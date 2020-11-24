#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iterator>

#include "../catch/catch.hpp"
#include "../voyager.h"
#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

//TODO: Add more test and testing strategy

/*---------- TESTING STRATEGY -----------
1. TEST Init
    - 1 Test read airport database size.
    - 2 Test first/last/randomly select airport read accuracy.
    - 3 Test Adjacency matrix
2. TEST Betweeness Centrality
    - 1 Test basic graph centrality 1
    - 2 Test basic graph centrality 2
    - 3 Test database with most connected airport LHR
3. TEST Visualization
    - 1 Test coordinate conversion

------------------ END ------------------*/

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

TEST_CASE("Test Apt Init", "[apt-dict init][init][basic]") {

    int APT_DAT_SIZE = 7698;
    Voyager* voyager = new Voyager();
    std::map<int, Voyager::Airport*> apt_dict = voyager->GetAptDict();

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

TEST_CASE("Test adj-matrix init", "[routes init][init][basic]") {

    Voyager *voyager = new Voyager("dataset/testapt.dat", "dataset/testroutes");
    Voyager::Airport* HGH = new Voyager::Airport("Hangzhou Xiaoshan International Airport", "HGH", 30.22949982, 120.4339981, 1);
    Voyager::Airport* CTU = new Voyager::Airport("Chengdu Shuangliu International Airport", "CTU", 30.57850075, 103.9469986, 4);
    Voyager::Airport* PVG = new Voyager::Airport("Shanghai Pudong International Airport", "PVG", 31.14340019,121.8050003, 2);
    Voyager::Airport* PEK = new Voyager::Airport("Beijing Capital International Airport", "PEK", 40.08010101,116.5849991, 3);

    // Expected
    std::map<int, std::unordered_set<int>*> expected;
    expected.insert(std::make_pair(0, new std::unordered_set<int>({2, 3})));
    expected.insert(std::make_pair(1, new std::unordered_set<int>({2})));
    expected.insert(std::make_pair(2, new std::unordered_set<int>({0, 1, 3})));
    expected.insert(std::make_pair(3, new std::unordered_set<int>({2})));
    // Actual
    std::map<int, std::unordered_set<int>*> actual = voyager->GetAdjMatrix();
    REQUIRE(expected.size() == actual.size());
    bool same_map = true;
    
    for (int i = 1; i < (int)expected.size(); i++) {
        if (*expected.at(i) != *actual.at(i)) {
            same_map = false;
            break;
        }
    }

    //clean up
    delete voyager;             voyager = nullptr;
    for (auto it : expected) {
        delete it.second;       it.second = nullptr;
    }
    REQUIRE(same_map);
}

//src graph: ../tests/CentralityTest1 Elem.png
TEST_CASE("Test Centrality", "[centrality][basic]") {

    Voyager *voyager = new Voyager();
    //init
    std::map<int, std::unordered_set<int>*> adj_matrix;
    int id = 0;
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({2})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({5})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({0, 5})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({4})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({3, 5, 9})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({1, 2, 4, 6, 7, 8})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({5, 7, 9})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({5, 6, 10})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({5, 11})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({4, 6, 12, 13})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({7, 11, 13, 14})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({8, 10, 14})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({9})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({9, 10, 14})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({10, 11, 13})));
    // Not normalized result
    double *expected = new double[id];
    id = 0;
    expected[id++] = 0;
    expected[id++] = 0;
    expected[id++] = 13;
    expected[id++] = 0;
    expected[id++] = 19 + 1.0 / 3;
    expected[id++] = 47.5;      //highest ID = 5;
    expected[id++] = 8 + 1.0 / 3;
    expected[id++] = 10 + 1.0 / 3;
    expected[id++] = 8.5;
    expected[id++] = 23 + 1.0 / 6;
    expected[id++] = 9 + 1.0 / 3;
    expected[id++] = 5;
    expected[id++] = 0;
    expected[id++] = 9.5;
    expected[id++] = 2;

    double *res = voyager->centrality((int) adj_matrix.size(), adj_matrix);
    // Check result size
    REQUIRE(sizeof(expected) == sizeof(res));
    // Check elements
    bool same_arr = true;
    for (int i = 0; i < id; i++) {
        if ((int)expected[i] * 10000 != (int)res[i] * 10000) {
            same_arr = false;
            break;
        }
    }
    // delete memory
    delete[] res;               res = nullptr;
    delete voyager;             voyager = nullptr;
    for (auto it : adj_matrix) {
        delete it.second;       it.second = nullptr;
    }
    REQUIRE(same_arr);
    
}

//src graph: ../tests/centrality_test_basic.jpg
TEST_CASE("Test Centrality basic", "[centrality][basic]") {

    Voyager *voyager = new Voyager();
    //init
    std::map<int, std::unordered_set<int>*> adj_matrix;
    int id = 0;
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({1, 2})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({0, 2, 3})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({0, 1, 3, 4})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({1, 2, 5, 6})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({2})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({3})));
    adj_matrix.insert(std::make_pair(id++, new std::unordered_set<int>({3})));

    // Not normalized result
    double *expected = new double[id];
    id = 0;
    expected[id++] = 0;
    expected[id++] = 1.5;
    expected[id++] = 6.5;
    expected[id++] = 9;
    expected[id++] = 0;
    expected[id++] = 0; 
    expected[id++] = 0;

    double *res = voyager->centrality((int) adj_matrix.size(), adj_matrix);
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
    
    // delete memory
    delete[] res;           res = nullptr;
    delete voyager;         voyager = nullptr;
    for (auto it : adj_matrix) {
        delete it.second;   it.second = nullptr;
    }
    REQUIRE(same_arr);
}

TEST_CASE("Test centrality large dataset", "[centrality][complex]") {

    // World's most connected airport LHR
    Voyager *voyager = new Voyager();
    int size = (int)voyager->GetAptDict().size();
    double *res = voyager->centrality(size, voyager->GetAdjMatrix());
    
    double LHR = res[502];
    int larger = 0;
    for (int i = 0; i < size; i++) {
        if (res[i] > LHR) larger++;
    }
    REQUIRE(larger < 20);
    delete voyager; voyager = nullptr;
}

TEST_CASE("Test coordinate conversion", "[visualization][basic]") {

    Voyager *voyager = new Voyager("dataset/testapt.dat", "dataset/testroutes");
    cs225::PNG png;
    png.readFromFile("worldMap.png");
    std::unordered_set<std::pair<int, int>, pair_hash> set;

    bool distinct_coor = true;
    std::map<int, Voyager::Airport*> apt_dict = voyager->GetAptDict();
    for (auto elem : apt_dict) {
        int x = voyager->convertToX(png, elem.second->lati_, elem.second->longi_);
        int y = voyager->convertToY(png, elem.second->lati_, elem.second->longi_);
        if (set.find(std::make_pair(x, y)) != set.end()) {
            distinct_coor = false;
            break;
        }
        else set.insert(std::make_pair(x, y));
    }
    REQUIRE(distinct_coor);
    delete voyager;     voyager = nullptr;
}

TEST_CASE("Test coordinate conversion 2", "[visualization][complex]") {
    Voyager *voyager = new Voyager();
    cs225::PNG png;
    png.readFromFile("worldMap.png");
    std::unordered_set<std::pair<int, int>, pair_hash> set;

    bool distinct_coor = true;
    std::map<int, Voyager::Airport*> apt_dict = voyager->GetAptDict();
    for (auto elem : apt_dict) {
        int x = voyager->convertToX(png, elem.second->lati_, elem.second->longi_);
        int y = voyager->convertToY(png, elem.second->lati_, elem.second->longi_);
        if (set.find(std::make_pair(x, y)) != set.end()) {
            distinct_coor = false;
            break;
        }
        else set.insert(std::make_pair(x, y));
    }
    REQUIRE(distinct_coor);
    delete voyager;     voyager = nullptr;
}