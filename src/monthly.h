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

#ifndef MONTHLY_H
#define MONTHLY_H

#include <sqlite3.h>
#include "parse.h"
#include "database.h"
#include "summary.h"
#include "plot.h"

int show_monthly_summary(char * account, int max_months, int year,
						 char * export_filename, char separator,
						 char * currency, char * image_filename,
						 char column_separator);

#endif
