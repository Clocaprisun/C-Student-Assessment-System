# Student Assessment System
A GUI-based application to manage student records, enrollments, and performance evaluations using C++ and the nana library.

**Note:** 
	- You have to install nana GUI library from GitHub and put the folder in the external folder
	- This project was developed locally, and the files were uploaded to GitHub as a mostly completed package. The commit history does not reflect the development of the project.

## Features
- user input
	- student management: add new students with unique IDs, names, and program details.
	- course enrollment: enroll selected students into courses (specifying course code, title, and credit hours)

	- assessment entry: add specific assessment items to courses. supports three distinct types:
		- quiz: includes duration.
		- exam: includes duration.
		- assignment: includes submission deadline
		- all assessments require a Score, Max Score, and Weighting factor (0.0 - 1.0)

- system logic & GUI
	- master-detail View: interactive lists allow users to select a Student to view their Courses, and select a Course to view its assessments
	- polymorphism: automatically calculates weighted scores differently based on the assessment type.
	- GPA calculation: computes individual course averages and overall Student GPA based on enrolled courses.

- analytics dashboard

	- class statistics: utilizes a template-based analytics engine to compute:
		- total number of students.
		- average GPA of the entire class.
		- highest and lowest GPA in the dataset.

- data persistence
	- save functionality: serializes the entire object hierarchy (Student -> Courses -> Assessments) to a formatted text file (data.txt).

	- load functionality: parses the text file to reconstruct objects and restore the application state.

## Folder/File Structure 

- /StudentAssessmentSystem
	- /data
		- student.txt
	- /external
		- /nana
	- /include
		- Analytics.h
		- Assessment.h
		- Assignment.h
		- Course.h
		- DataManager.h
		- Exam.h
		- Quiz.h
		- Student.h
	- /src
		- Course.cpp
		- DataManager.cpp
		- GUI.cpp
		- main.cpp
		- Student.cpp
- CMakeLists.txt
- README.md
