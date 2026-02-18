#include "../include/Student.h"
#include <iostream>

Student::Student(const std::string& id, const std::string& name, const std::string& program): studentID(id), name(name), program(program) {}

/* takes a course and pushes a copy of it onto student's private courses
 vector */
void Student::enrollCourse(const Course& course) {
    courses.push_back(course);
}
/* computes the selected student's GPA
  returns 0-4 based on percentile */
double Student::computeGPA() const {
    double GPA = 0;
    for (const auto& c : courses) {
        GPA += c.computeAverage();
    }
    double p = GPA / courses.size();
    if (p >= 90) {return 4.0;}
    if (p >= 80) {return 3.0;}
    if (p >= 70) {return 2.0;}
    if (p >= 60) {return 1.0;}
    return 0.0;
}
/* Not explicitly used but can be used for console printing
and debugging
 */
void Student::displayTranscript() const{
    std::cout << "Transcript for " << name << " (" << studentID << ")" << std::endl;
    std::cout << "========================================\n" << std::endl;
    std::cout << "Program" << program << "\n" << std::endl;
    std::cout << "========================================\n" << std::endl;
    //prints students average
    for (const auto& c : courses) {
        std::cout << c << " | Final: " << c.computeAverage() << "\n" << std::endl;
    }
    //prints students gpa
    std::cout << "========================================\n" << std::endl;
    std::cout << "GPA: " << computeGPA() << "\n" << std::endl;
}

/* checks if 2 student id's are the same - unique key - cannot have 2
 * returns true or false
 */
bool operator==(const Student& s1, const Student& s2) {
    return s1.studentID == s2.studentID;
}

/* allows you to print a student object as shown below: */
std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.studentID << " - " << s.name << " (" << s.program << ") " <<std::endl;
    return os;
}

