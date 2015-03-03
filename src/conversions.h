/*
  Fin - a simple personal accounts manager
  Copyright (C) 2012-2015  Bob Mottram <bob@robotics.uk.to>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

void money_to_integer(char * money, char * integer);
void integer_to_money(char * integer, char * money);

void quantity_to_integer(char * money, char * integer);
void integer_to_quantity(char * integer, char * money);

void pad_value(char * value, char * padded, int leading_spaces,
               int trailing_zeros);
void pad_text(char * str, int width);

void convert_date_to_standard(char * date,
                              char * date_format, char * converted);
void convert_standard_to_date(char * standard,
                              char * date_format, char * date);

int reverse_file(char * filename);
void trim_file_header(char * filename, char * trimmed);
void convert_time_to_standard(char * timestr, char * converted,
                              char * am, char * pm);

#endif
