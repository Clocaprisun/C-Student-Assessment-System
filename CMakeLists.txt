cmake_minimum_required(VERSION 3.15)
project(GUIAssignment LANGUAGES CXX)

# Use C++17, which is recommended for modern Nana versions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Define Nana Path ---
set(NANA_ROOT "${CMAKE_SOURCE_DIR}/StudentAssessmentSystem/external/nana")

# --- 2. Compile Nana Source Files ---
# Nana is NOT header-only. We must compile the .cpp files in /source.
# This command gathers all .cpp files recursively from the source folder.
file(GLOB_RECURSE NANA_SOURCES "${NANA_ROOT}/source/*.cpp")

# Create a static library named 'NanaLibrary' from these source files
add_library(NanaLibrary STATIC ${NANA_SOURCES})

# --- 3. Setup Include Directories ---
# Allow the library and the main app to see the headers in /include
target_include_directories(NanaLibrary PUBLIC "${NANA_ROOT}/include")

# --- 4. Link System Dependencies ---
if(WIN32)
    # Windows requires GDI and other system libs
    target_link_libraries(NanaLibrary PUBLIC
            Gdi32
            User32
            ComDlg32
            Advapi32
    )
elseif(UNIX AND NOT APPLE)
    # Linux requires X11, Threads, and FontConfig
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    target_link_libraries(NanaLibrary PUBLIC
            X11::X11
            Threads::Threads
            stdc++fs
    )
endif()

# --- 5. Your Executable ---
add_executable(GUIAssignment StudentAssessmentSystem/src/main.cpp
        StudentAssessmentSystem/src/Course.cpp
        StudentAssessmentSystem/include/Course.h
        StudentAssessmentSystem/src/DataManager.cpp
        StudentAssessmentSystem/src/GUI.cpp
        StudentAssessmentSystem/src/Student.cpp
        StudentAssessmentSystem/include/Analytics.h
        StudentAssessmentSystem/include/Assessment.h
        StudentAssessmentSystem/include/Assignment.h
        StudentAssessmentSystem/include/DataManager.h
        StudentAssessmentSystem/include/Exam.h
        StudentAssessmentSystem/include/Quiz.h
        StudentAssessmentSystem/include/Student.h)

# Link your app to the Nana library we just built
target_link_libraries(GUIAssignment PRIVATE NanaLibrary)