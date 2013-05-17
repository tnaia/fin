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

#include "spend.h"

int command_spend(char * value, int recur_period, char * quantity,
				  char * date, char * timestr,
				  char * date_format, char * account,
				  char * description, char * currency, char * status,
				  char * invoice_number, char * sales_number,
				  char * purchase_number, char * works_number,
				  char * job_number, char * serial_number,
				  char * part_number, char * tracking_number,
				  int progress, char * latitude, char * longitude,
				  char * altitude,
				  char * supplier, char * customer, char * VATrate)
{
	char curr[8];
	if (is_money(value, (char*)curr)!=0) {
		if (curr[0]!=0) {
			currency = (char*)curr;
		}
		return database_save_transaction(
		    account, date_format, quantity,
			description, value, "0",
			date, timestr,
			recur_period, currency, status,
			invoice_number, sales_number,
			purchase_number, works_number,
			job_number, serial_number,
			part_number, tracking_number,
			progress, latitude, longitude, altitude,
			supplier, customer, VATrate);
	}
	else {
		printf(get_text_from_identifier(MESSAGE_NOT_VALID_AMOUNT),value);
		printf("\n");
	}
	return 0;
}
