#include "Simulation.h"
#include "Action.h"
#include "Facility.h"
#include "Settlement.h"
#include "Plan.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0){}

bool Simulation::addSettlement(Settlement settlement) {
    if (isSettlementExists(settlement.getName())) {
        cout << "Settlement already exists." << endl;
        return false;
    }
    settlements.push_back(settlement);
    cout << "Settlement " << settlement.getName() << " added successfully." << endl;
    return true;
}

bool Simulation::addFacility(FacilityType facility){
    if (isFacilityExists(facility.getName())){
        cout << "Facility already exists." << endl;
        return false;
    }
    // the sentence is missing 

    facilitiesOptions.push_back(facility);
    cout << "Facility " << facility.getName() << " added successfully." << endl;
    return true;

}

//we added
bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement &settlement : settlements) {
        if (settlement.getName() == settlementName) {
            return true;
        }
    }
    return false;
}
//we added
bool Simulation::isFacilityExists(const string &facilityName) {
    for (const FacilityType &facility : facilitiesOptions) {
        if (facility.getName() == facilityName) {
            return true; 
        }
    }
    return false; 
}  
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0){}

bool Simulation::addSettlement(Settlement settlement) {
    if (isSettlementExists(settlement.getName())) {
        cout << "Settlement already exists." << endl;
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    if (!isSettlementExists(settlement.getName())) {
        cout << "Cannot create this plan- settlement does not exist" << endl;
        return;
    }
     const std::string policyName = selectionPolicy->toString();
    if (policyName != "NaiveSelection" && 
        policyName != "BalancedSelection" && 
        policyName != "EconomySelection" && 
        policyName != "SustainabilitySelection") {
        std::cout << "Cannot create this plan- invalid selection policy" << std::endl;
        return;
    }
    Plan newPlan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
}