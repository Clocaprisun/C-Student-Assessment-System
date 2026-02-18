//nana include
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/msgbox.hpp>
//c++ include
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
//include
#include "../include/Student.h"
#include "../include/Course.h"
#include "../include/DataManager.h"
#include "../include/Analytics.h"
#include "../include/Quiz.h"
#include "../include/Exam.h"
#include "../include/Assignment.h"

/* full GUI class with panels and buttons and updating logic as well as popups to change the interface
 * completely runs the program by including all the h files
 */
class GUI {
private:
    nana::form mainForm{ nana::API::make_center(950, 600) };

    //widgets
    nana::listbox studentListBox{ mainForm, nana::rectangle(10, 30, 250, 480) };
    nana::listbox courseListBox{ mainForm, nana::rectangle(270, 30, 380, 230) };
    nana::listbox assessmentListBox{ mainForm, nana::rectangle(270, 290, 380, 220) };

    //analytics square
    nana::label lblAnalytics{ mainForm, nana::rectangle(660, 30, 250, 250) };

    //headers
    nana::label lblStudentsHeader{ mainForm, nana::rectangle(10, 5, 200, 20) };
    nana::label lblCoursesHeader{ mainForm, nana::rectangle(270, 5, 200, 20) };
    nana::label lblAssessmentsHeader{ mainForm, nana::rectangle(270, 265, 200, 20) };
    nana::label lblAnalyticsHeader{ mainForm, nana::rectangle(660, 5, 200, 20) };

    //buttons
    nana::button btnAddStudent{ mainForm, nana::rectangle(10, 520, 100, 30) };
    nana::button btnAddCourse{ mainForm, nana::rectangle(120, 520, 100, 30) };
    nana::button btnAddAssessment{ mainForm, nana::rectangle(230, 520, 120, 30) };
    nana::button btnSave{ mainForm, nana::rectangle(700, 520, 100, 30) };
    nana::button btnLoad{ mainForm, nana::rectangle(810, 520, 100, 30) };
    nana::button btnRefresh{ mainForm, nana::rectangle(660, 300, 200, 30) };

    //student data
    std::vector<Student> students;

    //student/course selection
    int selectedStudentIndex = -1;
    int lastClickedCourseIndex = -1;

public:
    GUI() {
        setupUI();
        bindEvents();
    }

    /*sets up all the main UI and interface/buttons*/
    void setupUI() {
        //main interface
        mainForm.caption("Student Assessment System");

        lblStudentsHeader.caption("Students");
        lblCoursesHeader.caption("Courses (Multi-select allowed)");
        lblAssessmentsHeader.caption("Assessments");
        lblAnalyticsHeader.caption("Student Analytics");

        lblAnalytics.caption("Select a student to view performance.");
        lblAnalytics.bgcolor(nana::colors::alice_blue); // cute color forvariation
        lblAnalytics.text_align(nana::align::left, nana::align_v::top);

        //when the addStudent popup appears:
        btnAddStudent.caption("Add Student");
        //when the addCourse popup appears:
        btnAddCourse.caption("Add Course");
        //when the addAssessment popup appears:
        btnAddAssessment.caption("Add Assess.");
        btnSave.caption("Save Data");
        btnLoad.caption("Load Data");
        btnRefresh.caption("Refresh Stats");

        //ability to select multiple courses - get the average of 2 specific courses out of 3 etc
        courseListBox.enable_single(false, false);

        //headers
        studentListBox.append_header("ID", 60);
        studentListBox.append_header("Name", 110);
        studentListBox.append_header("Course", 80);

        courseListBox.append_header("Code", 80);
        courseListBox.append_header("Title", 150);
        courseListBox.append_header("Avg", 60);

        assessmentListBox.append_header("Type", 80);
        assessmentListBox.append_header("Score", 80);
        assessmentListBox.append_header("Wgt", 60);
        assessmentListBox.append_header("Details", 100);
    }

