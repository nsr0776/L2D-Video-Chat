#ifndef HELPER_HPP_
#define HELPER_HPP_

#include <string>
#include <cmath>


#include <QtNetwork/qnetworkinterface.h>
#include <QList>
std::string getHostMacAddress();

inline double euclideanDistance(double x1, double y1, double x2, double y2);
template<typename T>
struct euclideanPoint;

template<typename T>
inline euclideanPoint<T> rotate(T x, T y, double theta);


#endif