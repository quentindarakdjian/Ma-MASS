
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <ctime>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include "Utility.h"
#include "SimulationConfig.h"
#include "Model_Presence.h"

Model_Presence::Model_Presence() {
}

void Model_Presence::calculatePresenceFromActivities(const std::vector<double> activities) {
    for(double activity: activities) {
        if (activity != 9) {
            presenceState.push_back(1);
        } else {
            presenceState.push_back(0);
        }
    }
}

int Model_Presence::calculateNumberOfDays(const int startDay, const int startMonth, const int endDay, const int endMonth){
    static const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int days = daysInMonth[startMonth - 1] - (startDay -1);
    for(int i = startMonth; i < endMonth -1; i++){
        days += daysInMonth[i];
    }
    days += endDay;
    return days;
}

void Model_Presence::calculatePresenceFromPage(const int agentID) {
    float pMon[24];
    float pTue[24];
    float pWed[24];
    float pThu[24];
    float pFri[24];
    float pSat[24];
    float pSun[24];
    bool adjustmentPageChoice = 1; // == 0  if base model is used, 1 if adjusted model is used
    double adjustmentCoeff = 1.7; // OECD et DARES

    std::map<int, std::string> probMap = SimulationConfig::agents.at(agentID).profile;
    for(int day = 0; day < 7; day++) {
        std::deque<std::string> tokProbs;
        boost::split(tokProbs, probMap.at(day), boost::is_any_of(","));

        std::string jobType = SimulationConfig::agents.at(agentID).jobType;
        tokProbs = contextualJob(jobType, tokProbs);

        int hour = 0;
        for(std::string strProb: tokProbs) {
            switch (day){
              case 0:
                pMon[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, adjustmentCoeff);
                break;
              case 1:
                pTue[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, adjustmentCoeff);
                break;
              case 2:
                pWed[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, adjustmentCoeff);
                break;
              case 3:
                pThu[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, adjustmentCoeff);
                break;
              case 4:
                pFri[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, adjustmentCoeff);
                break;
              case 5:
                pSat[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, 0.5);
                break;
              case 6:
                pSun[hour] = adjustmentPage(adjustmentPageChoice, hour, strProb, 0.5);
                break;
            }
            hour++;
        }
    }

    unsigned int timeStepsPerHour = SimulationConfig::info.timeStepsPerHour;
    unsigned int days = calculateNumberOfDays(SimulationConfig::info.startDay, SimulationConfig::info.startMonth, SimulationConfig::info.endDay, SimulationConfig::info.endMonth);
    // Model for the prediction of presence derived by J. Page
    // Reference: J. Page, D. Robinson, N. Morel, J.-L. Scartezzini, A generalised stochastic
    // model for the simulation of occupant presence, Energy and Buildings 40(2), 83-98 (2008).
    std::vector<int> occ;
    occ.push_back(false);
    double shuff = 0.11; // Mean observed value for mobility parameter
    double LongAbsCurrentDuration = 0.;

    for (unsigned int day = 1; day <= days+1; ++day) {
        unsigned int dayOfTheWeek = (day - 1) % 7;
        for (unsigned int hour = 1; hour <= 24; ++hour) {
            double pHour = 0.0;
            double pNextHour = 0.0;
            if (dayOfTheWeek == 0) {
                pHour = pMon[hour - 1];
                pNextHour = ((hour == 24) ? pTue[0] : pMon[hour]);
            } else if (dayOfTheWeek == 1) {
                pHour = pTue[hour - 1];
                pNextHour = ((hour == 24) ? pWed[0] : pTue[hour]);
            } else if (dayOfTheWeek == 2) {
                pHour = pWed[hour - 1];
                pNextHour = ((hour == 24) ? pThu[0] : pWed[hour]);
            } else if (dayOfTheWeek == 3) {
                pHour = pThu[hour - 1];
                pNextHour = ((hour == 24) ? pFri[0] : pThu[hour]);
            } else if (dayOfTheWeek == 4) {
                pHour = pFri[hour - 1];
                pNextHour = ((hour == 24) ? pSat[0] : pFri[hour]);
            } else if (dayOfTheWeek == 5) {
                pHour = pSat[hour - 1];
                pNextHour = ((hour == 24) ? pSun[0] : pSat[hour]);
            } else {
                pHour = pSun[hour - 1];
                pNextHour = ((hour == 24) ? pMon[0] : pSun[hour]);
            }
            for (unsigned int fracHour = 0; fracHour < timeStepsPerHour; ++fracHour) {
                // intrapolation
                double pcurr = ((static_cast<double>(timeStepsPerHour - fracHour)) * pHour + (static_cast<double>(fracHour)) * pNextHour) / timeStepsPerHour;
                double pnext = ((static_cast<double>(timeStepsPerHour - (fracHour + 1))) * pHour + (static_cast<double>(fracHour + 1)) * pNextHour) / timeStepsPerHour;
                double ProbLongAbsence = 0.000; // To be adjusted later, lack of calibration data.
                // --- 1. If a long absence is ongoing -----------------------
                if (LongAbsCurrentDuration > 0.) {
                    LongAbsCurrentDuration = std::max(LongAbsCurrentDuration - 1., 0.);
                    occ.push_back(false);
                }
                // --- 2. If there is no long absence ------------------------
                else {
                    // --- 2a. If a long absence starts ------------
                    if (Utility::randomDouble(0.0, 1.0) < ProbLongAbsence) {
                        occ.push_back(false);
                        LongAbsCurrentDuration = 1000.;
                    }// 1000 constante arbitraire, longueur de longue absence dans XML
                    // --- 2b. If a long absence does not start ----
                    else {
                        // Room currently not occupied
                        if (!occ.back()) {
                            if (Utility::randomDouble(0.0, 1.0) < getT01(pcurr, pnext, shuff)) {
                                occ.push_back(true);
                            } else {
                                occ.push_back(false);
                            }
                        }                            // Room currently occupied
                        else {
                            if (Utility::randomDouble(0.0, 1.0) < (1. - getT11(pcurr, pnext, shuff))) {
                                occ.push_back(false);
                            } else {
                                occ.push_back(true);
                            }
                        }
                    }
                }
            }
        }
    }

    // remove the first element of the vector (which was just for starting the process)
    occ.erase(occ.begin());
    presenceState = occ;
}