    /* handles the selections and button events*/
    void bindEvents() {
        //select a specific student
        studentListBox.events().selected([this](const nana::arg_listbox& arg) {
            if (!arg.item.selected()) return;
            auto sel = studentListBox.selected();
            if (sel.empty()) return;

            //finds which student was picked in index
            selectedStudentIndex = sel[0].item;
            lastClickedCourseIndex = -1;

            //changes the courses and analytics based on student chosen
            refreshCourses();
            assessmentListBox.clear();
            updateAnalytics();
        });

        //select a specific course - if student is selected
        courseListBox.events().selected([this](const nana::arg_listbox& arg) {
            auto sel = courseListBox.selected();

            //updates courses based on student picked
            if (sel.size() == 1) {
                lastClickedCourseIndex = sel[0].item;
                refreshAssessments();
            } else if (sel.empty()) {
                lastClickedCourseIndex = -1;
                assessmentListBox.clear();
            }
            updateAnalytics();
        });

        //buttons
        btnAddStudent.events().click([this] { addStudentPopup(); });
        btnAddCourse.events().click([this] {
            if (selectedStudentIndex == -1) {
                nana::msgbox(mainForm, "Error: ") << "Please select a student first.";
                return;
            }
            addCoursePopup();
        });
        btnAddAssessment.events().click([this] {
            if (selectedStudentIndex == -1 || lastClickedCourseIndex == -1) {
                nana::msgbox(mainForm, "Error: ") << "Please select a student and a SINGLE course.";
                return;
            }
            addAssessmentPopup();
        });

        //save and load buttons
        btnSave.events().click([this] {
            fileInputPopup(true);
        });
        btnLoad.events().click([this] {
            fileInputPopup(false);
        });
        //refresh the analytics
        btnRefresh.events().click([this] { updateAnalytics(); });
    }

    /* refreshes the student data/GUI */
    void refreshStudents() {
        studentListBox.auto_draw(false);
        studentListBox.clear();
        for (const auto& s : students) {
            studentListBox.at(0).append({ s.getStudentID(), s.getName(), s.getProgram() });
        }
        studentListBox.auto_draw(true);
    }

    /*refreshes the course data/GUI */
    void refreshCourses() {
        courseListBox.auto_draw(false);
        courseListBox.clear();

        if (selectedStudentIndex >= 0 && selectedStudentIndex < students.size()) {
            const auto& courses = students[selectedStudentIndex].getCourses();
            for (const auto& c : courses) {
                std::stringstream ssAvg;
                ssAvg << std::fixed << std::setprecision(1) << c.computeAverage();
                courseListBox.at(0).append({ c.getCourseCode(), c.getCourseTitle(), ssAvg.str() });
            }
        }
        courseListBox.auto_draw(true);
    }

    /* refreshes the assessment data/GUI*/
    void refreshAssessments() {
        assessmentListBox.auto_draw(false);
        assessmentListBox.clear();

        if (selectedStudentIndex >= 0 && lastClickedCourseIndex >= 0) {
            const auto& courses = students[selectedStudentIndex].getCourses();
            if (lastClickedCourseIndex < courses.size()) {
                const auto& assessments = courses[lastClickedCourseIndex].getAssessments();
                for (const auto& a : assessments) {
                    std::stringstream ssScore, ssWgt, ssInfo;
                    ssScore << a->getScore() << "/" << a->getMaxScore();
                    ssWgt << a->getWeight();

                    if (auto q = std::dynamic_pointer_cast<Quiz>(a)) ssInfo << q->getDuration() << "m";
                    else if (auto e = std::dynamic_pointer_cast<Exam>(a)) ssInfo << e->getDuration() << "m";
                    else if (auto as = std::dynamic_pointer_cast<Assignment>(a)) ssInfo << as->getDeadline();

                    assessmentListBox.at(0).append({ a->getType(), ssScore.str(), ssWgt.str(), ssInfo.str() });
                }
            }
        }
        assessmentListBox.auto_draw(true);
    }

    /*updates the analytics box if a student is selected*/
    void updateAnalytics() {
        if (selectedStudentIndex < 0 || selectedStudentIndex >= students.size()) {
            lblAnalytics.caption("Select a student to view stats.");
            return;
        }

        const auto& s = students[selectedStudentIndex];
        const auto& courses = s.getCourses();

        //if selected student has no courses
        if (courses.empty()) {
            lblAnalytics.caption("Student has no courses.");
            return;
        }

        auto selectedRows = courseListBox.selected();
        std::vector<double> scoresToAnalyze;
        std::string scopeText;

        //shows the overall if no courses are selected
        if (selectedRows.empty()) {
            scopeText = "Overall Performance (All Courses)";
            for (const auto& c : courses) {
                scoresToAnalyze.push_back(c.computeAverage());
            }
        } else {
            scopeText = "Selected Courses Analysis";
            for (const auto& sel : selectedRows) {
                if (sel.item < courses.size()) {
                    scoresToAnalyze.push_back(courses[sel.item].computeAverage()); //TODO this creates an error where max and min show average
                }
            }
        }
        //gets scores data if there are any
        if (scoresToAnalyze.empty()) {
             lblAnalytics.caption("No data in selected scope.");
             return;
        }

        double avg = Analytics<double>::computeAverage(scoresToAnalyze);
        double max = Analytics<double>::findMax(scoresToAnalyze);
        double min = Analytics<double>::findMin(scoresToAnalyze);

        std::stringstream ss;
        ss << "Student: " << s.getName() << " (" << s.getStudentID() << ")\n";
        ss << "----------------------------\n";
        ss << scopeText << "\n";
        ss << "----------------------------\n";
        ss << "Courses: " << scoresToAnalyze.size() << "\n";
        ss << "Average: " << std::fixed << std::setprecision(2) << avg << "%\n";
        ss << "Highest: " << max << "%\n";
        ss << "Lowest:  " << min << "%\n";

        lblAnalytics.caption(ss.str());
    }

