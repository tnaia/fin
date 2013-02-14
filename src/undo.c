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

#include "undo.h"

int command_undo(char * account, int record_number,
				 char * date, char * timestr,
				 char * date_format)
{
	char date2[STRING_BLOCK];
	char timestr2[STRING_BLOCK];
	char status[STRING_BLOCK];
	char status2[STRING_BLOCK];
	char description2[STRING_BLOCK];
	char currency2[STRING_BLOCK];
	char VATrate2[STRING_BLOCK];
	char progress2[STRING_BLOCK];
	char supplier2[STRING_BLOCK];
	char receive2[STRING_BLOCK];
	char spend2[STRING_BLOCK];
	char transaction_number[STRING_BLOCK];
	int i,length;

	if (database_get_record(account,record_number,
							(char*)transaction_number,
							(char*)date2,(char*)timestr2,
							(char*)status2,(char*)description2,
							(char*)currency2,(char*)VATrate2,
							(char*)progress2,(char*)supplier2,
							(char*)receive2,(char*)spend2)==0) {
		return 0;
	}

	/* mark as cancelled */
	sprintf((char*)status,"%s",
			get_text_from_identifier(CANCEL_TRANSACTION_STATUS));

	/* edit the existing record */
	if (database_edit_record(account,record_number,
							 (char*)date2,(char*)timestr2,
							 (char*)status2,
							 (char*)description2,(char*)currency2,
							 (char*)VATrate2,atof(progress2),
							 (char*)supplier2,(char*)receive2,
							 (char*)spend2,
							 (char*)date2,(char*)timestr2,
							 (char*)status,
							 (char*)description2,(char*)currency2,
							 (char*)VATrate2,atof(progress2),
							 (char*)supplier2,(char*)receive2,
							 (char*)spend2,
							 date_format)==0) {
		return 0;
	}

	sprintf((char*)description2,
			get_text_from_identifier(CANCEL_TRANSACTION),
			transaction_number);

	if (atof(receive2)>0) {
		length = strlen(receive2);
		for (i=length;i>=1;i--) {
			receive2[i] = receive2[i-1];
		}
		receive2[0]='-';
		receive2[length+1]=0;
	}
	if (atof(spend2)>0) {
		length = strlen(spend2);
		for (i=length;i>=1;i--) {
			spend2[i] = spend2[i-1];
		}
		spend2[0]='-';
		spend2[length+1]=0;
	}

	/* add a balancing record */
	return database_save_short(account, date, timestr,
							   date_format, status,
							   description2, currency2, VATrate2,
							   atof(progress2), supplier2,
							   receive2, spend2);
}
