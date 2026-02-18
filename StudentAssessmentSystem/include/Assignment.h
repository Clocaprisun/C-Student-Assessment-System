
#ifndef GUIASSIGNMENT_ASSIGNMENT_H
#define GUIASSIGNMENT_ASSIGNMENT_H

class Assignment : public Assessment {
private:
    std::string deadline;

public:
    Assignment(const std::string& type, double score, double maxScore, double weight, const std::string& deadline) : Assessment(type, score, maxScore, weight), deadline(deadline) {}

    double computeWeightedScore() const override {
        return (score/maxScore) * weight;
    }
    std::string getDeadline() const {return deadline;}
};
#endif