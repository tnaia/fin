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

#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_integer(char * str);
int is_value(char * str);
int is_money(char * str, char * curr);
int is_date(char * str, char * date_format);
void remove_currency_symbol(char * value);
int valid_language(char * language);

#endif
