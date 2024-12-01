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
        
        processCommand(args);
    }
}

void Simulation::processCommand(const vector<string>& args){    // כאן כתוב גוף הפונקציה

    const string& command = args[0];
    BaseAction* action = nullptr; // מצביע כללי לפעולה

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
    } 
    else if (command == "restore") {
        action = new RestoreSimulation();
    
    } else if (command == "planStatus") {
        int planId;
        planId = stoi(args[1]); // המרת המחרוזת למספר
        action = new PrintPlanStatus(planId);

    } else if (command == "changePolicy") {

        int planId;
        planId = stoi(args[1]); // המרת המחרוזת למספר
        const string& selectionPolicy = args[2];
        action = new ChangePlanPolicy(planId, selectionPolicy);

    
    }if (action != nullptr) {
        action->act(*this);
        actionsLog.push_back(action); // שמירת הפעולה בלוג
    }
   
}

<<<<<<< HEAD
int Simulation::getPlanCounter() const{
    return planCounter;
}


void Simulation::simulateStep(int numOfSteps) {
    for (int step = 0; step < numOfSteps; ++step) {
        // מעבר על כל ה plans בסימולציה
        for (Plan& plan : plans) {
            // שלב 1: בדיקת סטטוס התוכנית
            if (plan.getStatus() == PlanStatus::AVALIABLE) {
                // הוספת מתקנים לבנייה עד למגבלת היישוב
                int constructionLimit = plan.getSettlement().getConstructionLimit();
                while (plan.getUnderConstruction().size() < constructionLimit) {
                    // בחירת המתקן הבא לבנייה
                    const FacilityType& nextFacilityType = plan.getSelectionPolicy()->selectFacility(plan.getFacilityOptions());
                    Facility* newFacility = new Facility(nextFacilityType, plan.getSettlement().getName());

                    // הוספת המתקן לבנייה
                    plan.addFacility(newFacility);
                }
            }

            // שלב 2: עדכון מתקנים בבנייה
            plan.updateUnderConstruction();

            // שלב 3: עדכון סטטוס התוכנית
            int constructionLimit = plan.getSettlement().getConstructionLimit();
            if (plan.getUnderConstruction().size() >= constructionLimit) {
                plan.setStatus(PlanStatus::BUSY);
            } else {
                plan.setStatus(PlanStatus::AVAILABLE);
            }
        }
    }

    cout << "Simulation steps completed." << endl;
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
=======
>>>>>>> 81b77671aba7ef55e2b1c37f62ad60baea965329
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


