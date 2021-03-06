/******************************************************/
/*                                                    */
/* histogram.h - streaming histogram                  */
/*                                                    */
/******************************************************/
/* Copyright 2016-2018 Pierre Abbat.
 * This file is part of Bezitopo.
 *
 * Bezitopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Bezitopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License and Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and Lesser General Public License along with Bezitopo. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include <vector>
#include "ps.h"
#define HISTO_LINEAR 0
#define HISTO_LOG 1

struct histobar
{
  double start,end;
  unsigned count;
};

class histogram
{
private:
  std::vector<double> bin;
  std::vector<unsigned> count;
  double discrete;
  unsigned total;
  std::vector<histobar> intervals;
  void split(int n);
public:
  histogram();
  histogram(double least,double most);
  void setdiscrete(double d);
  void clear(); // leaves least and most intact, makes a single empty bin
  void clear(double least,double most);
  void clearcount(); // leaves bin widths intact, just clears all their counts
  void addinterval(double start,double end);
  int find(double val);
  histogram& operator<<(double val);
  unsigned nbars();
  histobar getbar(unsigned n);
  histobar getinterval(unsigned n);
  unsigned gettotal();
  void plot(PostScript &ps,int xtype);
  void dump();
};
#endif
