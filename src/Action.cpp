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
}