#include <vector>
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Simulation.h"
using std::vector;

#include "SelectionPolicy.h"

// Base SelectionPolicy Implementation
SelectionPolicy::~SelectionPolicy() = default;

// NaiveSelection Implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive Selection";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this); // יצירת עותק באמצעות בנאי ההעתקה
}
 
 
 //BalancedSelection Implementation
BalancedSelection::BalancedSelection(int lifeQuality, int economy, int environment)
    : LifeQualityScore(lifeQuality), EconomyScore(economy), EnvironmentScore(environment) {}

const FacilityType& BalancedSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
 const FacilityType* bestFacility = nullptr;
    int minDifference = INT_MAX;

    for (const FacilityType& facility : facilitiesOptions) {
        int diff = std::abs((LifeQualityScore + facility.getLifeQualityScore()) - 
                            (EconomyScore + facility.getEconomyScore())) +
                   std::abs((LifeQualityScore + facility.getLifeQualityScore()) - 
                            (EnvironmentScore + facility.getEnvironmentScore())) +
                   std::abs((EconomyScore + facility.getEconomyScore()) - 
                            (EnvironmentScore + facility.getEnvironmentScore()));

        if (diff < minDifference) {
            minDifference = diff;
            bestFacility = &facility;
        }
    }

    LifeQualityScore += bestFacility->getLifeQualityScore();
    EconomyScore += bestFacility->getEconomyScore();
    EnvironmentScore += bestFacility->getEnvironmentScore();

    return *bestFacility;
}

const string BalancedSelection::toString() const {
    return "Balanced Selection";
}


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
