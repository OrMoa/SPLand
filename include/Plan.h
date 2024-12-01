#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
using std::vector;
using std::string;


enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        ~Plan();
        Plan(const Plan&) = delete;
        Plan& operator=(const Plan&) = delete;
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
        void updateUnderConstruction();
        const Settlement getSettlement();
        const vector<Facility*>& getUnderConstruction() const;
        const SelectionPolicy* getSelectionPolicy() const;
<<<<<<< HEAD
        
=======
        const int getPlanId() const;

>>>>>>> f4b9d9f7fb61509794aae48d78d5840b0fd2e5bd

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