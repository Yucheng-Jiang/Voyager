#pragma once
#include <string>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <map>

class Voyager {

    //TODO: Modify controlled access if necessary
    public:
        Voyager();
        cs225::PNG* DrawGraph(short** matrix);
        int* centrality(short** matrix);
        ~Voyager();
    private:
        struct Airport {
            // constructor
            Airport(string setName, string setIATA, double setLat, double setLong) {
                lati_ = setLat;
                longi_ = setLong;
                IATA = setIATA;
                name = setName;
            }
            // copy helper
            void _copy(Airport const & other) {
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
                return IATA == other.IATA && name == other.name && lati_ == other.lati_ && longi_ == other.longi_;
            }
            string IATA;
            string name;
            double lati_;
            double longi_;
        };
        
        
        std::map<int, Airport*> airport_dict;   // Airport dictionary
        short** matrix_adj;                     // Adjacency matrix

        // Helper func
        // TODO: Add Helper here
        void ReadAirport(std::string filePath);
        void ReadRoute(std::string filePath);
        void DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y);
};