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

#ifndef YEARLY_H
#define YEARLY_H

#include <sqlite3.h>
#include "parse.h"
#include "database.h"
#include "summary.h"

int show_yearly_summary(char * account, int max_months,
						char * export_filename, char separator,
						char * currency, char column_separator);

#endif
