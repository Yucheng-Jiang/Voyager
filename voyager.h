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
             * equal operator overloading
             * @param other const reference of another airport
             */
            bool operator== (Airport const & other) const {
                return !(*this != other);
            }            

            /**
             * not equal operator
             * @param other const reference of another airport
             */ 
             bool operator!= (Airport const & other) const {
                return !(IATA == other.IATA 
                    && name == other.name 
                    && lati_ == other.lati_ 
                    && longi_ == other.longi_ 
                    && index == other.index);
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
         * Default constructor initialize airport dictionary and adjacency matrix.
         * Using default database.
         */ 
        Voyager();

        /**
         * Customized constructor.
         * Using user specified database.
         * @param AptPath relative path to airport database
         * @param RoutePath relative path to route database
         */
        Voyager(std::string AptPath, std::string RoutePath);

        /**
         * Draw flight routes onto png
         * @param airport_dict airport dictionary
         * @param centrality  double array of centrality of each airport
         * @param inputFile inputFile name worldMap.png
         * @param outputFile outputFile name
         */ 

        void DrawGraph(std::map<int, Airport*>& airport_dict, double* centrality, std::string inputFile, std::string outputFile);

        /**
         * Calculate betweeness centriality of each airport.
         * @param SIZE total number of airport
         * @param map adjacency matrix.
         * @return array of int representing centrailiaty of each airport
         */ 
        double* CalculateCentrality(int SIZE, std::map<int, std::unordered_set<int>*>& map);

        /**
         * Draw flight routes onto png
         * @param airport_dict airport dictionary
         * @param centrality  double array of centrality of each airport
         * @param inputFile inputFile name worldMap.png
         * @param outputFile outputFile name
         * @param isAnimation whether generate animation gif
         * @param FRAME_COUNT frame count const of gif
         */ 

        void DrawAnimation(std::map<int, Airport*>& airport_dict, double* centrality, 
                                std::string inputFile, std::string outputFile, bool isAnimation, int FRAME_COUNT);

        /**
         * destructor. delete aiport dictionary and adjacency matrix.
         */ 
        ~Voyager();

        /**
         * Airport dictionary getter
         * @return reference of the airport dictionary
         */ 
        std::map<int, Airport*>& GetAptDict();

        /**
         * Adjacency map getter
         * @return reference to the adjacency map
         */
        std::map<int, std::unordered_set<int>*>& GetAdjMatrix();

        //public for testing purposes
        /**
         * Ronvert longti and lati into 2D map.
         * @param png the 2D map
         * @param lati latitude of the airport
         * @param longi longtide of the airport
         * @return the X coordinate of the airport on the 2D map
         * @return the Y coordinate of the airport on the 2D map
         */
        double ConvertToX(int width, int height, double longi);
        double ConvertToY(int width, int height, double lati);
        
    private:
        // Airport dictionary using airport unique id as key and airport pointer as value.
        std::map<int, Airport*> airport_dict;   
        // adjacency map. key is the id of airport, value is unordered_set of all neighbor aiports
        std::map<int, std::unordered_set<int>*> map_adj; 

    private:
        /**
         * Read airport info from dataset to airport_dict.
         * initialize adjacency matrix based on number of airports.
         * @param filepath string path the airport info file.
         */ 
        void ReadAirport(std::string filePath);

        /**
         * Update adjacency matrix based on routes.
         * @param filepath string path the routes info file.
         */ 
        void ReadRoute(std::string filePath);
};