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

#ifndef SEARCH_H
#define SEARCH_H

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

int search(char * account, char * search_string, int month_number,
           int year, int max_rows, char * date_format,
           char * export_filename, char field_separator,
           char * kml_filename, char column_separator);

int search_grouped(char * account, char * search_string, int year,
                   int max_rows, char * date_format,
                   char * export_filename, char field_separator,
                   char * image_filename, char column_separator,
                   int monthly);

#endif
