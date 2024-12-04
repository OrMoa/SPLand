#include "Action.h"
#include "SelectionPolicy.h"

//baseAction:
BaseAction::BaseAction() 
    : errorMsg(""), status(ActionStatus::COMPLETED) {
}

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
    for (int stepLeft = 0; stepLeft < numOfSteps; ++stepLeft) {
        simulation.step();
    }
}

const string SimulateStep::toString() const {
    return "step " + to_string(numOfSteps) + " " + 
           (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this); // שימוש בבנאי ההעתקה ליצירת עותק
}

//AddSettlement:
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {

    Settlement *newSettlement = new Settlement(settlementName, settlementType);
    if (!simulation.addSettlement(newSettlement)) {
        delete newSettlement;
        error("Settlement already exists: " + settlementName);
        return;
    }
    else{
        complete(); 
    }
}

const string AddSettlement::toString() const {
    return "AddSettlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + 
           " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);
}
//addPlan

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)) {
        error("Cannot create this plan");
        return;
    }

    SelectionPolicy *policy = nullptr;
    if (selectionPolicy == "nve") {
        policy = new NaiveSelection(); 
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelection(0, 0, 0); 
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelection(); 
    } else if (selectionPolicy == "env") {
        policy = new SustainabilitySelection();
    }else{
        error("Cannot create this plan");
        return;
    }

    Settlement &settlement = simulation.getSettlement(settlementName);
    simulation.addPlan(settlement, policy);
    complete();

}

const string AddPlan::toString() const {
    return "plan " + settlementName + " " + selectionPolicy + 
           (getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR (" + getErrorMsg() + ")");
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(*this);
}

//PrintActionsLog

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog(); 
    complete();
}

const string PrintActionsLog::toString() const {
    return "log COMPLETED";
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this); 
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
    FacilityType *newFacility = new FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);

    if (!simulation.addFacility(*newFacility)) {
        delete newFacility;
        error("Facility already exists: " + facilityName);
        return;
    }
    else {
        complete();
    }
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

AddFacility* AddFacility::clone() const {
    return new AddFacility(*this);
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
        plan.printStatus();
        complete();
   }
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId) + " " +
           (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
}

const string Close::toString() const {
    return "Close action";  
}

Close* Close::clone() const {
    return new Close(*this);  
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

const string BackupSimulation::toString() const {
    return "backup " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this); 
}

RestoreSimulation::RestoreSimulation(){};

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr) {
        error("No backup AVALIABLE");
        return;
    }
    simulation.clearToRestore();
    simulation.restoreFromBackup();
    complete();
}

const string RestoreSimulation::toString() const {
    return "restore " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this); 
}

//changePlanPolicy:
ChangePlanPolicy::ChangePlanPolicy(int planId, const string& newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    //  בדיקה אם התוכנית קיימת או חוקית
    if (planId > simulation.getPlanCounter()) {
        error("Cannot change selection policy");
        return;
    }

    Plan& plan = simulation.getPlan(planId);

    if (plan.getSelectionPolicy()->toString() == newPolicy) {
        error("Cannot change selection policy");
        return;
    }
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
        return;
    }

    plan.setSelectionPolicy(policy);
    complete();
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy " + to_string(planId) + " " + newPolicy + 
           " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR (" + getErrorMsg() + ")");
}