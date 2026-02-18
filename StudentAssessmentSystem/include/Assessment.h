
#ifndef GUIASSIGNMENT_ASSESSMENT_H
#define GUIASSIGNMENT_ASSESSMENT_H

#include <string>

class Assessment {
protected:
    std::string type;
    double score;
    double maxScore;
    double weight;

public:
    Assessment(const std::string& type, double score, double maxScore, double weight) : type(type), score(score), maxScore(maxScore), weight(weight) {}

    //virtual functions
    virtual ~Assessment() = default; //cleans up the hidden vptr and derived data
    virtual double computeWeightedScore() const = 0;

    //additional assessment functions
    double getScore() const {return score;}
    double getMaxScore() const {return maxScore;}
    double getWeight() const {return weight;}
    std::string getType() const {return type;}
};
#endif