    /* ---------------Popup Logic :)--------------------------------*/
    /*gets the filename if you want to save/load, defaults to data.txt*/
    void fileInputPopup(bool isSave) {
        //tertiary operators change output based on save or load
        nana::form fm(mainForm, nana::API::make_center(300, 150));
        fm.caption(isSave ? "Save to File" : "Load from File");
        nana::label lbl(fm, nana::rectangle(20, 20, 260, 20));
        lbl.caption(isSave ? "Enter filename to save:" : "Enter filename to load:");
        nana::textbox tFile(fm, nana::rectangle(20, 50, 260, 30));
        tFile.multi_lines(false);
        tFile.caption("data.txt");
        nana::button btnAction(fm, nana::rectangle(50, 90, 80, 30));
        btnAction.caption(isSave ? "Save" : "Load");
        nana::button btnCancel(fm, nana::rectangle(170, 90, 80, 30));
        btnCancel.caption("Cancel");

        btnAction.events().click([&, isSave] {
            std::string filename = tFile.text();
            if (filename.empty()) {
                nana::msgbox(fm, "Error") << "Filename cannot be empty.";
                return;
            }
            if (filename.find(".txt") == std::string::npos) {
                filename += ".txt";
            }
            //goes to datamanager and attempts to load/save the data selected
            try {
                DataManager dm(filename);
                if (isSave) { //saves the current status using the method in datamanager
                    dm.saveData(students);
                    nana::msgbox(mainForm, "Success") << "Data saved to " << filename;
                } else {
                   //refreshed UI and loads the data from selected file(if exists)
                    students = dm.loadData();
                    selectedStudentIndex = -1;
                    lastClickedCourseIndex = -1;
                    refreshStudents();
                    courseListBox.clear();
                    assessmentListBox.clear();
                    updateAnalytics();
                    nana::msgbox(mainForm, "Success") << "Data loaded from " << filename;
                }
                fm.close();
            } catch (const std::exception& e) {
                nana::msgbox(fm, "Error") << e.what();
            }
        });
        btnCancel.events().click([&] { fm.close(); });
        fm.modality();//cannot click other things while menu is open
    }

    /* add student popup - allows user to enter information to add a student to the system */
    void addStudentPopup() {
        nana::form fm(mainForm, nana::API::make_center(300, 200));
        fm.caption("Add Student");

        //labels and textboxes for user to add the info
        nana::label lblID(fm, nana::rectangle(20, 20, 60, 20)); lblID.caption("ID:");
        nana::textbox tID(fm, nana::rectangle(80, 20, 200, 25));
        nana::label lblName(fm, nana::rectangle(20, 55, 60, 20)); lblName.caption("Name:");
        nana::textbox tName(fm, nana::rectangle(80, 55, 200, 25));
        nana::label lblProg(fm, nana::rectangle(20, 90, 60, 20)); lblProg.caption("Prog:");
        nana::textbox tProg(fm, nana::rectangle(80, 90, 200, 25));
        nana::button btnSave(fm, nana::rectangle(60, 140, 80, 30)); btnSave.caption("Save");
        nana::button btnCancel(fm, nana::rectangle(160, 140, 80, 30)); btnCancel.caption("Cancel");

        btnSave.events().click([&] {
            if(!tID.text().empty() && !tName.text().empty()) {
                //check if student already exists using operator==
                Student tempStudent(tID.text(), tName.text(), tProg.text());
                bool exists = false;
                for (const auto& s : students) {
                    if (s == tempStudent) { // USES OPERATOR==
                        exists = true;
                        break;
                    }
                }
                //if the student exists, error
                if (exists) {
                    nana::msgbox(fm, "Error") << "Student ID " << tID.text() << " already exists!";
                } else {
                    //otherwise, add student to he end of the vector and update
                    students.push_back(tempStudent);
                    refreshStudents();
                    updateAnalytics();
                    fm.close();
                }
            } else {
                nana::msgbox(fm, "Error") << "ID and Name are required.";
            }
        });
        btnCancel.events().click([&] { fm.close(); });
        fm.modality();
    }

