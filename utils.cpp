//
// Created by comet on 24. 3. 22.
//
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include "utils.h"


void twoOptSwap(std::vector<City>& tour, int i, int k) {
    std::reverse(tour.begin() + i + 1, tour.begin() + k + 1);
}

double calculateTourLength(std::vector<City>& tour) {
    double length = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        length += distance(tour[i], tour[i + 1]);
    }
    length += distance(tour.back(), tour.front()); // return to the first city
    return length;
}

// Method 1: Swap first, then calculate the new tour length
double calculateWithSwap(std::vector<City>& tour, int i, int k) {
    std::vector<City> newTour = tour;
    twoOptSwap(newTour, i, k);
    return calculateTourLength(tour) - calculateTourLength(newTour) ;
}

// Method 2: Calculate the potential improvement of a swap without actually performing the swap
double calculateWithoutSwap(std::vector<City>& tour, int i, int k) {
    double oldDistance = distance(tour[i], tour[i + 1]) + distance(tour[k], tour[(k + 1) % tour.size()]);
    double newDistance = distance(tour[i], tour[k]) + distance(tour[i + 1], tour[(k + 1) % tour.size()]);
    return oldDistance - newDistance;
}

// Strategy 1: Among all possible 2-opt swaps, choose the one with the best improvement
std::pair<int, int> chooseBestSwap(std::vector<City>& tour, CalculateMethod calculate) {
    double bestValue = 0;
    std::pair<int, int> bestSwap(-1, -1);
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        for (size_t k = i + 1; k < tour.size(); ++k) {
            double value = calculate(tour, i, k);
            if (value > bestValue) {
                bestValue = value;
                bestSwap = std::make_pair(i, k);
            }
        }
    }
    return bestSwap;
}

// Strategy 2: Accept the first 2-opt swap that improves the tour
std::pair<int, int> chooseFirstImprovingSwap(std::vector<City>& tour, CalculateMethod calculate) {
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        for (size_t k = i + 1; k < tour.size(); ++k) {
            double value = calculate(tour, i, k);
            if (value > 0) {
                return std::make_pair(i, k);
            }
        }
    }
    return std::make_pair(-1, -1); // no improving swap found
}

double acceptanceProbability(double delta, double temperature) {
    if (delta > 0) {
        return 1.0; // Always accept improvements
    } else {
        // For non-improvements, use a function that increases with temperature
        // This is a conventional approach, assuming delta < 0 indicates worsening
        return exp(delta / temperature); // Note the flipped sign in the exponent
    }
}


std::vector<std::pair<double, double>> simulatedAnnealing(std::vector<City>& tour, double initialTemperature, double coolingRate, bool useWarmStart) {

    if (useWarmStart) {
        // Warm start: Optimize the tour using the first improving swap before starting the annealing process
        std::pair<int, int> swap;
        while ((swap = chooseFirstImprovingSwap(tour, calculateWithoutSwap)).first != -1) {
            twoOptSwap(tour, swap.first, swap.second);
        }
    }

    //    // Shuffle the tour to start with a random sequence
    //    std::srand(unsigned(std::time(0)));
    //    std::random_shuffle(tour.begin(), tour.end());

    double temperature = initialTemperature;
    double currentDistance = calculateTourLength(tour);
    double bestDistance = currentDistance;

    // Container to record distances
    std::vector<std::pair<double, double>> distanceRecord;

    while (temperature > 1) {
        int i = rand() % tour.size();
        int k = (i + 1 + rand() % (tour.size() - 1)) % tour.size();
        double delta = calculateWithoutSwap(tour, i, k);
        if (acceptanceProbability(delta * 1000, temperature) > ((double) rand() / RAND_MAX)) {
            twoOptSwap(tour, i, k);
            currentDistance = calculateTourLength(tour);
            if (currentDistance < bestDistance){
                bestDistance = currentDistance;
            }
        }
        distanceRecord.push_back(std::make_pair(currentDistance, bestDistance));
        temperature *= (1 - coolingRate);
    }

    return distanceRecord;
}


// For debugging; Print the tour w.r.t the city id
void printTour(const std::vector<City>& tour) {
    for (const auto& city : tour) {
        std::cout << city.id << " ";
    }
    std::cout << std::endl;
}

City* generateRandomCities(int n) {
    srand(static_cast<unsigned int>(time(0)));
    City* cities = new City[n];
    for (int i = 0; i < n; ++i) {
        cities[i].id = i;
        cities[i].x = static_cast<double>(rand()) / RAND_MAX;
        cities[i].y = static_cast<double>(rand()) / RAND_MAX;
    }
    return cities;
}

void writeTSPLIB(City* cities, int n, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::filesystem::path p = std::filesystem::absolute(filename);
    std::cout << "File created at: " << p << std::endl;
    file << "NAME : TSP\n";
    file << "TYPE : TSP\n";
    file << "DIMENSION : " << n << "\n";
    file << "EDGE_WEIGHT_TYPE : EUC_2D\n";
    file << "NODE_COORD_SECTION\n";
    for (int i = 0; i < n; ++i) {
        file << (i+1) << " " << cities[i].x * 1000 << " " << cities[i].y * 1000 << "\n";
    }
    file << "EOF\n";
    file.close();
}


