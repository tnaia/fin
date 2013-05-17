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

#ifndef SPEND_H
#define SPEND_H

#include "parse.h"
#include "database.h"
#include "validation.h"

int command_spend(char * value, int recur_period, char * quantity,
				  char * date, char * timestr,
				  char * date_format, char * account, 
				  char * description, char * currency, char * status,
				  char * invoice_number, char * sales_number,
				  char * purchase_number, char * works_number,
				  char * job_number, char * serial_number,
				  char * part_number, char * tracking_number,
				  int progress, char * latitude,
				  char * longitude, char * altitude,
				  char * supplier, char * customer, char * VATrate);

#endif
