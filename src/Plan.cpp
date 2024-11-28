#include "Plan.h"

Plan::Plan(int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const std::vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE),
      life_quality_score(0), economy_score(0), environment_score(0) {}

//Destructor
Plan::~Plan() {
    delete selectionPolicy;
    for (auto facility : facilities) {
        delete facility;
    }
    for (auto facility : underConstruction) {
        delete facility;
    }
}
const vector<Facility*>& Plan::getFacilities() const {
    return facilities;
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

//לבדוק 
void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    if (selectionPolicy) {
        delete selectionPolicy;
    }
    selectionPolicy = newPolicy->clone();
}

void Plan::addFacility(Facility* facility){
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        underConstruction.push_back(facility);
    } else if (facility->getStatus() == FacilityStatus::OPERATIONAL) {
        facilities.push_back(facility);
        life_quality_score += facility->getLifeQualityScore();
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore();
    } 
}


const string Plan::toString() const {
    //לממש
    }

const PlanStatus Plan::getStatus() const{
    return status;
}

void Plan::printStatus() {
    std::cout << "PlanID: " << plan_id << "\n"
              << "SettlementName: " << settlement.getName() << "\n"
              << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") << "\n"
              << "LifeQualityScore: " << life_quality_score << "\n"
              << "EconomyScore: " << economy_score << "\n"
              << "EnvironmentScore: " << environment_score << "\n"
              << "Facilities Under Construction: " << underConstruction.size() << "\n"
              << "Facilities Completed: " << facilities.size() << "\n";
}
