#include "Plan.h"
#include <sstream>

Plan::Plan(int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const std::vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE),
      life_quality_score(0), economy_score(0), environment_score(0) {}

Plan::Plan(const Plan& other)
    : plan_id(other.plan_id), 
      settlement(other.settlement), 
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr), 
      facilityOptions(other.facilityOptions), 
      status(other.status), 
      life_quality_score(other.life_quality_score), 
      economy_score(other.economy_score), 
      environment_score(other.environment_score) 
{
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

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

Plan& Plan::operator=(const Plan& other) {
    if (this == &other) {
        return *this; // הגנה מפני השמה עצמית
    }

    // ניקוי משאבים קיימים
    delete selectionPolicy;
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();

    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    // העתקה עמוקה של מדיניות הבחירה
    if (other.selectionPolicy) {
        selectionPolicy = other.selectionPolicy->clone();
    } else {
        selectionPolicy = nullptr;
    }

    // העתקת רשימת מתקנים (עמוקה)
    for (const Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    // העתקת מתקנים בבנייה (עמוקה)
    for (const Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }

    // העתקת שדות פשוטים
    plan_id = other.plan_id;
    status = other.status;

    // הערה: settlement נשאר ללא שינוי כי הוא const reference

    return *this;
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

const Settlement Plan::getSettlement(){
    return settlement;
}

const vector<Facility*>& Plan::getUnderConstruction() const {
    return underConstruction;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

const SelectionPolicy* Plan::getSelectionPolicy() const {
    return selectionPolicy;
}

const int Plan::getPlanId() const{
    return plan_id;
}


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
    std::ostringstream output;

    output << "PlanID: " << plan_id << "\n";
    output << "SettlementName: " << settlement.getName() << "\n";
    output << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") << "\n";
    output << "SelectionPolicy: " << (selectionPolicy ? selectionPolicy->toString() : "None") << "\n";
    output << "LifeQualityScore: " << life_quality_score << "\n";
    output << "EconomyScore: " << economy_score << "\n";
    output << "EnvironmentScore: " << environment_score << "\n";

    // מתקנים בבנייה
    for (const Facility* facility : underConstruction) {
        output << "FacilityName: " << facility->getName() << "\n"
               << "FacilityStatus: UNDER_CONSTRUCTIONS\n";
    }

    // מתקנים שהושלמו
    for (const Facility* facility : facilities) {
        output << "FacilityName: " << facility->getName() << "\n"
               << "FacilityStatus: OPERATIONAL\n";
    }

    return output.str(); // החזרת המחרוזת שנוצרה
}

const void Plan::closePrint() const {
    std::cout << "PlanID: " << plan_id << "\n"
              << "SettlementName: " << settlement.getName() << "\n"
              << "LifeQualityScore: " << life_quality_score << "\n"
              << "EconomyScore: " << economy_score << "\n"
              << "EnvironmentScore: " << environment_score << "\n";

}

const PlanStatus Plan::getStatus() const{
    return status;
}

void Plan::printStatus() {
    std::cout << toString();
}

void Plan::step() {
    if (status == PlanStatus::BUSY) {
        updateUnderConstruction();
        return;
    }

    int constructionLimit = settlement.getConstructionLimit();
    while (underConstruction.size() < constructionLimit) {
        const FacilityType* nextFacilityType = &selectionPolicy->selectFacility(facilityOptions);       
        if (nextFacilityType == nullptr) {
            break;
        }
        Facility* newFacility = new Facility(*nextFacilityType, settlement.getName());
        addFacility(newFacility); 
    }
    updateUnderConstruction();
    if (underConstruction.size() < settlement.getConstructionLimit()) {
        status = PlanStatus::AVALIABLE;
    } else {
        status = PlanStatus::BUSY;
    }
}

void Plan::updateUnderConstruction() {
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        Facility* facility = *it;
        FacilityStatus status = facility->step(); 

        if (status == FacilityStatus::OPERATIONAL) {
            facilities.push_back(facility); 
            it = underConstruction.erase(it); 
        } else {
            ++it; 
        }
    }
}

void Plan::setSelectionPolicy(SelectionPolicy* newPolicy) {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    selectionPolicy = newPolicy;
}