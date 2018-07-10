/******************************************************/
/*                                                    */
/* dxf.h - Drawing Exchange Format                    */
/*                                                    */
/******************************************************/
/* Copyright 2018 Pierre Abbat.
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
#include <string>
#include "xyz.h"

struct TagRange
{
  int tag;
  int format;
  /* 000 00: invalid
   * 001 01: bool
   * 002 02: short
   * 004 04: int
   * 008 08: long long
   * 072 48: double
   * 128 80: string
   * 129 81: hex string representing binary chunk
   * 132 84: hex string representing int
   * Only 128 and 129 are stored as strings.
   * 132 is read as string, but stored as integer.
   */
};

int tagFormat(int tag);

class GroupCode
{
public:
  GroupCode();
  GroupCode(int tag0);
  GroupCode(const GroupCode &b);
  GroupCode& operator=(const GroupCode &b);
  GroupCode(GroupCode&&)=default;
  GroupCode& operator=(GroupCode&&)=default;
  ~GroupCode();
  int tag; // short in binary file
  union
  {
    std::string str;
    double real;
    long long integer;
    bool flag;
  };
};
