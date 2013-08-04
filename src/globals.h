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

#ifndef GLOBALS_H
#define GLOBALS_H

#define VERSION_NUMBER                 1.51

#define MAX_CURRENCIES                 11

#define MAX_ARGS                       32
#define FIELD_WIDTH                    256

#define DECIMAL_PLACES                 8
#define DECIMAL_PLACES_QUANTITY        8

#define LEADING_SPACES                 6
#define TRAILING_ZEROS                 2

#define MAX_ROW_LENGTH                 80

#define DEFAULT_INFLATION_RATE         2.0
#define DEFAULT_USURY_RATE             18.0

#define DISTRIBUTION_SIZE              100
#define DEFAULT_DISTRIBUTION_INCREMENT 5

/* the maximum number of years beyond which
   spending will be considered to be sustainable */
#define MAX_PREDICTION_YEARS           30

/* terminal colours */
#define USE_COLOURS

#define NORMAL  "\x1B[0m"
#define BLACK   "\x1B[30m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

enum {
	COLOUR_POSITIVE = 0,
	COLOUR_NEGATIVE,
	COLOUR_BEFORE,
	COLOUR_AFTER,
	COLOUR_DELETE
};

/*
#define COLOUR_POSITIVE GREEN
#define COLOUR_NEGATIVE CYAN

#define COLOUR_BEFORE GREEN
#define COLOUR_AFTER  YELLOW
#define COLOUR_DELETE RED
*/

/* maximum string length */
#define STRING_BLOCK       256
#define STRING_BLOCK_LONG  (STRING_BLOCK*10)

#define ADJUSTMENT_TYPE_BEFORE 'B'
#define ADJUSTMENT_TYPE_AFTER  'A'
#define ADJUSTMENT_TYPE_DELETE 'D'

#define YEAR_BASELINE 1900

#define MAX_PIECHART_FIELDS 100

#endif
