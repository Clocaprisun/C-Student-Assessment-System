#include "../include/Course.h"

Course::Course(const std::string &code, const std::string &title, int credits) : courseCode(code), courseTitle(title), creditHours(credits) {}

/* add an assessment to the assessment vector to the end */
void Course::addAssessment(std::shared_ptr<Assessment> assessment) {
    assessments.push_back(std::move(assessment));
}

/* gets the average of a single selected course
 * returns average */
double Course::computeAverage() const {
    if (assessments.empty()) return 0.0;

    double total = 0.0;
    double totalWeight = 0.0; // Optional: track total weight if you want to normalize partial grades

    for (const auto& a : assessments) {
        // computeWeightedScore returns decimal (e.g., 0.20 for 20%)
        total += a->computeWeightedScore();
        totalWeight += a->getWeight();
    }

    return total;
}

/* Operator<< to print the selected course
 * returns os */
std::ostream& operator<<(std::ostream& os, const Course& c) {
    os << c.courseCode << " - " << c.courseTitle
       << " (" << c.creditHours << " credits)";
    return os;
}