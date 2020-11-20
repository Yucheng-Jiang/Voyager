#include <string>
#include <vector>
#include <map>

#include "../catch/catch.hpp"
#include "../voyager.h"
#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

//TODO: Add more test and testing strategy

TEST_CASE("Test Init", "[apt-dict init][route init][init]") {

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
    Voyager::Airport* GKA = new Voyager::Airport("Goroka Airport", "GKA", -6.081689835, 145.3919983, 1);
    Voyager::Airport* HGH = new Voyager::Airport("Hangzhou Xiaoshan International Airport", "HGH", 30.22949982, 120.4339981, 3386);
    Voyager::Airport* CTU = new Voyager::Airport("Chengdu Shuangliu International Airport", "CTU", 30.57850075, 103.9469986, 3395);
    Voyager::Airport* AB = new Voyager::Airport("Melitopol Air Base", "\N", 46.880001, 35.305, 14110);

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
    REQUIRE(GKA == A_GKA);
    REQUIRE(HGH == A_HGH);
    REQUIRE(CTU == A_CTU);
    REQUIRE(AB == A_AB);
    //Clean up
    delete GKA;     GKA = nullptr;
    delete HGH;     HGH = nullptr;
    delete CTU;     CTU = nullptr;
    delete AB;      AB = nullptr;
    delete voyager; voyager = nullptr;
}