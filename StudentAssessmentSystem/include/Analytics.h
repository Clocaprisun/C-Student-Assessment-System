#ifndef GUIASSIGNMENT_ANALYTICS_H
#define GUIASSIGNMENT_ANALYTICS_H

#include <vector>

template <typename T>
class Analytics {
public:

    /* calculates the average of the values in a vector(assuming not null/empty)
     * returns double average
     */
    static double computeAverage(const std::vector<T>& data) {
        double sum = 0.0;
        for (const T& value : data) {
            sum += static_cast<double>(value);
        }
        return sum / data.size();
    }

    /* finds the maximum value in a vector(assumming not null/empty)
     * returns maximum
     */
    static T findMax(const std::vector<T>& data) {
        T max = data[0];
        for (const T& value : data) {
            if (value > max) {
                max = value;
            }
        }
        return max;
    }

    /* finds the maximum value in a vector(assumming not null/empty)
     * returns minimum
     */
    static T findMin(const std::vector<T>& data) {
        T min = data[0];
        for (const T& value : data) {
            if (value < min) {
                min = value;
            }
        }
        return min;
    }
};
#endif