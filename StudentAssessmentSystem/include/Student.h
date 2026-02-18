
#ifndef GUIASSIGNMENT_STUDENT_H
#define GUIASSIGNMENT_STUDENT_H
#include <iomanip>

#include "Course.h"

class Student {
private:
    std::string studentID;
    std::string name;
    std::string program;
    std::vector<Course> courses;

public:
    Student(const std::string& id, const std::string& name, const std::string& program);

    void enrollCourse(const Course& course);
    double computeGPA() const;
    void displayTranscript() const;

    //friend functions
    friend bool operator==(const Student& s1, const Student& s2);
    friend std::ostream& operator<<(std::ostream& os, const Student& s);

    //getters
    std::string getStudentID() const {
        return studentID;
    }
    std::string getName() const {
        return name;
    }
    std::string getProgram() const {
        return program;
    }
    const std::vector<Course>& getCourses() const {
        return courses;
    }
};
#endif