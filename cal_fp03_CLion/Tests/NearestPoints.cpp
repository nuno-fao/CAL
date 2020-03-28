/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
#include <algorithm>
#include <cmath>
#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0,0);
	this->p2 = Point(0,0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.x < q.x || (p.x == q.x && p.y < q.y); });
}

static void sortByY(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.y < q.y || (p.y == q.y && p.x < q.x); });
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
    Result res;
    res.dmin=9999999;
    double aux=0;
    for(size_t i=0;i<vp.size()-1;i++){
        for(size_t j=i+1;j<vp.size();j++){
            aux=vp[i].distance(vp[j]);
            if(aux<res.dmin){
                res.dmin=aux;
                res.p1=vp[i];
                res.p2=vp[j];
            }
        }
    }
    return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
    Result res;
    sortByX(vp, 0, vp.size()-1);
    for (unsigned i = 0; i < vp.size() && res.dmin > 0; i++) {
        Point p = vp[i];
        for (unsigned j = i + 1; j < vp.size(); j++) {
            if (vp[j].x - p.x > res.dmin)
                break;
            double dist = p.distance(vp[j]);
            if (dist < res.dmin)
                res = Result(dist, p, vp[j]);
        }
    }
    return res;
}


/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res)
{
    for (int i = left; i < right; i++) {
        Point p = vp[i];
        for (int j = i + 1; j <= right; j++) {
            if (vp[j].y - p.y > res.dmin)
                break;
            else {
                double d = p.distance(vp[j]);
                if (d < res.dmin)
                    res = Result(d, p, vp[j]);
            }
        }
    }
}

/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {
    // Base case of a single point (or none): no solution, so distance is MAX_DOUBLE
    if (right <= left)
        return Result();
    // Base case of two points
    if (right - left == 1)
        return Result(vp[left].distance(vp[right]), vp[left], vp[right]);

	// Divide in halves (left and right) and solve them recursively,
    // possibly in parallel (in case numThreads > 1)
    int middle = (right + left)/2;
    Result resL, resR;
    if (numThreads > 1) {
        thread t([&](){resL = np_DC(vp, left, middle, numThreads / 2);});
        resR = np_DC(vp, middle + 1, right, numThreads / 2);
        t.join();
    }
    else {
        resL = np_DC(vp, left, middle, 1);
        resR = np_DC(vp, middle + 1, right, 1);
    }
    // Select the best solution from left and right
    Result res = resL.dmin < resR.dmin? resL : resR;
    // Determine the strip area around middle point
    double midX = (vp[middle].x + vp[middle+1].x) / 2;
    int sLeft = middle, sRight = middle+1;
    while (sLeft > left && vp[sLeft-1].x > midX - res.dmin)
        sLeft--;
    while (sRight < right && vp[sRight+1].x < midX + res.dmin)
        sRight++;
    // Order points in strip area by Y coordinate
    sortByY(vp, sLeft, sRight);
    // Calculate nearest points in strip area (using npByY function)
    npByY(vp, sLeft, sRight, res);
    // Reorder points in strip area back by X coordinate
    sortByX(vp, sLeft, sRight);
    return res;
}


/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num)
{
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}


/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
