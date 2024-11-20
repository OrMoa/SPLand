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
    return true;
}

bool Simulation::addFacility(FacilityType facility){
    
}


bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement &settlement : settlements) {
        if (settlement.getName() == settlementName) {
            return true;
        }
    }
    return false;
}