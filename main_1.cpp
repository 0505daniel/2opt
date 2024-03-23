//
// Created by comet on 24. 3. 22.
//
#include <chrono>
#include <vector>
#include "city.h"
#include "utils.h"

int main() {
    int n = 100;
    City* cities = generateRandomCities(n);
    std::vector<City> tour(cities, cities + n);
//    printTour(tour);

    // Implementation 1: Swap first, then calculate the new tour length
    auto start = std::chrono::high_resolution_clock::now();
    std::pair<int, int> swap;
    while ((swap = chooseBestSwap(tour, calculateWithSwap)).first != -1) {
        twoOptSwap(tour, swap.first, swap.second);
//        std::cout << swap.first << swap.second << "\n";
//        printTour(tour);
//        std::cout << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Implementation 1: Swap tour to calculate\n";
    std::cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    std::cout << "Final tour length: " << calculateTourLength(tour) << "\n";
    std::cout << "------------------------------------------------------------------\n";
    // Reset the tour
    tour = std::vector<City>(cities, cities + n);
//    printTour(tour);

    // Implementation 2: Calculate the potential improvement of a swap without actually performing the swap
    start = std::chrono::high_resolution_clock::now();
    while ((swap = chooseBestSwap(tour, calculateWithoutSwap)).first != -1) {
        twoOptSwap(tour, swap.first, swap.second);
//        std::cout << swap.first << swap.second << "\n";
//        printTour(tour);
//        std::cout << std::endl;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Implementation 2: Only Calculation\n";
    std::cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    std::cout << "Final tour length: " << calculateTourLength(tour) << "\n";
    std::cout << "------------------------------------------------------------------\n";


    // Concorde solver
    std::string filename = "exp_1.tsp";
    writeTSPLIB(cities, n, filename);

    // Replace the path with the path where concorde exe. is located
    std::string command = "/home/comet/opt/concorde/TSP/concorde " + filename;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to run Concorde solver." << std::endl;
        delete[] cities;
        return 1;
    }

    delete[] cities;
    return 0;
}