#include "Plan.h"
#include <sstream>
using std::vector;


Plan::Plan(int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, 
           const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE), facilities(), underConstruction(), facilityOptions(facilityOptions),
      life_quality_score(0), economy_score(0), environment_score(0) {}
    
    
Plan::Plan(const Plan& other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(nullptr),
      status(other.status),
      facilities(), // אתחול רשימת המתקנים
      underConstruction(), // אתחול מתקנים בבנייה
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score){
    
    // העתקה עמוקה של SelectionPolicy
    if (other.selectionPolicy != nullptr) {
        selectionPolicy = other.selectionPolicy->clone(); // assuming clone is implemented
    }

    // העתקה עמוקה של facilities
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility)); // יצירת עותק של כל מתקן
    }

    // העתקה עמוקה של underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility)); // יצירת עותק של כל מתקן
    }
}

Plan::Plan(const Plan& other, const Settlement& newSettlement)
    : plan_id(other.plan_id),
      settlement(newSettlement), // שימוש במופע Settlement החדש
      selectionPolicy(nullptr),
      status(other.status),
      facilities(), // אתחול רשימת המתקנים
      underConstruction(), // השארת underConstruction ריק
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {

    // העתקה עמוקה של SelectionPolicy
    if (other.selectionPolicy != nullptr) {
        selectionPolicy = other.selectionPolicy->clone();
    }

    // העתקה עמוקה של facilities בלבד
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    // underConstruction יישאר ריק בבנאי הזה
}

//Destructor
Plan::~Plan() {
    
    // מחיקת מתקנים מ-facilities
    for (Facility* facility : facilities) {
        if (facility != nullptr) {
            delete facility;
        }
    }
    facilities.clear(); // ניקוי הווקטור

    // מחיקת מתקנים מ-underConstruction
    for (Facility* facility : underConstruction) {
        if (facility != nullptr) {
            delete facility;
        }
    }
    underConstruction.clear();
    
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
        selectionPolicy = nullptr; // איפוס למניעת גישה כפולה
    }
}

Plan& Plan::operator=(const Plan& other) {

    if (this == &other) {
        return *this; 
    }

    delete selectionPolicy;

    for (auto* facility : facilities) {
        delete facility;
    }
    facilities.clear();

    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    // העתקה עמוקה של מדיניות הבחירה
    if (other.selectionPolicy != nullptr) {
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

    
    plan_id = other.plan_id;
    status = other.status;

    return *this;
}

/*Plan& Plan::operator=(const Plan& other) {
    if (this != &other) {
        if (selectionPolicy != nullptr) {
            delete selectionPolicy;
        }
        selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
        // העתקת שאר האובייקטים
    }
    return *this;
}*/

const vector<Facility*>& Plan::getFacilities() const {
    return facilities;
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const Settlement& Plan::getSettlement() const {
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
    size_t constructionLimit = settlement.getConstructionLimit();
    if(status == PlanStatus::AVALIABLE){
        while (underConstruction.size() < constructionLimit) {
            addNewToConstruction();
        }
    }
   
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
    Facility* facility = *it;

    if (facility != nullptr) {
        facility->step();  // קריאה לפונקציה step() עבור כל Facility

        // בדיקה אם המתקן הפך ל-OPERATIONAL
        if (facility->getStatus() == FacilityStatus::OPERATIONAL) {
            addFacility(facility); 
            it = underConstruction.erase(it);  // מחק מ-underConstruction ועבור לאיבר הבא
            continue;  // דלג על ההגדלה של it במקרה של מחיקה
        }
    }
    ++it;  // עבור לאיבר הבא
    }
    if (underConstruction.size() < constructionLimit) {
        status = PlanStatus::AVALIABLE;
    }
    else{
        status = PlanStatus::BUSY;
    }
}

void Plan::addNewToConstruction(){
    const FacilityType* nextFacilityType = &selectionPolicy->selectFacility(facilityOptions);       
        if (nextFacilityType != nullptr) {
            Facility* newFacility = new Facility(*nextFacilityType, settlement.getName());
            addFacility(newFacility); 
        }
    }

void Plan::setSelectionPolicy(SelectionPolicy* newPolicy) {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    selectionPolicy = newPolicy;
}