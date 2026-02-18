
#ifndef GUIASSIGNMENT_COURSE_H
#define GUIASSIGNMENT_COURSE_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Assessment.h"

class Course {
private:
    std::string courseCode;
    std::string courseTitle;
    int creditHours;
    std::vector<std::shared_ptr<Assessment>> assessments;

public:
    Course(const std::string& code, const std::string& title, int credits);

    void addAssessment(std::shared_ptr<Assessment> assessment);
    double computeAverage() const;

    //friend function / c - course
    friend std::ostream& operator<<(std::ostream& os, const Course& c);

    //getters
    std::string getCourseCode() const {
        return courseCode;
    }
    std::string getCourseTitle() const {
        return courseTitle;
    }
    int getCreditHours() const {
        return creditHours;
    }
    const std::vector<std::shared_ptr<Assessment>>& getAssessments() const {
        return assessments;
    }
};
#endif