#include "Facility.h"

// Constructor for FacilityType
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, 
                           const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), 
      lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

// Getters for FacilityType
const string &FacilityType::getName() const {
    return name;
}

int FacilityType::getCost() const {
    return price;
}

int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return category;
}

// Constructor for Facility
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, 
                   const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}

// Getters for Facility
const string &Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus Facility::step() {
    // TODO: Reduce timeLeft by one and update the status if construction is complete
}

void Facility::setStatus(FacilityStatus status) {
    // TODO: Set the status of the facility
}

const FacilityStatus& Facility::getStatus() const {
    // TODO: Return the status of the facility
}

const string Facility::toString() const {
    // TODO: Return a string representation of the facility
}
