
#ifndef GUIASSIGNMENT_EXAM_H
#define GUIASSIGNMENT_EXAM_H

class Exam : public Assessment {
private:
    int duration;
public:
    Exam(const std::string& type, double score, double maxScore, double weight, int duration): Assessment(type, score, maxScore, weight), duration(duration) {}
    double computeWeightedScore() const override {
        return (score/maxScore) * weight;
    }
    int getDuration() const {return duration;}
};
#endif