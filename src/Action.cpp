#include "Action.h"


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = std::move(errorMsg); // שימוש ב-std::move לשיפור יעילות
    std::cerr << "Error: " << this->errorMsg << std::endl;
}

const std::string& BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation& simulation) {
    // עבור כל צעד
    for (int stepLeft = 0; stepLeft < numOfSteps; ++stepLeft) {
        for (Plan& plan : simulation.getPlans()) {// להחליט האם מתאים בדרך זו
                plan.step();      
            }
        
        }
    }

//AddSettlement

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        error("Settlement already exists: " + settlementName); 
        return;
    }

    Settlement *newSettlement = new Settlement(settlementName, settlementType);
    simulation.addSettlement(newSettlement);

    complete(); // קריאה לפונקציית complete מ-BaseAction
}

const string AddSettlement::toString() const {
    return "AddSettlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + 
           " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

//addPlan

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)) {
        error("Settlement does not exist: " + settlementName);
        return;
    }

    // קיימת חזרתיות על הקטע קוד הזה, לתקן ולשנות
    SelectionPolicy *policy = nullptr;
    if (selectionPolicy == "nve") {
        policy = new NaiveSelectionPolicy();
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelectionPolicy();
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelectionPolicy();
    } else if (selectionPolicy == "env") {
        policy = new SustainabilitySelectionPolicy();
    } else {
        error("Invalid selection policy: " + selectionPolicy);
        return;
    }

    Settlement &settlement = simulation.getSettlement(settlementName);
    simulation.addPlan(settlement, policy);
    complete();

    
}
