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

#include "transfer.h"

void get_transfer_identifier(char * value, char * transfer_identifier)
{
    int i,j;
    char str[16];
    char nums[128];
    time_t now;
    struct tm *d;

    j=0;
    for (i = 0; i < strlen(value); i++)
        if ((value[i] >= '0') && (value[i] <= '9'))
            nums[j++] = value[i];

    nums[j] = 0;
    time(&now);
    d = localtime(&now);
    strftime(str, 16, "%Y%m%d%H%M%S", d);

    sprintf(transfer_identifier,"%s%s",str,nums);
}

int transfer(char * from_account, char * to_account,
             char * value, char * date, char * timestr,
             char * date_format,
             char * currency, char * VATrate)
{
    int retval=0;
    char description[STRING_BLOCK];
    char transfer_identifier[STRING_BLOCK];

    if (is_value(value) == 0) {
        printf("%s is not a value\n",value);
        return 0;
    }

    if ((from_account == 0) || (to_account == 0)) {
        if (from_account == 0)
            printf("%s","From account not specified\n");
        else
            printf("%s","To account not specified\n");

        return 0;
    }

    if (strcmp(from_account, to_account)==0) {
        printf("Account names are the same (%s)\n",from_account);
        return 0;
    }

    if ((strlen(from_account) == 0) || (strlen(to_account) == 0)) {
        if (strlen(from_account) == 0)
            printf("%s","From account not specified\n");
        else
            printf("%s","To account not specified\n");

        return 0;
    }

    get_transfer_identifier(value, transfer_identifier);

    sprintf((char*)description,
            get_text_from_identifier(TRANSFER_TO),
            transfer_identifier,to_account);
    retval = database_save_short(from_account,
                                 date, timestr,
                                 date_format,
                                 "", description,
                                 currency, VATrate,
                                 0, "", "0", value);
    if (retval!=0) {
        sprintf((char*)description,
                get_text_from_identifier(TRANSFER_FROM),
                transfer_identifier,from_account);
        retval = database_save_short(to_account,
                                     date, timestr,
                                     date_format,
                                     "", description,
                                     currency, VATrate,
                                     0, "", value, "0");
    }

    return retval;
}
