#pragma once
#include <string>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <map>
#include <vector>

class Voyager {

    //TODO: Modify controlled access if necessary
    public:
        struct Airport {
            // constructor
            Airport(string setName, string setIATA, double setLat, double setLong, int setIndex) {
                index = setIndex;
                lati_ = setLat;
                longi_ = setLong;
                IATA = setIATA;
                name = setName;
            }
            // copy helper
            void _copy(Airport const & other) {
                index = other.index;
                IATA = other.IATA;
                name = other.name;
                lati_ = other.lati_;
                longi_ = other.longi_;
            }
            // copy constructor
            Airport(Airport const & other) {
                _copy(other);
            }
            // copy assignment
            Airport const & operator=(Airport const & other) {
                if (this != &other) { _copy(other); }
                return *this;
            }
            // not equal operator
             bool operator!= (Airport const & other) const {
                return IATA == other.IATA 
                    && name == other.name 
                    && lati_ == other.lati_ 
                    && longi_ == other.longi_ 
                    && index == other.index;
            }
            int index;
            string IATA;
            string name;
            double lati_;
            double longi_;
        };
    public:
        Voyager();
        cs225::PNG* DrawGraph(short** matrix);
        int* centrality(short** matrix);
        ~Voyager();
        std::map<int, Airport*> getAptDict();
        
    private:
        
        // Enum to represent relative route direction
        enum Dir { NORTH, EAST, SOUTH, WEST};   // NORTH, SOUTH are unlikely
        
        std::map<int, Airport*> airport_dict;   // Airport dictionary
        short** matrix_adj;                     // Adjacency matrix

        // Helper func
        // TODO: Add Helper here
        void ReadAirport(std::string filePath);
        void ReadRoute(std::string filePath);
        void DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y);
        /**
         * Helper func for draw line, calculates relative dir and distance(pixel).
         * @param all coordinate of source airport and destination airport
         * @return a pair of direction and pixel distance
         **/
        std::pair<Dir, int> GetDirDist(int src_x, int src_y, int dest_x, int dest_y);
};