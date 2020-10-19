//
//  facility_location_model.cpp
//  facility_location_problem
//
//  Created by Shuotao Diao on 10/18/20.
//

#include "facility_location_model.hpp"

location_decision facility_location_solver(const std::vector<double>& cost_open, const std::vector<std::vector<double>>& distance) {
    std::cout << "Facility Location Problem" << std::endl;
    std::cout << "Initialize CPLEX Environment" << std::endl;
    // measure the scale of the problem
    int num_facilities = cost_open.size();
    int num_clients = distance.size();
    // set up cplex environment
    IloEnv env;
    IloModel mod(env);
    // decision variable
    IloNumVarArray open_facility(env, num_facilities, 0, 1, ILOINT);
    std::vector<IloNumVarArray> serve;
    mod.add(open_facility);
    for (int index_client = 0; index_client < num_clients; index_client++) {
        IloNumVarArray serve_tmp(env, num_facilities, 0, 1, ILOINT);
        serve.push_back(serve_tmp);
        mod.add(serve[index_client]);
    }
    // objective
    IloExpr expr_obj(env);
    for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
        expr_obj += cost_open[index_facility] * open_facility[index_facility];
    }
    for (int index_client = 0; index_client < num_clients; index_client++) {
        for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
            expr_obj += distance[index_client][index_facility] * serve[index_client][index_facility];
        }
    }
    IloObjective obj = IloMinimize(env, expr_obj);
    mod.add(obj);
    // constraints
    for (int index_client = 0; index_client < num_clients; index_client++) {
        IloExpr expr_serve_con(env);
        for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
            expr_serve_con += serve[index_client][index_facility];
            IloExpr expr_open_con(env);
            expr_open_con += serve[index_client][index_facility] - open_facility[index_facility];
            mod.add(expr_open_con <= 0);
        }
        mod.add(expr_serve_con == 1);
    }
    // set up cplex environment
    IloCplex cplex(env);
    cplex.extract(mod);
    cplex.solve();
    // obtain optimal solutions
    location_decision optimal_solution;
    for (int index_client = 0; index_client < num_clients; index_client++) {
        std::vector<int> tmp_serve;
        for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
            tmp_serve.push_back(cplex.getValue(serve[index_client][index_facility]));
            std::cout << "serve[" << index_client << "][" << index_facility;
            std::cout << "] = " << cplex.getValue(serve[index_client][index_facility]) << std::endl;
        }
        optimal_solution.serve_client.push_back(tmp_serve);
    }
    for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
        optimal_solution.open_facility.push_back(cplex.getValue(open_facility[index_facility]));
        std::cout << "open_facility[" << index_facility << "] = " << cplex.getValue(open_facility[index_facility]) << std::endl;
    }
    // end environment
    std::cout << "End CPLEX environment." << std::endl;
    env.end();
    return optimal_solution;
}


std::vector<std::vector<double>> distance_generator(const std::vector<location>& client_location, const std::vector<location>& facility_location) {
    int num_clients = client_location.size();
    int num_facilities = facility_location.size();
    std::vector<std::vector<double>> distance;
    for (int index_client = 0; index_client < num_clients; index_client++) {
        std::vector<double> distance_tmp;
        for (int index_facility = 0; index_facility < num_facilities; index_facility++) {
            double cur_distance = 0;
            cur_distance = (client_location[index_client].x - facility_location[index_facility].x) * (client_location[index_client].x - facility_location[index_facility].x);
            cur_distance += (client_location[index_client].y - facility_location[index_facility].y) * (client_location[index_client].y - facility_location[index_facility].y);
            distance_tmp.push_back(cur_distance);
        }
        distance.push_back(distance_tmp);
    }
    return distance;
}


std::vector<location> location_generator(int num_locations) {
    std::vector<location> location_res;
    for (int index = 0; index < num_locations; index++) {
        location cur_location;
        cur_location.x = ((double) rand()) / RAND_MAX;
        cur_location.y = ((double) rand()) / RAND_MAX;
        location_res.push_back(cur_location);
    }
    return location_res;
}


location_decision sample_generator(int num_client, int num_facility, const std::string& outputPath) {
    // client location
    std::vector<location> client_location = location_generator(num_client);
    // facility location
    std::vector<location> facility_location = location_generator(num_facility);
    // cost (constant = 1)
    double const_cost = 0.1;
    std::vector<double> cost;
    for (int index_facility = 0; index_facility < num_facility; index_facility++) {
        cost.push_back(const_cost);
    }
    // distance
    std::vector<std::vector<double>> distance = distance_generator(client_location, facility_location);
    // location decsion
    location_decision optimal_location_decision = facility_location_solver(cost, distance);
    // write sample
    std::fstream writeTrainingSet;
    writeTrainingSet.open(outputPath, std::fstream::app);
    writeTrainingSet << "=== Sample (Start) ===\n";
    //feature
    writeTrainingSet << "Feature (Client_location x axis): ";
    for (int index_client = 0; index_client < num_client - 1; index_client++) {
        writeTrainingSet << client_location[index_client].x << ", ";
    }
    writeTrainingSet << client_location[num_client - 1].x << std::endl;
    writeTrainingSet << "Feature (Client_location y axis): ";
    for (int index_client = 0; index_client < num_client - 1; index_client++) {
        writeTrainingSet << client_location[index_client].y << ", ";
    }
    writeTrainingSet << client_location[num_client - 1].y << std::endl;
    writeTrainingSet << "Feature (Facility_location x axis): ";
    for (int index_facility = 0; index_facility < num_facility - 1; index_facility++) {
        writeTrainingSet << facility_location[index_facility].x << ", ";
    }
    writeTrainingSet << facility_location[num_facility - 1].x << std::endl;
    writeTrainingSet << "Feature (Facility_location y axis): ";
    for (int index_facility = 0; index_facility < num_facility - 1; index_facility++) {
        writeTrainingSet << facility_location[index_facility].y << ", ";
    }
    writeTrainingSet << facility_location[num_facility - 1].y << std::endl;
    writeTrainingSet << "Feature (facility open cost): ";
    for (int index_facility = 0; index_facility < num_facility - 1; index_facility++) {
        writeTrainingSet << cost[index_facility] << ", ";
    }
    writeTrainingSet << cost[num_facility - 1] << std::endl;
    // response
    writeTrainingSet << "Response (Open_facility): ";
    for (int index_facility = 0; index_facility < num_facility - 1; index_facility++) {
        writeTrainingSet << optimal_location_decision.open_facility[index_facility] << ", ";
    }
    writeTrainingSet << optimal_location_decision.open_facility[num_facility - 1] << std::endl;
    writeTrainingSet << "Response (serve): " << std::endl;
    for (int index_client = 0; index_client < num_client; index_client++) {
        for (int index_facility = 0; index_facility < num_facility - 1; index_facility++) {
            writeTrainingSet << optimal_location_decision.serve_client[index_client][index_facility] << ", ";
        }
        writeTrainingSet << optimal_location_decision.serve_client[index_client][num_facility - 1] << std::endl;
    }
    writeTrainingSet << "=== Sample (End) ===\n";
    return optimal_location_decision;
}

void traning_set_generator(int num_client, int num_facility, int num_samples, const std::string& outputPath) {
    for (int index_sample = 0; index_sample < num_samples; ++index_sample) {
        sample_generator(num_client, num_facility, outputPath);
    }
}
