# Student Assessment System
A GUI-based application to manage student records, enrollments, and performance evaluations using C++ and the nana library. 

## Features
- User Input
	- Student Management: Add new students with unique IDs, Names, and Program details.
	- Course Enrollment: Enroll selected students into courses (specifying Course Code, Title, and Credit Hours)

	- Assessment Entry: Add specific assessment items to courses. Supports three distinct types:
		- Quiz: Includes duration.
		- Exam: Includes duration.
		- Assignment: Includes submission deadline
		- All assessments require a Score, Max Score, and Weighting factor (0.0 - 1.0)

- System Logic & GUI
	- Master-Detail View: Interactive lists allow users to select a Student to view their Courses, and select a Course to view its Assessments
	- Polymorphism: Automatically calculates weighted scores differently based on the assessment type.
	- GPA Calculation: Computes individual course averages and overall Student GPA based on enrolled courses.

- Analytics Dashboard

	- Class Statistics: Utilizes a template-based Analytics engine to compute:
		- Total number of students.
		- Average GPA of the entire class.
		- Highest and Lowest GPA in the dataset.

- Data Persistence
	- Save Functionality: Serializes the entire object hierarchy (Student -> Courses -> Assessments) to a formatted text file (data.txt).

	- Load Functionality: Parses the text file to reconstruct objects and restore the application state.

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
