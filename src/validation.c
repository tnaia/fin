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

#include "validation.h"

int is_integer(char * str)
{
	int i;
	if (str==0) return 0;
	for (i=0;i<strlen(str);i++) {
		if ((str[i]<'0') || (str[i]>'9')) {
			return 0;
		}
	}
	return 1;
}

int is_value(char * str)
{
	int i;
	int decimals=0;
	if (str==0) return 0;
	if (strlen(str)==0) return 0;
	for (i=0;i<strlen(str);i++) {
		if ((str[i]<'0') || (str[i]>'9')) {
			if ((str[i]=='.') || (str[i]==',')) {
				decimals++;
				if (decimals > 1) return 0;
			}
			else {
				if (!((i==0) && (str[i]=='-'))) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int is_money(char * str, char * curr)
{
	int i;
	int decimals=0;
	char str2[10];

	curr[0]=0;

	if (str==0) return 0;
	if (strlen(str)==0) return 0;
	for (i=0;i<strlen(str);i++) {
		if ((str[i]<'0') || (str[i]>'9')) {
			if ((str[i]=='.') || (str[i]==',')) {
				decimals++;
				if (decimals > 1) return 0;
			}
			else {
				if (i>0) {
					return 0;
				}
				else {
					sprintf(str2,"%s","£");
					if (strncmp(str,str2,strlen(str2))==0) {
						sprintf(curr,"%s","GBP");
						i = strlen(str2)-1;
					}
					sprintf(str2,"%s","$");
					if (strncmp(str,str2,strlen(str2))==0) {
						sprintf(curr,"%s","USD");
						i = strlen(str2)-1;
					}
					sprintf(str2,"%s","€");
					if (strncmp(str,str2,strlen(str2))==0) {
						sprintf(curr,"%s","EUR");
						i = strlen(str2)-1;
					}
				}
			}
		}
	}
	return 1;
}

int is_date(char * str, char * date_format)
{
	int i,ctr=0,separators=0;
	char numstr[5];

	if (strlen(str)!=10) {
		return 0;
	}

	for (i=0;i<strlen(str);i++) {
		if ((str[i]>='0') && (str[i]<='9')) {
			ctr++;
		}
		else {
			if (!((str[i]=='/') || (str[i]=='-'))) {
				return 0;
			}
			else {
				separators++;
			}
		}
	}
	if ((ctr!=8) || (separators!=2)) {
		return 0;
	}

	if (strcmp(date_format,"%d/%m/%Y")==0) {
		numstr[0] = str[0];
		numstr[1] = str[1];
		numstr[2] = 0;
		if ((atoi(numstr)<1) || (atoi(numstr)>31)) {
			return 0;
		}
		numstr[0] = str[3];
		numstr[1] = str[4];
		numstr[2] = 0;
		if ((atoi(numstr)<1) || (atoi(numstr)>12)) {
			return 0;
		}
		numstr[0] = str[6];
		numstr[1] = str[7];
		numstr[2] = str[8];
		numstr[3] = str[9];
		numstr[4] = 0;
		if ((atoi(numstr)<1000) || (atoi(numstr)>3000)) {
			return 0;
		}
	}

	if (strcmp(date_format,"%m/%d/%Y")==0) {
		numstr[0] = str[0];
		numstr[1] = str[1];
		numstr[2] = 0;
		if ((atoi(numstr)<1) || (atoi(numstr)>12)) {
			return 0;
		}
		numstr[0] = str[3];
		numstr[1] = str[4];
		numstr[2] = 0;
		if ((atoi(numstr)<1) || (atoi(numstr)>31)) {
			return 0;
		}
		numstr[0] = str[6];
		numstr[1] = str[7];
		numstr[2] = str[8];
		numstr[3] = str[9];
		numstr[4] = 0;
		if ((atoi(numstr)<1000) || (atoi(numstr)>3000)) {
			return 0;
		}
	}

	return 1;
}

void remove_currency_symbol(char * value)
{
	int i,j;
	char result[100];

	if (strlen(value)>0) {
		if (!(((value[0]>='0') && (value[0]<='9')) ||
			  (value[0]==',') || (value[0]=='.'))) {
			j=0;
			for (i=1;i<strlen(value);i++,j++) {
				result[j] = value[i];
			}
			result[j]=0;

			sprintf(value,"%s",result);
		}
	}
}

int valid_language(char * language)
{
	int i;
	char * langs[] = {
		"english uk",
		"english us",
		"french",
		"german",
		"italian",
		"spanish",
		"greek",
		"russian",
		"polish",
		"dutch",
		"chinese"
	};
  
	for (i=0;i<11;i++) {
		if (strcmp(language,langs[i])==0) {
			return i+1;
		}
	}
	return 0;
}
