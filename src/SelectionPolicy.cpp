#include <vector>
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Simulation.h"
using std::vector;

#include "SelectionPolicy.h"
//#include <stdexcept>
//#include <algorithm>

// Base SelectionPolicy Implementation
SelectionPolicy::~SelectionPolicy() = default;

// NaiveSelection Implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        //למלא("Error: No facilities available for selection.");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive Selection";
}

/* BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQuality, int economy, int environment)
    : LifeQualityScore(lifeQuality), EconomyScore(economy), EnvironmentScore(environment) {}

const FacilityType& BalancedSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        //למלא("Error: No facilities available for selection.");
    }

    auto minDiffFacility = std::min_element(facilitiesOptions.begin(), facilitiesOptions.end(),
        [this](const FacilityType& a, const FacilityType& b) {
            int diffA = std::abs((LifeQualityScore + a.lifeQualityImpact) - (EconomyScore + a.economyImpact) - (EnvironmentScore + a.environmentImpact));
            int diffB = std::abs((LifeQualityScore + b.lifeQualityImpact) - (EconomyScore + b.economyImpact) - (EnvironmentScore + b.environmentImpact));
            return diffA < diffB;
        });

    return *minDiffFacility;
}

std::string BalancedSelection::toString() const {
    return "Balanced Selection";
}*/

// EconomySelection Implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    int startIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    for (int i = 0; i < facilitiesOptions.size(); ++i) {
        int currentIndex = (startIndex + i) % facilitiesOptions.size();
        if (facilitiesOptions[currentIndex].getCategory() == static_cast<FacilityCategory>(1)) {
            lastSelectedIndex = currentIndex;
            return facilitiesOptions[currentIndex];
        }
    }
}

const string EconomySelection::toString() const {
    return "Economy Selection";
}

// SustainabilitySelection Implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
     int startIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    for (int i = 0; i < facilitiesOptions.size(); ++i) {
        int currentIndex = (startIndex + i) % facilitiesOptions.size();
        if (facilitiesOptions[currentIndex].getCategory() == static_cast<FacilityCategory>(2)) {
            lastSelectedIndex = currentIndex;
            return facilitiesOptions[currentIndex];
        }
    }
}

const string SustainabilitySelection::toString() const {
    return "Sustainability Selection";
}
