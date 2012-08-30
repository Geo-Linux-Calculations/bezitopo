/******************************************************/
/*                                                    */
/* point.h - classes for points and gradients         */
/*                                                    */
/******************************************************/

#ifndef POINT_H
#define POINT_H
#include <string>
//#include "tin.h"

class xyz;
class point;
class edge;

using std::string;

class xy
{public:
 xy(double e,double n);
 xy(xyz point);
 xy();
 double east();
 double north();
 friend xy operator+(const xy &l,const xy &r);
 friend xy operator+=(xy &l,const xy &r);
 friend xy operator-(const xy &l,const xy &r);
 friend xy operator*(const xy &l,double r);
 friend xy operator/(const xy &l,double r);
 friend xy operator/=(xy &l,double r);
 friend bool operator!=(const xy &l,const xy &r);
 friend bool operator==(const xy &l,const xy &r);
 friend xy turn90(xy a);
 friend double dist(xy a,xy b);
 friend double dir(xy a,xy b);
 friend double dot(xy a,xy b);
 friend class point;
 friend class qindex;
 protected:
 double x,y;
 };

/*class farna: public xy
{
 private:
 void normalize();
 }

farna perp(farna dir);*/

class xyz
{public:
 xyz(double e,double n,double h);
 xyz();
 double east();
 double north();
 double elev();
 friend class xy;
 friend class triangle;
 friend double dist(xyz a,xyz b);
 friend bool operator==(const xyz &l,const xyz &r);
 protected:
 double x,y,z;
 };

class point: public xyz
{public:
 //xy pagepos; //used when dumping a lozenge in PostScript
 point();
 point(double e,double n,double h,string desc);
 point(xy pnt,double h,string desc);
 point(const point &rhs);
 //~point();
 const point& operator=(const point &rhs);
 //void setedge(point *oend);
 void dump();
 friend class edge;
 friend void maketin();
 friend void rotate(int n);
 friend void movesideways(double sw);
 short flags;
 /* 0: a normal point from the input file
    1: a point resulting from merging several points
    2: an extremal point in an edge
    3: an extremal point inside a triangle
    4: an explicitly ignored point
    5: a point ignored because it's in a group that was merged
    */
 string note;
 edge *line; // a line incident on this point in the TIN. Used to arrange the lines in order around their endpoints.
 public:
 xy newgradient,gradient,oldgradient; // may be redone later, but for now a point has one gradient, and type 0 breaklines merely force edges to be in the TIN.
 };
#endif
