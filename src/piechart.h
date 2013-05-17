/*
    Fin - a simple personal accounts manager
    Copyright (C) 2012-2013  Bob Mottram <bob@robotics.uk.to>

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

#ifndef PIECHART_H
#define PIECHART_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "parse.h"
#include "settings.h"
#include "conversions.h"
#include "database.h"
#include "plot.h"

int calc_piechart(char * account, char * search_string,
				  int month_number, int year,
				  char field_separator, char * currency,
				  int spending,
				  char ** fieldname, double * value,
				  double * total_value,
				  int max_fields,
				  int include_zeros);

int piechart(char * title, char * account, char * search_string,
			 int month_number, int year,
			 char field_separator, char column_separator,
			 char * currency,
			 int spending, char * export_filename);

#endif
