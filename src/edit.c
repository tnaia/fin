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

#include "undo.h"

int command_edit(char * account,
                 int record_number,
                 char * date, char * timestr,
                 char * date_format,
                 char * status, char * description,
                 char * currency, char *VATrate,
                 float progress, char * supplier,
                 char * receive, char * spend)
{
  /*char prev_date_std[STRING_BLOCK];*/
  char prev_date[STRING_BLOCK];
  char new_date[STRING_BLOCK];

  char prev_timestr[STRING_BLOCK];
  char new_timestr[STRING_BLOCK];

  char prev_status[STRING_BLOCK];
  char new_status[STRING_BLOCK];

  char prev_description[STRING_BLOCK];
  char new_description[STRING_BLOCK];

  char prev_currency[STRING_BLOCK];
  char new_currency[STRING_BLOCK];

  char prev_VATrate[STRING_BLOCK];
  char new_VATrate[STRING_BLOCK];

  char prev_progress[STRING_BLOCK];
  char new_progress[STRING_BLOCK];

  char prev_supplier[STRING_BLOCK];
  char new_supplier[STRING_BLOCK];

  char prev_receive[STRING_BLOCK];
  char new_receive[STRING_BLOCK];

  char prev_spend[STRING_BLOCK];
  char new_spend[STRING_BLOCK];

  char transaction_identifier[STRING_BLOCK];
  int changed = 0;

  if (database_get_record(account,record_number,
                          (char*)prev_date, (char*)prev_timestr,
                          (char*)transaction_identifier,
                          (char*)prev_status,(char*)prev_description,
                          (char*)prev_currency,(char*)prev_VATrate,
                          (char*)prev_progress,
                          (char*)prev_supplier,
                          (char*)prev_receive,
                          (char*)prev_spend)==0) {
    printf("Failed to get transaction\n");
    return 0;
  }

  sprintf(new_date,"%s",prev_date);
  sprintf(new_timestr,"%s",prev_timestr);
  sprintf(new_status,"%s",prev_status);
  sprintf(new_description,"%s",prev_description);
  sprintf(new_currency,"%s",prev_currency);
  sprintf(new_VATrate,"%s",prev_VATrate);
  sprintf(new_progress,"%s",prev_progress);
  sprintf(new_supplier,"%s",prev_supplier);
  sprintf(new_receive,"%s",prev_receive);
  sprintf(new_spend,"%s",prev_spend);
        
  if (date!=0) {
    if (strlen(date)!=0) {                      
      convert_date_to_standard(date, date_format,
                               new_date);
      changed = 1;
    }
  }
  if (timestr!=0) {
    if (strlen(timestr)!=0) {
      sprintf((char*)new_timestr,"%s",timestr);
      changed = 1;
    }
  }
  if (status!=0) {
    if (strlen(status)!=0) {
      sprintf((char*)new_status,"%s",status);
      changed = 1;
    }
  }
  if (description!=0) {
    if (strlen(description)!=0) {
      sprintf((char*)new_description,"%s",description);
      changed = 1;
    }
  }
  if (currency!=0) {
    if (strlen(currency)!=0) {
      sprintf((char*)new_currency,"%s",currency);
      changed = 1;
    }
  }
  if (VATrate!=0) {
    if (strlen(VATrate)!=0) {
      sprintf((char*)new_VATrate,"%s",VATrate);
      changed = 1;
    }
  }
  if (supplier!=0) {
    if (strlen(supplier)!=0) {
      sprintf((char*)new_supplier,"%s",supplier);
      changed = 1;
    }
  }
  if (receive!=0) {
    if (strlen(receive)!=0) {
      sprintf((char*)new_receive,"%s",receive);
      changed = 1;
    }
  }
  if (spend!=0) {
    if (strlen(spend)!=0) {
      sprintf((char*)new_spend,"%s",spend);
      changed = 1;
    }
  }
  if (progress>0) {
    sprintf((char*)new_progress,"%f",progress);
    changed = 1;
  }
  else {
    sprintf(new_progress,"%s","0");
  }

  /* if no fields were changed */
  if (changed==0) {
    return -1;
  }

  return database_edit_record(account,record_number,
                              (char*)prev_date,
                              (char*)prev_timestr,
                              (char*)prev_status,
                              (char*)prev_description,
                              (char*)prev_currency,
                              (char*)prev_VATrate,
                              atof(prev_progress),
                              (char*)prev_supplier,
                              (char*)prev_receive,
                              (char*)prev_spend,
                              (char*)new_date,
                              (char*)new_timestr,
                              (char*)new_status,
                              (char*)new_description,
                              (char*)new_currency,
                              (char*)new_VATrate,
                              atof(new_progress),
                              (char*)new_supplier,
                              (char*)new_receive,
                              (char*)new_spend,
                              date_format);
}
