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

<<<<<<< HEAD
=======
//לממש
>>>>>>> f4b9d9f7fb61509794aae48d78d5840b0fd2e5bd
const string Plan::toString() const {
    }

const PlanStatus Plan::getStatus() const{
    return status;
}

// נדרש? קיים כפל קוד?
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

void Plan::step() {
    // שלב 1: בדיקת סטטוס
    if (status == PlanStatus::BUSY) {
        // אם עסוק, נבצע רק עדכון מתקנים בבנייה
        updateUnderConstruction();
        return;
    }

    // שלב 2: הוספת מתקנים ל-`underConstruction`
    int constructionLimit = settlement.getConstructionLimit();
    while (underConstruction.size() < constructionLimit) {
        const FacilityType* nextFacilityType = selectionPolicy->selectFacility(facilityOptions);

        // אם אין מתקנים זמינים, עצור
        if (nextFacilityType == nullptr) {
            break;
        }

        // יצירת מתקן חדש והוספתו ל-`underConstruction`
        Facility* newFacility = new Facility(*nextFacilityType, settlement.getName());
        addFacility(newFacility); // שימוש במתודת העזר
    }

    // שלב 3: עדכון מתקנים בבנייה
    updateUnderConstruction();
}

void Plan::updateUnderConstruction() {

    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        Facility* facility = *it;
        facility->decrementTimeLeft(); // הפחתת זמן הבנייה

        if (facility->getTimeLeft() == 0) {
            facility->setStatus(FacilityStatus::OPERATIONAL);
            facilities.push_back(facility); // העברת המצביע לוקטור השני
            it = underConstruction.erase(it); // מחיקת האיבר ועדכון האיטרטור***לבדוק
        } else {
            ++it; // התקדמות ידנית אם האיבר לא נמחק
        }
    }
    if (underConstruction.empty()) {
        status = PlanStatus::AVALIABLE; // אין מתקנים בבנייה
    } else if (underConstruction.size() >= settlement.getConstructionLimit()) {
        status = PlanStatus::BUSY; // הגעת למגבלת הבנייה
    }
}

void Plan::setSelectionPolicy(SelectionPolicy* newPolicy) {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    selectionPolicy = newPolicy;

    if (auto* naivePolicy = dynamic_cast<NaiveSelection*>(selectionPolicy)) {
        naivePolicy->reset();
    }
}