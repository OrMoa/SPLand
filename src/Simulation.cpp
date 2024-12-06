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

Simulation::Simulation(const string &configFilePath) : isRunning(false),
 planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    std::ifstream file(configFilePath);
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
            continue;
        }
    }

    file.close();
    
}

Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),
      plans(other.plans),
      settlements(),
      facilitiesOptions(other.facilitiesOptions)
      
{
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());  //using BaseAction clone() method
    }

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement)); //using Settlement has a copy constructor
    }
}

Simulation::~Simulation(){
    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    for (BaseAction* action : actionsLog) {
        delete action; // שחרור אובייקט הפעולה
    }
    actionsLog.clear(); // ניקוי וקטור הפעולות

}

Simulation& Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        // Clear and copy actionsLog
       /*for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone()); // using BaseAction clone() method
        }*/

        // Clear and copy settlements
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }

        // Copy primitive fields
        isRunning = other.isRunning;

        planCounter = other.planCounter;

        // Copy facilitiesOptions
        facilitiesOptions = other.facilitiesOptions;
        plans.clear();

        /*for (const Plan& plan : other.plans) {
            plans.push_back(Plan(plan));
        }*/
       for (const Plan& plan : other.plans) {
            plans.push_back(Plan(plan, plan.getSettlement()));
    }
    }
    return *this;
}


void Simulation::clearToRestore() {
    for (BaseAction* action : actionsLog) {
        delete action; // שחרור זיכרון עבור כל פעולה
    }
    actionsLog.clear(); // ניקוי הרשימה
}

void Simulation::open(){
    cout << "The simulation has started." << endl;
    isRunning = true;
}

void Simulation::createSettlement(const vector<string> &args) {

    const string &name = args[1];
    //stoi is a C++ function that converts a string (std::string) to an integer (int).
    int type = std::stoi(args[2]);

    if (isFacilityExists(name)) {
        return;
    }        
    //static_cast is a safe and specific way in C++ to perform type conversion between compatible data types at compile time.
    SettlementType settlementType = static_cast<SettlementType>(type);
    Settlement *newSettlement = new Settlement(name, settlementType);
    settlements.push_back(newSettlement);
}

void Simulation::createFacility(const std::vector<std::string> &args) {
    const string &name = args[1];

    //stoi is a C++ function that converts a string (std::string) to an integer (int).
    //static_cast is a safe and specific way in C++ to perform type conversion between compatible data types at compile time.
    FacilityCategory category = static_cast<FacilityCategory>(stoi(args[2]));
    int price = stoi(args[3]);
    int lifeQ = stoi(args[4]);
    int ecoImpact = stoi(args[5]);
    int envImpact = stoi(args[6]);

    FacilityType newFacility(name, category, price, lifeQ, ecoImpact, envImpact);
    facilitiesOptions.push_back(newFacility);
}

void Simulation::createPlan(const vector<string> &args) {

    const string &settlementName = args[1];
    const string &policy = args[2];

    if (!isSettlementExists(settlementName)) {
        throw std::runtime_error("Error: Settlement does not exist for plan.");
    }
    SelectionPolicy *selectionPolicy = nullptr;
    if (policy == "nve") {
        selectionPolicy =  new NaiveSelection(); 
    } else if (policy == "bal") {
        selectionPolicy = new BalancedSelection(0, 0, 0); 
    } else if (policy == "eco") {
        selectionPolicy = new EconomySelection(); 
    } else if (policy == "env") {
        selectionPolicy = new SustainabilitySelection();
    }

    Settlement &settlement = getSettlement(settlementName);
    Plan newPlan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
}

void Simulation::start() {
    open();
    while (isRunning) {

        cout << "> ";
        string input;
        getline(cin, input);

        vector<string> args = Auxiliary::parseArguments(input);

        if (args.empty()) {
            continue;
        }

        processCommand(args);
    }
}

void Simulation::processCommand(const vector<string>& args){   

    const string& command = args[0];
    BaseAction* action = nullptr; 

    //step:
    if (command == "step") {

        int numOfSteps;
        numOfSteps = stoi(args[1]); // המרת המחרוזת למספר
        action = new SimulateStep(numOfSteps);

    //plan:
    } else if (command == "plan") {

        const string& settlementName = args[1];
        const string& selectionPolicy = args[2];
        action = new AddPlan(settlementName, selectionPolicy);

    //settlement:
    } else if (command == "settlement") {

        const string& settlementName = args[1];
        int settlementType;
        settlementType = stoi(args[2]);
        action = new AddSettlement(settlementName, static_cast<SettlementType>(settlementType));

    //facility: 
    } else if (command == "facility") {
        const string& facilityName = args[1];
        int category, price, lifeQualityScore, economyScore, environmentScore;
        category = stoi(args[2]);
        price = stoi(args[3]);
        lifeQualityScore = stoi(args[4]);
        economyScore = stoi(args[5]);
        environmentScore = stoi(args[6]);
        action = new AddFacility(facilityName,
                                 static_cast<FacilityCategory>(category),
                                 price,
                                 lifeQualityScore,
                                 economyScore,
                                 environmentScore);


    }else if (command == "backup") {
        action = new BackupSimulation();
    } else if (command == "restore") {
        action = new RestoreSimulation();
    } else if (command == "log"){
        action = new PrintActionsLog();
    }else if (command == "planStatus") {
        int planId;
        planId = stoi(args[1]);
        action = new PrintPlanStatus(planId);
    } else if (command == "changePolicy") {
        int planId;
        planId = stoi(args[1]);
        const string& selectionPolicy = args[2];
        action = new ChangePlanPolicy(planId, selectionPolicy);
    }else if (command == "close"){
        action = new Close();
    }
    /*if (action != nullptr && command != "log") {
        addAction(action);
        action->act(*this);
    } else if (action != nullptr && command == "log") {
        action->act(*this);
        addAction(action);
    }*/
   if(action != nullptr){
    if (command == "log"){
        action->act(*this);
        addAction(action);
        
    }
    else{
        addAction(action); 
        action->act(*this);   
    }
   }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions);
}

bool Simulation::addSettlement(Settlement* settlement) {
    if (isSettlementExists(settlement->getName())) {
        return false; 
    }

    settlements.push_back(settlement);
    return true; 
}

bool Simulation::addFacility(FacilityType facility) {
    if (isFacilityExists(facility.getName())) {
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true; 
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement *settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    for (const FacilityType &facility : facilitiesOptions) {
        if (facility.getName() == facilityName) {
            return true; 
        }
    }
    return false; 
}  

Settlement& Simulation::getSettlement(const string& name) const {
    for (Settlement* settlement : settlements) {
        if (settlement->getName() == name) {
            return *settlement; 
        }
    }
}

int Simulation::getPlanCounter() const{
    return planCounter;
}

void Simulation::close() {
    for (Plan& plan : plans) { 
        plan.closePrint();
    }
    isRunning = false;
}

void Simulation::printActionsLog() const {
    for (const BaseAction* action : actionsLog) {
        cout << action->toString() << endl; 
    }
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

Settlement& Simulation::getSettlement(const string& settlementName) {
    Settlement* result = nullptr; 
    for (Settlement* settlement : settlements) {
        if (settlement->getName() == settlementName) {
            result = settlement;  
            break;  
        }
    }
    return *result; 
}

void Simulation::step() {
    for (Plan& plan : plans) {
        plan.step();  
    }
}

Plan& Simulation::getPlan(const int planID) {
    return plans[planID];
}

void Simulation::restoreFromBackup() {
    if (backup != nullptr) {
        *this = *backup;   // שיבוץ מחדש מהגיבוי
    }
}
