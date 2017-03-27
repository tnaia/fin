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

#include "conversions.h"

static void value_to_integer(char * money, char * integer,
                             int decimal_places)
{
    int i,ctr=0;
    char intval[STRING_BLOCK];
    char decval[STRING_BLOCK];

    for (i=0;i<strlen(money);i++) {
        if (money[i]=='.')
            break;

        if ((money[i]>='0') && (money[i]<='9'))
            intval[ctr++]=money[i];
    }
    intval[ctr]=0;
    if (i==strlen(money)) {
        ctr=0;
        for (i=0;i<strlen(money);i++) {
            if (money[i]==',')
                break;

            if ((money[i]>='0') && (money[i]<='9'))
                intval[ctr++]=money[i];
        }
        intval[ctr]=0;
    }

    i++;
    ctr=0;
    while (i<strlen(money)) {
        if ((money[i]>='0') && (money[i]<='9'))
            decval[ctr++]=money[i];

        i++;
    }
    decval[ctr]=0;

    ctr=0;
    for (i=0;i<strlen(intval);i++)
        integer[ctr++] = intval[i];

    for (i=0;i<strlen(decval);i++)
        integer[ctr++] = decval[i];

    for (i=0;i<decimal_places-strlen(decval);i++)
        integer[ctr++] = '0';

    integer[ctr]=0;
}

static void integer_to_value(char * integer, char * money,
                             int decimal_places)
{
    int i,ctr=0;

    for (i=0;i<strlen(integer)-decimal_places;i++)
        money[ctr++]=integer[i];

    money[ctr++]='.';
    while (i<strlen(integer)) {
        money[ctr++]=integer[i];
        i++;
    }
    money[ctr]=0;
}

void money_to_integer(char * money, char * integer)
{
    value_to_integer(money,integer, DECIMAL_PLACES);
}

void integer_to_money(char * integer, char * money)
{
    integer_to_value(integer, money, DECIMAL_PLACES);
}

void quantity_to_integer(char * money, char * integer)
{
    value_to_integer(money,integer, DECIMAL_PLACES_QUANTITY);
}

void integer_to_quantity(char * integer, char * money)
{
    integer_to_value(integer, money, DECIMAL_PLACES_QUANTITY);
}

void pad_text(char * str, int width)
{
    int i = 0;

    for (i=strlen(str);i<width;i++)
        str[i]=' ';

    str[i]=0;
}

void pad_value(char * value, char * padded,
               int leading_spaces, int trailing_zeros)
{
    int i,decimal_point=-1,zeros,leading;
    char temp[32];

    if (value == 0)
        return;

    sprintf(temp,"%s", value);

    for (i = 0; i < strlen(value); i++)
        if (value[i] == '.')
            decimal_point = i;

    if (decimal_point == -1)
        for (i = 0; i < strlen(value); i++) {
            if (value[i] == ',') {
                decimal_point = i;
            }
        }

    if (decimal_point > -1) {
        zeros = trailing_zeros+1 - (strlen(value) - decimal_point);
        if (zeros < 0)
            temp[strlen(value) + zeros] = 0;

        for (i = 0; i < zeros; i++)
            temp[strlen(value) + i] = '0';

        temp[strlen(value) + i] = 0;
    }
    else {
        temp[strlen(value)] = '.';
        for (i = 0; i < trailing_zeros; i++)
            temp[strlen(value) + i + 1]='0';

        temp[strlen(value) + i + 1]=0;
    }

    if (decimal_point == -1)
        decimal_point = strlen(value);

    leading = leading_spaces - decimal_point - 1;
    for (i = 0; i < leading; i++)
        padded[i] = ' ';

    for (i = 0; i < strlen(temp); i++)
        padded[leading + i] = temp[i];

    padded[leading + i] = 0;
}

/* convert a time string into a standard form */
void convert_time_to_standard(char * timestr, char * converted,
                              char * am, char * pm)
{
    int i, ctr = 0, state = 0;
    int hr = 0, min = 0, sec = 0;
    char str[STRING_BLOCK];

    /* handle empty string */
    if (timestr[0] == 0) {
        converted[0]=0;
        return;
    }

    /* convert to lower case */
    for (i = 0; i < strlen(timestr); i++)
        timestr[i] = tolower(timestr[i]);

    for (i = 0; i < strlen(timestr); i++) {
        if ((timestr[i]==am[0]) ||
            (timestr[i]==pm[0])) {
            str[ctr]=0;
            switch(state) {
            case 0: {
                hr = atoi(str);
                if (timestr[i]==pm[0])
                    if (hr < 12) hr += 12;

                break;
            }
            case 1: { min = atoi(str); break; }
            case 2: { sec = atoi(str); break; }
            }
            break;
        }
        if ((timestr[i]==':') ||
            (timestr[i]=='.') || (i == strlen(timestr)-1)) {
            if (i == strlen(timestr)-1)
                str[ctr++] = timestr[i];

            str[ctr]=0;
            switch(state) {
            case 0: { hr = atoi(str); break; }
            case 1: { min = atoi(str); break; }
            case 2: { sec = atoi(str); break; }
            }
            ctr = 0;
            state++;
        }
        else
            str[ctr++] = timestr[i];
    }

    sprintf(converted,"%02d:%02d:%02d",hr,min,sec);
}

