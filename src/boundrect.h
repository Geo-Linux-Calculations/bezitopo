/******************************************************/
/*                                                    */
/* boundrect.h - bounding rectangles                  */
/*                                                    */
/******************************************************/
/* Copyright 2018,2020 Pierre Abbat.
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
#ifndef BOUNDRECT_H
#define BOUNDRECT_H
class BoundRect;
#include <memory>
#include "pointlist.h"

class BoundRect
{
private:
  int orientation;
  std::array<double,4> bounds;
  // The four numbers are left, bottom, -right, and -top.
public:
  BoundRect();
  BoundRect(int ori);
  void clear();
  void setOrientation(int ori);
  int getOrientation();
  void include(xy obj);
  void include(drawobj *obj); // Expands the rectangle to include the object.
  void include(std::shared_ptr<drawobj> obj);
#ifdef POINTLIST
  void include(pointlist *obj);
#endif
  double left()
  {
    return bounds[0];
  }
  double bottom()
  {
    return bounds[1];
  }
  double right()
  {
    return -bounds[2];
  }
  double top()
  {
    return -bounds[3];
  }
};
#endif
