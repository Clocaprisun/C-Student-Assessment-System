
#ifndef GUIASSIGNMENT_QUIZ_H
#define GUIASSIGNMENT_QUIZ_H
#include "Assessment.h"

class Quiz : public Assessment {
private:
    int duration;

public:
    Quiz(const std::string& type, double score, double maxScore, double weight, int duration): Assessment(type, score, maxScore, weight), duration(duration) {}
    double computeWeightedScore() const override {
        return (score / maxScore) * weight;
    }
    int getDuration() const {return duration;}
};
#endif