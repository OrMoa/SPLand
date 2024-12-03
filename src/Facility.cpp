#include "Facility.h"

// Constructor for FacilityType
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, 
                           const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), 
      lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

FacilityType::FacilityType(const FacilityType &other)
    : name(other.name), 
      category(other.category), 
      price(other.price), 
      lifeQuality_score(other.lifeQuality_score), 
      economy_score(other.economy_score), 
      environment_score(other.environment_score) {}

FacilityType& FacilityType::operator=(const FacilityType& other) {
    if (this == &other) {
        return *this;  // מונע העתקה עצמית
    }
    // מחיקת האובייקט הנוכחי
    this->~FacilityType();  // קריאה לבנאי ההשמדת האובייקט הנוכחי
    // יצירת מופע חדש במקום האובייקט הנוכחי
    new (this) FacilityType(other);  // Placement new: יצירת אובייקט חדש במקום האובייקט הנוכחי
    return *this;  // מחזיר את המצביע לאובייקט הנוכחי
}

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

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}

Facility::Facility(const Facility &other)
    : FacilityType(other), // קריאה לבנאי המעתיק של FacilityType
      settlementName(other.settlementName), 
      status(other.status), 
      timeLeft(other.timeLeft) {}
      
const string &Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus Facility::step() {
    if (timeLeft > 0) {
        --timeLeft;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus status) {
    this->status = status;
}

const FacilityStatus& Facility::getStatus() const {
    return status;
}

const string Facility::toString() const {
    string statusString;
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS) 
          statusString = "UNDER_CONSTRUCTIONS";
    else
           statusString = "OPERATIONAL";

    return "FacilityName: " + getName() + "\n" +
           "FacilityStatus: " + statusString;
}
