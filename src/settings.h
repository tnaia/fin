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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "international.h"
#include "parse.h"
#include "validation.h"


int settings_exist(char * directory);
int settings_load(char * directory);
void settings_save(char * directory);

char * settings_get_currency();
void settings_set_currency(char * curr);
char * settings_get_account();
void settings_set_account(char * acc);
char * settings_get_vat_rate();
void settings_set_vat_rate(char * acc);
char * settings_get_server();
void settings_set_server(char * srv);
char * settings_get_language();
void settings_set_language(char * srv);
char * settings_get_theme();
void settings_set_theme(char * theme_name);
char * settings_get_theme_colour(int category);
int settings_valid_theme(char * theme_name);

char * settings_get_date_format();
void settings_set_date_format(char * format);

#endif
