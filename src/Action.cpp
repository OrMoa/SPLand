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
        for (Plan& plan : simulation.getPlans()) {
            // שלב 1: בדיקת סטטוס התוכנית
            if (plan.getStatus() == PlanStatus::AVALIABLE) {
                // שלב 2: הוספת מתקנים חדשים לבנייה
                int constructionLimit = plan.getSettlement().getConstructionLimit();
                while (plan.getUnderConstruction().size() < constructionLimit) {
                    const FacilityType& nextFacilityType = plan.getSelectionPolicy()->selectFacility(plan.getFacilityOptions());
                    Facility* newFacility = new Facility(nextFacilityType, plan.getSettlement().getName());
                    plan.addFacility(newFacility);
                
            }
        
        }