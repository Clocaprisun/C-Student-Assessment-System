
#ifndef GUIASSIGNMENT_DATAMANAGER_H
#define GUIASSIGNMENT_DATAMANAGER_H

#include <string>
#include <vector>
#include "Student.h"

class DataManager {
private:
    std::string filename;

public:
    DataManager(const std::string& filename);

    void saveData(const std::vector<Student>& students) const;
    std::vector<Student> loadData() const;
};
#endif