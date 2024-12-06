#include "Settlement.h"

// Constructor
Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {}

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

// Return a string representation of the settlement
const string Settlement::toString() const {
    string typeString;
    switch (type) {
        case SettlementType::VILLAGE:
            typeString = "Village";
            break;
        case SettlementType::CITY:
            typeString = "City";
            break;
        case SettlementType::METROPOLIS:
            typeString = "Metropolis";
            break;
    }
    return "Settlement " + name + typeString;
}


int Settlement::getConstructionLimit() const {
    if (type == SettlementType::VILLAGE) {
        return 1; 
    } else if (type == SettlementType::CITY) {
        return 2;
    } else if (type == SettlementType::METROPOLIS) {
        return 3;
    } else {
        return 0;
    }
}