#include "Simulation.h"
#include "Action.h"
#include "Facility.h"
#include "Settlement.h"
#include "Plan.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Auxiliary.h"

using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0){
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open config file.");
    }
    string line;
    while (std::getline(file, line)) {
        vector<string> arguments = Auxiliary::parseArguments(line);

        if (arguments.empty()) {
            continue; // דילוג על שורות ריקות
        }

        const std::string &type = arguments[0];

        if (type == "settlement") {
            createSettlement(arguments);
        } else if (type == "facility") {
            createFacility(arguments);
        } else if (type == "plan") {
            createPlan(arguments);
        } else {
            throw std::runtime_error("Error: Unknown line type in config file.");
        }
    }

    file.close();
    
}

void Simulation::createSettlement(const vector<string> &args) {
    if (args.size() != 3) {
        throw std::runtime_error("Error: Invalid settlement line format.");
    }

    const std::string &name = args[1];
    int type = std::stoi(args[2]);

    if (isSettlementExists(name)) {
        throw std::runtime_error("Error: Settlement already exists.");
    }

    SettlementType settlementType = static_cast<SettlementType>(type);
    Settlement *newSettlement = new Settlement(name, settlementType);
    settlements.push_back(newSettlement);
}

void Simulation::createFacility(const std::vector<std::string> &args) {
    if (args.size() != 7) {
        throw std::runtime_error("Error: Invalid facility line format.");
    }

    const std::string &name = args[1];
    FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
    int price = std::stoi(args[3]);
    int lifeQ = std::stoi(args[4]);
    int ecoImpact = std::stoi(args[5]);
    int envImpact = std::stoi(args[6]);

    if (isFacilityExists(name)) {
        throw std::runtime_error("Error: Facility already exists.");
    }

    FacilityType newFacility(name, category, price, lifeQ, ecoImpact, envImpact);
    facilitiesOptions.push_back(newFacility);
}

void Simulation::createPlan(const std::vector<std::string> &args) {
    if (args.size() != 3) {
        throw std::runtime_error("Error: Invalid plan line format.");
    }

    const std::string &settlementName = args[1];
    const std::string &policy = args[2];

    if (!isSettlementExists(settlementName)) {
        throw std::runtime_error("Error: Settlement does not exist for plan.");
    }
    SelectionPolicy *selectionPolicy = nullptr;
    if (policy == "nve") {
        selectionPolicy = new NaiveSelectionPolicy(); // מדיניות בחירה נאיבית
    } else if (policy == "bal") {
        selectionPolicy = new BalancedSelectionPolicy(); // מדיניות מאוזנת
    } else if (policy == "eco") {
        selectionPolicy = new EconomySelectionPolicy(); // מדיניות כלכלית
    } else if (policy == "env") {
        selectionPolicy = new SustainabilitySelectionPolicy(); // מדיניות סביבתית
    }

    Settlement &settlement = getSettlement(settlementName);
    plans.push_back(Plan(settlement, selectionPolicy));
}


void Simulation::start() {
    cout << "The simulation has started." << endl;
    isRunning = true;

    while (isRunning) {
        // תצוגת קלט למשתמש
        cout << "> ";
        string input;
        getline(cin, input); // קרא שורת פקודה מהמשתמש

        // שימוש ב-Auxiliary::parseArguments לפירוק הקלט
        vector<string> args = Auxiliary::parseArguments(input);

        // טיפול בקלט ריק
        if (args.empty()) {
            continue;
        }

        // זיהוי הפקודה הראשית
        const string& command = args[0];
        try {
            if (command == "step") {
                if (args.size() != 2) throw runtime_error("Error: Invalid syntax for 'step'.");
                int steps = stoi(args[1]);
                simulateStep(steps);
            } else if (command == "plan") {
                createPlan(args);
            } else if (command == "settlement") {
                createSettlement(args);
            } else if (command == "facility") {
                createFacility(args);
            } else if (command == "planStatus") {
                if (args.size() != 2) throw runtime_error("Error: Invalid syntax for 'planStatus'.");
                printPlanStatus(stoi(args[1]));
            } else if (command == "changePolicy") {
                if (args.size() != 3) throw runtime_error("Error: Invalid syntax for 'changePolicy'.");
                changePlanPolicy(stoi(args[1]), args[2]);
            } else if (command == "log") {
                printActionsLog();
            } else if (command == "close") {
                closeSimulation();
                isRunning = false; // סיום הלולאה
            } else {
                throw runtime_error("Error: Unknown command.");
            }
        } catch (const exception& e) {
            cerr << e.what() << endl; // הצגת שגיאה למשתמש
        }
    }
}



/*
bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        cout << "Settlement already exists." << endl;
        return false;
    }
    settlements.push_back(settlement);
    cout << "Settlement " << settlement->getName() << " added successfully." << endl;
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
    for (const Settlement *settlement : settlements) {
        if (settlement->getName() == settlementName) {
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
    */
}