bool Model_Presence::at(const int i) const{
    return presenceState.at(i);
}

unsigned int Model_Presence::size() const{
    return presenceState.size();
}

double Model_Presence::getT01(const double pcurr, const double pnext, const double shuff) {
    // This function returns the transition probabilities T01
    // pcurr: current step occupancy probability
    // pnext: next step occupancy probability
    // shuff: shuffling parameter
    // beta:  adjusted value of shuff
    double T01; // Probability to leave the space
    double beta = shuff; // default: no adjustment needed
    if (pnext == 0.) {
        T01 = 0.;
    }
    else if (pnext == 1.) {
        T01 = 1.;
    }
    else {
        if (pcurr == 1.) {
            T01 = 0.;
        } else if (pcurr == 0.) {
            T01 = pnext;
        } else if (pcurr == pnext) {
            if (pcurr + pnext > 1.) {
                if (shuff > 1. / (2.*pcurr - 1.)) {
                    beta = 1. / (2.*pcurr - 1.);
                }
                else {
                    beta = shuff;
                }
            }
            else if (pcurr + pnext < 1.) {
                if (shuff > 1. / (1. - 2.*pcurr)) {
                    beta = 1. / (1. - 2.*pcurr);
                } else {
                    beta = shuff;
                }
            }
            else {
                beta = shuff;
            }
            T01 = 2.*beta * pcurr / (beta + 1.);
        }
        else if (pcurr < pnext) {
            if (shuff < (pnext - pcurr) / (2. - (pnext + pcurr))) {
                beta = (pnext - pcurr) / (2. - (pnext + pcurr));
            }
            else {
                if ((pcurr + pnext > 1.) && (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                }
                else if ((pcurr + pnext < 1.) && (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
                    beta = (1. - pcurr + pnext) / (1. - pcurr - pnext);
                }
                else {
                    beta = shuff;
                }
            }
            T01 = pnext + pcurr * (beta - 1.) / (beta + 1.);
        }
        else { // Case of (pcurr>pnext)
            if (shuff < (pcurr - pnext) / (pnext + pcurr)) {
                beta = (pcurr - pnext) / (pnext + pcurr);
            }
            else {
                if ((pcurr + pnext > 1.) && (shuff > (pcurr - pnext + 1.) / (pnext + pcurr - 1.))) {
                    beta = (pcurr - pnext + 1.) / (pnext + pcurr - 1.);
                }
                else if ((pcurr + pnext < 1.) && (shuff > (1. - pcurr + pnext) / (1. - pcurr - pnext))) {
                    beta = (1. - pcurr + pnext) / (1. - pcurr - pnext);
                }
                else {
                    beta = shuff;
                }
            }
            T01 = pnext + pcurr * (beta - 1.) / (beta + 1.);
        }
    }
    return T01;
}

double Model_Presence::getT11(const double pcurr, const double pnext, const double shuff) {
    // This function returns the transition probabilities T01 and T11
    // pcurr: current step occupancy probability
    // pnext: next step occupancy probability
    // shuff: shuffling parameter
    // beta:  adjusted value of shuff
    double T11; // Probability to stay in the space
    if (pnext == 0.) {
        T11 = 0.;
    }
    else if (pnext == 1.) {
        T11 = 1.;
    }
    else {
        if (pcurr == 1.) {
            T11 = pnext;
        }
        else if (pcurr == 0.) {
            T11 = 0.;
        }
        else if (pcurr == pnext) {
            T11 = 1. - (1. - pcurr) * getT01(pcurr, pnext, shuff) / pcurr;
        }
        else { // Case of (pcurr>pnext)
            T11 = 1. / pcurr * (pnext - (1. - pcurr) * getT01(pcurr, pnext, shuff));
        }
    }
    return T11;
}

float Model_Presence::adjustmentPage(const bool adjustmentChoice, const int hour, const std::string strProb, const double adjustmentCoeff) {
    float adjustedProb = boost::lexical_cast<double>(strProb);
    if (adjustmentChoice){
        if (hour < 6 || hour > 21){
            adjustedProb = 0;
        }
        else {
            adjustedProb = adjustedProb * adjustmentCoeff;
        }
    }
    return adjustedProb;
}

std::deque<std::string> Model_Presence::contextualJob(std::string jobType, std::deque<std::string> tokProbs){
    double randJob = Utility::randomDouble();
    if (jobType == "Unkwown"){
        double rand = Utility::randomDouble();
        if (rand < 0.15){jobType = "AdministrativeWorker";}
        if (rand >= 0.15 && rand < 0.29){jobType = "Technician";}
        if (rand >= 0.29 && rand < 0.43){jobType = "ExecutiveWorker";}
        if (rand >= 0.43 && rand < 0.58){jobType = "Ingineer";}
        if (rand >= 0.58 && rand < 0.72){jobType = "LiberalWorker";}
        if (rand >= 0.72 && rand < 0.86){jobType = "IndependantWorker";}
        else {jobType = "CompanyHead";}
    }
    if (jobType == "AdministrativeWorker" || jobType == "Technician"){
        if (randJob>0.09){
            std::string lastProb = tokProbs.back(); // Recover the last value of the vector
            tokProbs.pop_back(); // Delete the last value of the vector
            tokProbs.push_front(lastProb); // Add the copied value in the front of the vector
        }
    }
    if (jobType == "ExecutiveWorker"){
        if (randJob<0.87){
            std::string lastProb = tokProbs.back();
            tokProbs.pop_back();
            tokProbs.push_front(lastProb);
            lastProb = tokProbs.back();
            tokProbs.pop_back();
            tokProbs.push_front(lastProb);
        }
        else{
            std::string Prob17 = tokProbs[17];
            tokProbs[18] = Prob17;
            tokProbs[19] = Prob17;
        }
    }
    if (jobType == "Ingineer"){
        std::string lastProb = tokProbs.back();
        tokProbs.pop_back();
        tokProbs.push_front(lastProb);
        lastProb = tokProbs.back();
        tokProbs.pop_back();
        tokProbs.push_front(lastProb);
    }
    if (jobType == "LiberalWorker"){
        if (randJob<0.70){
            std::string lastProb = tokProbs.back();
            tokProbs.pop_back();
            tokProbs.push_front(lastProb);
            lastProb = tokProbs.back();
            tokProbs.pop_back();
            tokProbs.push_front(lastProb);
        }
        if (randJob>=0.70&&randJob<0.80){
            std::string Prob8 = tokProbs[8];
            tokProbs[7] = Prob8;
            tokProbs[6] = Prob8;
        }
        else{
            std::string Prob17 = tokProbs[17];
            tokProbs[18] = Prob17;
            tokProbs[19] = Prob17;
        }
    }
    if (jobType == "IndependantWorker" || jobType == "CompanyHead"){ // Empirical - To be adjusted
        if (randJob<0.35){
            std::string Prob8 = tokProbs[8];
            tokProbs[7] = Prob8;
            tokProbs[6] = Prob8;
        }
        else{
            std::string Prob17 = tokProbs[17];
            tokProbs[18] = Prob17;
            tokProbs[19] = Prob17;
        }
    }
    return tokProbs;
}
