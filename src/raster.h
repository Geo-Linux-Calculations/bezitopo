/******************************************************/
/*                                                    */
/* raster.cpp - raster image output                   */
/*                                                    */
/******************************************************/
/* Copyright 2013,2015,2016,2017 Pierre Abbat.
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
#include "pointlist.h"
#include "geoid.h"
#ifdef NUMSGEOID
#include "sourcegeoid.h"
#endif

void rasterdraw(pointlist &pts,xy center,double width,double height,
	    double scale,int imagetype,double zscale,std::string filename);
#ifdef NUMSGEOID
void drawglobecube(int side,double zscale,double zmid,geoid *source,int imagetype,std::string filename);
#endif
void drawglobemicro(int side,xy center,double size,int source,int imagetype,std::string filename);
