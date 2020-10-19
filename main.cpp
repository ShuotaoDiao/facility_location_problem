//
//  main.cpp
//  facility_location_problem
//
//  Created by Shuotao Diao on 10/18/20.
//

#include <iostream>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>

#include "facility_location_model.hpp"

void testCplex(){
    std::cout << "Test on cplex solver\n";
    // set up cplex environment
    IloEnv env;
    IloModel mod(env);
    // decision variables
    IloNumVar x(env,-1,1,ILOFLOAT);
    mod.add(x);
    // objective function
    IloExpr expr_obj(env);
    expr_obj = (x - 0.5) * (x - 0.5);
    IloObjective obj = IloMinimize(env, expr_obj);
    mod.add(obj);
    // cplex solver setup
    IloCplex cplex(env);
    cplex.extract(mod);
    cplex.solve();
    std::cout << "Minimum is " << cplex.getObjValue() << std::endl;
    std::cout << "x: " << cplex.getValue(x) << std::endl;
    // end the environment
    env.end();
}

int main(int argc, const char * argv[]) {
    /*
    std::vector<double> cost;
    for (int j = 0; j < 4; ++j) {
        cost.push_back(j);
    }
    std::vector<std::vector<double>> distance;
    for (int i = 0; i < 2; ++i) {
        std::vector<double> distance_tmp;
        for (int j = 0; j < 4; ++j) {
            distance_tmp.push_back(i + j);
        }
        distance.push_back(distance_tmp);
    }
    
    location_decision optimal_location_decision = facility_location_solver(cost, distance);
     */
    int num_clients = 4;
    int num_facilities = 8;
    int num_samples = 10;
    std::string outputPath = "/Users/sonny/Documents/Courses/CSCI566/experiment/training_set/facility_location_01.txt";
    traning_set_generator(num_clients, num_facilities, num_samples, outputPath);
    return 0;
}
