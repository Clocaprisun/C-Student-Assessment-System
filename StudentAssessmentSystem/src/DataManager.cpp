#include "../include/DataManager.h"
#include "../include/Quiz.h"
#include "../include/Exam.h"
#include "../include/Assignment.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

// Constructor
DataManager::DataManager(const std::string& filename) : filename(filename) {}

// Helper function to split a string by a delimiter
// Example: "S|101|John" -> ["S", "101", "John"]
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/* saves data in a text file */
void DataManager::saveData(const std::vector<Student>& students) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    //iterate through every student
    for (const auto& s : students) {
        // header: S|ID|Name|Program
        out << "S|" << s.getStudentID() << "|" << s.getName() << "|" << s.getProgram() << "\n";

        // iterate through the student's courses
        for (const auto& c : s.getCourses()) {
            // course header: C|Code|Title|Credits
            out << "C|" << c.getCourseCode() << "|" << c.getCourseTitle() << "|" << c.getCreditHours() << "\n";

            //iterate through the course's assessments
            for (const auto& a : c.getAssessments()) {
                // save assessment data A|Type|Score|Max|Weight|ExtraData
                std::string type = a->getType();
                std::string extra = "0"; // Default for unknown types

                //determine which kind of assessment was picked in order to save it properly
                if (auto q = std::dynamic_pointer_cast<Quiz>(a)) {
                    extra = std::to_string(q->getDuration());
                } else if (auto e = std::dynamic_pointer_cast<Exam>(a)) {
                    extra = std::to_string(e->getDuration());
                } else if (auto assign = std::dynamic_pointer_cast<Assignment>(a)) {
                    extra = assign->getDeadline();
                }

                //get the information
                out << "A|" << type << "|"
                    << a->getScore() << "|"
                    << a->getMaxScore() << "|"
                    << a->getWeight() << "|"
                    << extra << "\n";
            }
        }
    }
    out.close();
    std::cout << "Data successfully saved to " << filename << std::endl;
}

/*loads data from an existing text file*/
std::vector<Student> DataManager::loadData() const {
    std::vector<Student> students;
    std::ifstream in(filename);

    if (!in) {
        std::cout << "File " << filename << " not found. Starting with empty data." << std::endl;
        return students;
    }
    std::string line;
    Student* currentStudent = nullptr; //pointer to the last student added
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        //split the line into parts and use delimiter |
        auto tokens = split(line, '|');
        if (tokens.empty()) continue; // skip empty tokens
        std::string tag = tokens[0];

        //student
        if (tag == "S") {
            if (tokens.size() >= 4) {
                // S|ID|Name|Program
                students.emplace_back(tokens[1], tokens[2], tokens[3]);
                currentStudent = &students.back(); //update pointer to current student
            }
        }
        //course
        else if (tag == "C") {
            if (tokens.size() >= 4 && currentStudent != nullptr) {
                // C|Code|Title|Credits
                int creds = 0;
                try { creds = std::stoi(tokens[3]); } catch(...) { creds = 0; }

                //create and enroll the course
                Course newCourse(tokens[1], tokens[2], creds);
                currentStudent->enrollCourse(newCourse);
            }
        }
        //assessment
        else if (tag == "A") {
            //check if we have a student and that student has courses
            if (tokens.size() >= 6 && currentStudent != nullptr && !currentStudent->getCourses().empty()) {
                // A|Type|Score|Max|Weight|Extra
                std::string type = tokens[1];
                double score = 0.0, max = 0.0, weight = 0.0;

                try {
                    score = std::stod(tokens[2]);
                    max = std::stod(tokens[3]);
                    weight = std::stod(tokens[4]);
                } catch(...) { continue; }

                std::string extra = tokens[5];

                //create the specific assessment object with quizes, exams, assignments
                std::shared_ptr<Assessment> ptr;
                if (type == "Quiz") {
                    ptr = std::make_shared<Quiz>(type, score, max, weight, std::stoi(extra));
                } else if (type == "Exam") {
                    ptr = std::make_shared<Exam>(type, score, max, weight, std::stoi(extra));
                } else if (type == "Assignment") {
                    ptr = std::make_shared<Assignment>(type, score, max, weight, extra);
                }

                //changes read-only object(const) so it can be temporarily modified
                if (ptr) {
                    auto& courses = const_cast<std::vector<Course>&>(currentStudent->getCourses());
                    courses.back().addAssessment(ptr);
                }
            }
        }
    }

    //success!
    in.close();
    std::cout << "Data loaded successfully. Found " << students.size() << " students." << std::endl;
    return students;
}