    /* adds a course to a student if a student is selected */
    void addCoursePopup() {
        nana::form fm(mainForm, nana::API::make_center(300, 200));
        fm.caption("Add Course");

        //labels and text to be edited by user
        nana::label lblCode(fm, nana::rectangle(20, 20, 60, 20)); lblCode.caption("Code:");
        nana::textbox tCode(fm, nana::rectangle(80, 20, 200, 25));
        nana::label lblTitle(fm, nana::rectangle(20, 55, 60, 20)); lblTitle.caption("Title:");
        nana::textbox tTitle(fm, nana::rectangle(80, 55, 200, 25));
        nana::label lblCred(fm, nana::rectangle(20, 90, 60, 20)); lblCred.caption("Cred:");
        nana::textbox tCred(fm, nana::rectangle(80, 90, 200, 25));
        nana::button btnSave(fm, nana::rectangle(60, 140, 80, 30)); btnSave.caption("Save");
        nana::button btnCancel(fm, nana::rectangle(160, 140, 80, 30)); btnCancel.caption("Cancel");

        //when save button is clicked, add the course to the student's course vector and update UI
        btnSave.events().click([&] {
            try {
                int cr = std::stoi(tCred.text());
                if (!tCode.text().empty()) {
                    students[selectedStudentIndex].enrollCourse(Course(tCode.text(), tTitle.text(), cr));
                    refreshCourses();
                    updateAnalytics();
                    fm.close();
                }
            } catch (...) {
                nana::msgbox(fm, "Error") << "Credits must be an integer.";
            }
        });
        btnCancel.events().click([&] { fm.close(); });
        fm.modality();
    }

    /* adds an assessment to a course from a student */
    void addAssessmentPopup() {
        nana::form fm(mainForm, nana::API::make_center(350, 320));
        fm.caption("Add Assessment");

        //labels and text fields for user to input information
        nana::label lblType(fm, nana::rectangle(20, 20, 60, 20)); lblType.caption("Type:");
        nana::combox cbType(fm, nana::rectangle(90, 20, 200, 25));
        cbType.push_back("Quiz"); cbType.push_back("Exam"); cbType.push_back("Assignment");
        cbType.option(0); //maks you pick from options because this is a selection and not a choice of words
        nana::label lblScore(fm, nana::rectangle(20, 55, 60, 20)); lblScore.caption("Score:");
        nana::textbox tScore(fm, nana::rectangle(90, 55, 200, 25));
        nana::label lblMax(fm, nana::rectangle(20, 90, 60, 20)); lblMax.caption("Max:");
        nana::textbox tMax(fm, nana::rectangle(90, 90, 200, 25));
        nana::label lblWeight(fm, nana::rectangle(20, 125, 60, 20)); lblWeight.caption("Wgt:");
        nana::textbox tWeight(fm, nana::rectangle(90, 125, 200, 25)); tWeight.tip_string("0.0 - 1.0");
        nana::label lblExtra(fm, nana::rectangle(20, 160, 60, 20)); lblExtra.caption("Dur:");
        nana::textbox tExtra(fm, nana::rectangle(90, 160, 200, 25)); tExtra.tip_string("Mins (or Deadline)");

        //change the output based on what is selected - assignment
        cbType.events().selected([&](const nana::arg_combox&){
             if(cbType.text(cbType.option()) == "Assignment") lblExtra.caption("Deadl:");
             else lblExtra.caption("Dur:");
        });

        nana::button btnSave(fm, nana::rectangle(80, 210, 80, 30)); btnSave.caption("Save");
        nana::button btnCancel(fm, nana::rectangle(180, 210, 80, 30)); btnCancel.caption("Cancel");

        //change the output based on what is selected - quiz/exam
        btnSave.events().click([&] {
            try {
                //text fields to numbers so they can be used for calculations (string to double)
                double s = std::stod(tScore.text());
                double m = std::stod(tMax.text());
                double w = std::stod(tWeight.text());
                //get the extra field and the type selection
                std::string extra = tExtra.text();
                std::string type = cbType.text(cbType.option());

                std::shared_ptr<Assessment> ptr;
                //quizzes and exams have duration which needs to be an integer
                if(type == "Quiz") ptr = std::make_shared<Quiz>(type, s, m, w, std::stoi(extra)); //turn text into int(stoi)
                else if(type == "Exam") ptr = std::make_shared<Exam>(type, s, m, w, std::stoi(extra));
                //assignments have a deadline
                else ptr = std::make_shared<Assignment>(type, s, m, w, extra);

                //allow modification temp, to add the assessment
                auto& courses = const_cast<std::vector<Course>&>(students[selectedStudentIndex].getCourses());
                courses[lastClickedCourseIndex].addAssessment(ptr);

                //update UI
                refreshCourses();
                refreshAssessments();
                updateAnalytics();
                fm.close();
            } catch (...) {
                nana::msgbox(fm, "Error") << "Invalid numeric input.";
            }
        });

        btnCancel.events().click([&] { fm.close(); });
        fm.modality();
    }

    /*runs the program and GUI*/
    void run() {
        mainForm.show();
        nana::exec();
    }
};