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

#ifndef TRANSFER_H
#define TRANSFER_H

#include <time.h>
#include <stdio.h>
#include "database.h"

int transfer(char * from_account, char * to_account,
			 char * value,
			 char * date, char * timestr,
			 char * date_format,
			 char * currency, char * VATrate);

#endif
