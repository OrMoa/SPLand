#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
using std::vector;
using std::string;
#include <sstream>


enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        ~Plan()=default;
        Plan(const Plan&);
        Plan& operator=(const Plan&);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const PlanStatus getStatus() const;
        const Settlement getSettlement();
        const vector<Facility*>& getUnderConstruction() const;
        const SelectionPolicy* getSelectionPolicy() const;
        const int getPlanId() const;
        const void closePrint() const;
        void addNewToConstruction();


    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};