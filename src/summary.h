/*
    Fin - a simple personal accounts manager
    Copyright (C) 2012-2013  Bob Mottram <bob@sluggish.dyndns.org>

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

#ifndef SUMMARY_H
#define SUMMARY_H

#include <sqlite3.h>
#include "parse.h"
#include "database.h"

double get_current_balance(char * account, char * currency);
int show_recent_transactions(char * account, int no_of_transactions,
							 char column_separator);
int show_summary(char * account, int no_of_transactions,
				 char column_separator);
int show_summary_balance(char * account);
int show_recent_adjustments(char * account, int no_of_transactions,
							char column_separator,
							char * search_string,
							int year, int month_number);

#endif
