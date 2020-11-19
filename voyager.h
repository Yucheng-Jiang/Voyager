#pragma once
#include <string>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <map>

class voyager {

    //TODO: Modify controlled access if necessary
    public:
        void init();
        cs225::PNG* DrawGraph();
        int* centrality(short** matrix);
    private:
        // Data Structure
        struct airport {
            airport(int index, double lati, double longi) {
                idx = index;
                lat = lati;
                lon = longi;
            }
            int idx;
            double lat;
            double lon;
        };
        
        
        std::map<std::string, airport> apt;     // Airport dictionary
        short** matrix_adj;           // Adjacency matrix

        // Helper func
        // TODO: Add Helper here
        void ReadAirport(std::string filePath);
        void ReadRoute(std::string filePath);
        void DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y);
};