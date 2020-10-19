//
//  facility_location_model.hpp
//  facility_location_problem
//
//  Created by Shuotao Diao on 10/18/20.
//

#ifndef facility_location_model_hpp
#define facility_location_model_hpp

#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

// struct variable
struct location_decision {
    std::vector<int> open_facility;
    std::vector<std::vector<int>> serve_client;
};


struct location {
    double x, y;
};

//
location_decision facility_location_solver(const std::vector<double>& cost_open, const std::vector<std::vector<double>>& distance);

std::vector<std::vector<double>> distance_generator(const std::vector<location>& client_location, const std::vector<location>& facility_location);

std::vector<location> location_generator(int num_locations);

location_decision sample_generator(int num_client, int num_facility, const std::string& outputPath);

void traning_set_generator(int num_client, int num_facility, int num_samples, const std::string& outputPath);
#endif /* facility_location_model_hpp */
