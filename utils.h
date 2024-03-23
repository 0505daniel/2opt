//
// Created by comet on 24. 3. 22.
//
#pragma once
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include "city.h"

inline double distance(const City& a, const City& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline std::string itos(int i) {
    std::stringstream s;
    s << i;
    return s.str();
}

void twoOptSwap(std::vector<City>& tour, int i, int k);

double calculateTourLength(std::vector<City>& tour) ;

// Method 1: Swap first, then calculate the new tour length
double calculateWithSwap(std::vector<City>& tour, int i, int k);

// Method 2: Calculate the potential improvement of a swap without actually performing the swap
double calculateWithoutSwap(std::vector<City>& tour, int i, int k);

typedef double (*CalculateMethod)(std::vector<City>&, int, int);
// Strategy 1: Among all possible 2-opt swaps, choose the one with the best improvement
std::pair<int, int> chooseBestSwap(std::vector<City>& tour, CalculateMethod calculate);

// Strategy 2: Accept the first 2-opt swap that improves the tour
std::pair<int, int> chooseFirstImprovingSwap(std::vector<City>& tour, CalculateMethod calculate);

std::vector<std::pair<double, double>> simulatedAnnealing(std::vector<City>& tour, double initialTemperature, double coolingRate, bool useWarmStart);

void printTour(const std::vector<City>& tour);

City* generateRandomCities(int n);

void writeTSPLIB(City* cities, int n, const std::string& filename);