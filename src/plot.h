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

#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "international.h"
#include "conversions.h"
#include "piechart.h"

int plot(char * title, char * subtitle, int balance_field,
         char * export_filename,
         int width, int height,
         char * image_filename);

int plot_distribution(char * title, char * subtitle,
                      int width, int height,
                      char * image_filename,
                      int * distribution, double maximum,
                      int distribution_size);

int plot_volume(char * title, char * subtitle,
                char * export_filename,
                int width, int height,
                char * image_filename);

int plot_piechart(char * account,
                  char * search_string, int year, int month_number,
                  char field_separator, char * currency,
                  int spending,
                  char * title,
                  int width, int height,
                  char * image_filename);

#endif
