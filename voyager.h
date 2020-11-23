#pragma once
#include <string>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <map>
#include <vector>
#include <unordered_set>

class Voyager {

    public:
        struct Airport {
            /**
             * constructor
             * @param setName string name of airport
             * @param setIATA 3-character string of IATA code
             * @param setLat latitude of the airport in double
             * @param setLong longitutde of the airport in double
             * @param setIndex index of the airport in dataset
             */ 
            Airport(string setName, string setIATA, double setLat, double setLong, int setIndex) {
                index = setIndex;
                lati_ = setLat;
                longi_ = setLong;
                IATA = setIATA;
                name = setName;
            }

            /** 
             * copy helper function
             * @param other const reference of another airport
             */ 
            void _copy(Airport const & other) {
                index = other.index;
                IATA = other.IATA;
                name = other.name;
                lati_ = other.lati_;
                longi_ = other.longi_;
            }

            /**
             * copy constructor
             * @param other const reference of another airport
             */ 
            Airport(Airport const & other) {
                _copy(other);
            }

            /** 
             * copy assignment
             * @param other const reference of another airport
             */ 
            Airport const & operator=(Airport const & other) {
                if (this != &other) { _copy(other); }
                return *this;
            }

            /**
             * not equal operator
             * @param other const reference of another airport
             */ 
             bool operator!= (Airport const & other) const {
                return IATA == other.IATA 
                    && name == other.name 
                    && lati_ == other.lati_ 
                    && longi_ == other.longi_ 
                    && index == other.index;
            }
            
            // the index in the dataset
            int index;
            // 3-character code of the airport
            string IATA;
            // name of the airport
            string name;
            //latitude of the airport
            double lati_;
            // longitude of the airport
            double longi_;
        };

    public:
        /**
         * constructor initialize airport dictionary and adjacency matrix.
         */ 
        Voyager();

        /**
         * draw flight routes onto png
         * @param map adjacency matrix. 
         * @return pointer of the updated PNG
         */ 
        cs225::PNG* DrawGraph( std::map<int, std::unordered_set<int>*>& map);

        /**
         * calculate betweeness centriality of each airport.
         * @param map adjacency matrix.
         * @return array of int representing centrailiaty of each airport
         */ 
        double* centrality( std::map<int, std::unordered_set<int>*>& map);

        /**
         * destructor. delete aiport dictionary and adjacency matrix.
         */ 
        ~Voyager();

        /**
         * airport dictionary getter
         * @return reference of the airport dictionary
         */ 
        std::map<int, Airport*>& getAptDict();
        
    private:
        // Airport dictionary using airport unique id as key and airport pointer as value.
        std::map<int, Airport*> airport_dict;   
        // adjacency map. key is the id of airport, value is unordered_set of all neighbor aiports
        std::map<int, std::unordered_set<int>*> map_adj; 

    private:
        /**
         * read airport info from dataset to airport_dict
         * initialize adjacency matrix based on number of airports.
         * @param filepath string path the airport info file.
         */ 
        void ReadAirport(std::string filePath);

        /**
         * update adjacency matrix based on routes.
         * @param filepath string path the routes info file.
         */ 
        void ReadRoute(std::string filePath);

        /**
         * draw lines onto png from source to destination.
         * @param src_x x coordinate of starting pixel on png.
         * @param src_y y coordinate of staring pixel on png.
         * @param dest_x x coordinate of ending pixel on png.
         * @param dest_y y coordinate of ending pixel on png.
         */
        void DrawLine(cs225::PNG &png, int src_x, int src_y, int dest_x, int dest_y);


        /**
         * get minimum steps of each airport from departure airport. 
         * if find destination airport immediately stop count.
         * @param map adjacency map
         * @param departureIndex id of departure index
         * @param destIndex if od destination index
         * @return array of int representing minimum steps from departure airport
         */ 
        int* getStepCount(std::map<int, std::unordered_set<int>*>& map, int departureIndex, int destIndex);

        /**
         * calculate number of each airport appears in all shortest path from departure to destination airport
         * @param map adjacency map
         * @param stepCount minumum steps of each airport from departure airport
         * @param departureIndex id of departure airport
         * @param destIndex id of destination airport.
         * @return array of int representing number of each airport appears in all shortest path from departure to dest airport.
         */ 
        int* getPathCount(std::map<int, std::unordered_set<int>*>& map, int* stepCount, int departureIndex, int destIndex);
};