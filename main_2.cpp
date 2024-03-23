//
// Created by comet on 24. 3. 23.
//
#include <chrono>
#include <vector>
#include <fstream>
#include "city.h"
#include "utils.h"

int main() {
    int n = 500;
    City* cities = generateRandomCities(n);
    std::vector<City> tour(cities, cities + n);
    // Strategy 1: Choose the best swap
    std::vector<double> tourLengths1;
    auto start = std::chrono::high_resolution_clock::now();
    std::pair<int, int> swap;
    while ((swap = chooseBestSwap(tour, calculateWithoutSwap)).first != -1) {
        twoOptSwap(tour, swap.first, swap.second);
        tourLengths1.push_back(calculateTourLength(tour));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Strategy 1: Best Swap\n"; std::cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    std::cout << "Tour length: " << calculateTourLength(tour) << "\n";
    std::cout << "Tour lengths: "; for (double length : tourLengths1) { std::cout << length << " "; } std::cout << "\n";
    std::cout << "------------------------------------------------------------------\n";
    std::ostringstream filenameStream1;
    filenameStream1 << "strategy1_tour_lengths_" << n << ".txt";
    std::ofstream file1(filenameStream1.str());
    for (double length : tourLengths1) {
        file1 << length << " ";
    }
    file1.close();

    // Reset the tour
    tour = std::vector<City>(cities, cities + n);
    // Strategy 2: Choose the first improving swap
    std::vector<double> tourLengths2;
    start = std::chrono::high_resolution_clock::now();
    while ((swap = chooseFirstImprovingSwap(tour, calculateWithoutSwap)).first != -1) {
        twoOptSwap(tour, swap.first, swap.second);
        tourLengths2.push_back(calculateTourLength(tour));
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Strategy 2: First Improving Swap\n";
    std::cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    std::cout << "Tour length: " << calculateTourLength(tour) << "\n";
    std::cout << "------------------------------------------------------------------\n";
    std::ostringstream filenameStream2;
    filenameStream2 << "strategy2_tour_lengths_" << n << ".txt";
    std::ofstream file2(filenameStream2.str());
    for (double length : tourLengths2) {
        file2 << length << " ";
    }
    file2.close();

    std::ostringstream filenameStream;
    filenameStream << "sa_current_tour_lengths_" << n << ".txt";
    std::string filename = filenameStream.str();
    std::ofstream currentFile(filename);

    filenameStream.str("");
    filenameStream << "sa_best_tour_lengths_" << n << ".txt";
    filename = filenameStream.str();
    std::ofstream bestFile(filename);

    // Reset the tour
    tour = std::vector<City>(cities, cities + n);
    // Simulated Annealing
    start = std::chrono::high_resolution_clock::now();
    auto distanceRecords = simulatedAnnealing(tour, 10000, 0.0000005, false);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Simulated Annealing\n";
    std::cout << "Time elapsed: " << elapsed.count() << " seconds\n";
    std::cout << "Final tour length: " << calculateTourLength(tour) << "\n";

    currentFile << "Current Tour lengths:\n";
    bestFile << "Best Tour lengths:\n";
    for (const auto& record : distanceRecords) {
        currentFile << record.first << " ";
        bestFile << record.second << " ";
    }
    std::cout << "------------------------------------------------------------------\n";

    // Concorde solver
    std::string filename3 = "exp_2.tsp";
    writeTSPLIB(cities, n, filename3);

    // Replace the path with the path where concorde exe. is located
    std::string command = "/home/comet/opt/concorde/TSP/concorde " + filename3;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to run Concorde solver." << std::endl;
        delete[] cities;
        return 1;
    }

    delete[] cities;
    return 0;
}