/******************************************************/
/*                                                    */
/* except.h - exceptions                              */
/*                                                    */
/******************************************************/
/* Copyright 2017 Pierre Abbat.
 * This file is part of Bezitopo.
 * 
 * Bezitopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bezitopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bezitopo. If not, see <http://www.gnu.org/licenses/>.
 */

/* For now, I'm just collecting the various integers thrown as exceptions.
 * Later, I'll make an exception class which can carry additional data
 * to the catch site. This will require all programs to be linked with Qt.
 */
#define notri 1
// less than 3 points, no triangle
#define samepnts 2
// two points are at same xy coordinates when making a TIN
#define flattri 3
// all the points are in a line
#define badheader 4
// when reading a geoid file
#define baddata 5
// when reading a geoid file
#define matrixmismatch 6
// operation on matrices is impossible because the sizes don't match
#define singularmatrix 7
// not actually used - for inverting a singular matrix
#define unsetgeoid 8
// trying to write a geoid file when the pointer in the geoid structure is null
#define unsetsource 9
// trying to copy topo points from a nonexistent pointlist
#define badunits 10
// unrecognized unit symbol in a measurement
#define badnumber 11
// malformatted (empty) number in a measurement