/* convert a date to a standard format for storage in the database */
void convert_date_to_standard(char * date, char * date_format,
                              char * converted)
{
    int i,month=1,day=1,year=0;
    char str[10];

    for (i=0;i<strlen(date_format)-1;i++) {
        if (date_format[i]=='%') {
            if (date_format[i+1]=='m') {
                str[0]=date[i];
                str[1]=date[i+1];
                str[2]=0;
                month = atoi((char*)str);
            }
            if (date_format[i+1]=='d') {
                str[0]=date[i];
                str[1]=date[i+1];
                str[2]=0;
                day = atoi((char*)str);
            }
            if (date_format[i+1]=='Y') {
                str[0]=date[i];
                str[1]=date[i+1];
                str[2]=date[i+2];
                str[3]=date[i+3];
                str[4]=0;
                year = atoi((char*)str);
            }
        }
    }
    sprintf(converted,"%04d-%02d-%02d",year,month,day);
}

/* convert a date from the database into the desired date format */
void convert_standard_to_date(char * standard, char * date_format,
                              char * date)
{
    char str[5];
    int day,month,year,i;

    str[0] = standard[0];
    str[1] = standard[1];
    str[2] = standard[2];
    str[3] = standard[3];
    str[4] = 0;
    year = atoi((char*)str);

    str[0] = standard[5];
    str[1] = standard[6];
    str[2] = 0;
    month = atoi((char*)str);

    str[0] = standard[8];
    str[1] = standard[9];
    str[2] = 0;
    day = atoi((char*)str);

    sprintf(date,"%s",date_format);

    for (i=0;i<strlen(date_format)-1;i++) {
        if (date_format[i]=='%') {
            if (date_format[i+1]=='m')
                sprintf((char*)&date[i],"%02d",month);

            if (date_format[i+1]=='d')
                sprintf((char*)&date[i],"%02d",day);

            if (date_format[i+1]=='Y')
                sprintf((char*)&date[i],"%04d",year);
        }
    }
    for (i=0;i<strlen(date_format);i++) {
        if ((date_format[i]=='/') ||
            (date_format[i]=='-') ||
            (date_format[i]=='.'))
            date[i] = date_format[i];
    }
    date[10]=0;
}

int reverse_file(char * filename)
{
    FILE * fp;
    int i,row,rows=0,retval=0;
    char line[STRING_BLOCK];
    char header[STRING_BLOCK];
    char * output_filename = "temp_reverse.txt";

    /* count the number of rows */
    fp = fopen(filename,"r");
    if (fp!=0) {
        while (!feof(fp)) {
            if (fgets(line , 255 , fp) != NULL ) {
                if (rows==0)
                    sprintf((char*)header,"%s",line);

                rows++;
            }
        }
        fclose(fp);
    }

    fp = fopen(output_filename,"w");
    if (fp != 0) {
        fprintf(fp,"%s",header);
        fclose(fp);
    }

    for (i = rows; i > 1; i--) {
        row = 0;
        fp = fopen(filename,"r");
        if (fp!=0) {
            while ((!feof(fp)) && (row<i)) {
                if (fgets(line , 255 , fp) != NULL )
                    row++;
            }
            fclose(fp);
        }

        fp = fopen(output_filename,"a");
        if (fp!=0) {
            fprintf(fp,"%s",line);
            fclose(fp);
        }
    }

    if (rows > 0) {
        sprintf((char*)line,"rm %s", filename);
        retval = system((char*)line);
        sprintf((char*)line,"mv %s %s", output_filename, filename);
        retval = system((char*)line);
    }
    return retval;
}

/* trims the first line from a text file */
void trim_file_header(char * filename, char * trimmed)
{
    FILE * fp, * fp_trimmed;
    int row=0;
    char line[STRING_BLOCK];

    fp = fopen(filename,"r");
    if (fp != 0) {
        fp_trimmed = fopen(trimmed,"w");
        if (fp_trimmed != 0) {

            while (!feof(fp)) {
                if (fgets(line , 255 , fp) != NULL ) {
                    if (row > 0)
                        fprintf(fp_trimmed,"%s",line);

                    row++;
                }
            }

            fclose(fp_trimmed);
        }
        fclose(fp);
    }
}
