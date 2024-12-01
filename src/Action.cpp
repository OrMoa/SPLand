#include "Action.h"
#include "SelectionPolicy.h"


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

//simulationStep:
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation& simulation) {
    // עבור כל צעד
    for (int stepLeft = 0; stepLeft < numOfSteps; ++stepLeft) {
        for (Plan& plan : simulation.getPlans()) {// להחליט האם מתאים בדרך זו
                plan.step();      
            }
        
        }
    }

// חסר to string ו clone

//AddSettlement:
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

//PrintActionsLog

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    const vector<BaseAction*>& actionsLog = simulation.getActionsLog(); // גישה ללוג
    for (const BaseAction* action : actionsLog) {
        cout << action->toString() << endl; // הדפסה של כל פעולה
    }
    complete(); // סימון הפעולה כ-COMPLETED
}

const string PrintActionsLog::toString() const {
    return "log COMPLETED";
}

//addFacility

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : facilityName(facilityName), 
      facilityCategory(facilityCategory), 
      price(price), 
      lifeQualityScore(lifeQualityScore), 
      economyScore(economyScore), 
      environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    if (simulation.isFacilityExists(facilityName)) {
        error("Facility already exists: " + facilityName);
        return;
    }

    FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    simulation.addFacility(newFacility);

    complete();
}

const string AddFacility::toString() const {
    return "AddFacility " + facilityName + " " + 
           to_string(static_cast<int>(facilityCategory)) + " " +
           to_string(price) + " " + 
           to_string(lifeQualityScore) + " " + 
           to_string(economyScore) + " " + 
           to_string(environmentScore) + 
           " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

//PrintPlanStatus

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if (planId > simulation.getPlanCounter()-1)
   {
        error("Plan doesn't exists");
   }
   else{
        Plan &plan = simulation.getPlan(planId);
        cout << plan.toString() << endl;
        complete();
   }
}

Close::Close() {}

void Close::act(Simulation &simulation) {
    for (const Plan &plan : simulation.getPlans()) {
        cout << plan.toString() << endl; // שימוש ב-toString של Plan
    }

}

//BackupSimulation

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    backup = new Simulation(simulation);

    complete();
}

RestoreSimulation::RestoreSimulation(){};

void RestoreSimulation::act(Simulation &simulation){
   if (backup == nullptr){
        error("No backup available");
   }
   else{
        simulation = *backup;
        complete();
    }         
}

//changePlanPolicy:
ChangePlanPolicy::ChangePlanPolicy(int planId, const string& newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    //  בדיקה אם התוכנית קיימת או חוקית
    if (planId >= simulation.getPlanCounter()) {
        error("Error: Plan doesn't exist.");
        return;
    }

    Plan& plan = simulation.getPlan(planId);

    // יצירת מדיניות חדשה
    SelectionPolicy* policy = nullptr;
    if (newPolicy == "nve") {
        policy = new NaiveSelection();
    } else if (newPolicy == "bal") {
        policy = new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());
    } else if (newPolicy == "eco") {
        policy = new EconomySelection();
    } else if (newPolicy == "env") {
        policy = new SustainabilitySelection();
    } else {
        cout << "Error: Invalid selection policy: " << newPolicy << endl;
        return;
    }

    // החלפת המדיניות בתוכנית
    plan.setSelectionPolicy(policy);
    complete();
}
