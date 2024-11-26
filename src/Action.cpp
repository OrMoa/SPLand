#include "Simulation.h"
#include "Action.h"
#include "Facility.h"
#include "Settlement.h"
#include "Plan.h"
#include <iostream>
#include <stdexcept>
#include "Auxiliary.h"



void Action::complete() {
    status = ActionStatus::COMPLETED; // שימוש בשם המלא של ה-enum
}

    // עדכון סטטוס ל-ERROR עם הודעת שגיאה
void Action::error(const std::string& errorMessage)
{
        status = ERROR;
        errorMsg = errorMessage;
        std::cerr << "Error: " << errorMsg << std::endl; // הדפסת הודעת שגיאה למסך
    }