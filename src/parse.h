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

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "globals.h"
#include "validation.h"
#include "international.h"

void trim(char * str);

int parse(int argc, char * argv[],
		  char * fieldname, char * value,
		  char separator,
		  int start_index, int max_results);

void remove_eol(char * str);

char * get_field_string(char * name,
						int no_of_fields, char * fieldname, char * value);

int get_field_value(char * name,
					int no_of_fields, char * fieldname, char * value);

char * get_field_string_from_identifier(int identifier,
										int no_of_fields,
										char * fieldname, char * value);

int get_field_value_from_identifier(int identifier,
									int no_of_fields,
									char * fieldname, char * value);

char * get_field(int identifier, int no_of_fields,
				 char* fieldname, char* value);

char * get_summary(int no_of_fields, char* fieldname, char* value);
char * get_account_name(int no_of_fields, char* fieldname, char* value);
char * get_invoice_number(int no_of_fields, char* fieldname, char* value);
char * get_sale_number(int no_of_fields, char* fieldname, char* value);
char * get_purchase_number(int no_of_fields, char* fieldname, char* value);
char * get_works_number(int no_of_fields, char* fieldname, char* value);
char * get_job_number(int no_of_fields, char* fieldname, char* value);
char * get_serial_number(int no_of_fields, char* fieldname, char* value);
char * get_part_number(int no_of_fields, char* fieldname, char* value);
char * get_description(int no_of_fields, char* fieldname, char* value);
char * get_date(int no_of_fields, char* fieldname, char* value);
char * get_currency(int no_of_fields, char* fieldname, char* value);
char * get_status(int no_of_fields, char* fieldname, char* value);
char * get_supplier(int no_of_fields, char* fieldname, char* value);
char * get_customer(int no_of_fields, char* fieldname, char* value);

char * get_spend(int no_of_fields, char* fieldname, char* value);
char * get_receive(int no_of_fields, char* fieldname, char* value);
char * get_import(int no_of_fields, char* fieldname, char* value);
char * get_export(int no_of_fields, char* fieldname, char* value);
char * get_report(int no_of_fields, char* fieldname, char* value);
char * get_list(int no_of_fields, char* fieldname, char* value);
char * get_calendar(int no_of_fields, char* fieldname, char* value);
char * get_tests(int no_of_fields, char* fieldname, char* value);
char * get_tracking_number(int no_of_fields, char* fieldname, char* value);
int get_progress(int no_of_fields, char* fieldname, char* value);
int get_summary_transactions(int no_of_fields,
							 char* fieldname, char* value);
char * get_VAT_rate(int no_of_fields, char* fieldname, char* value);
char * get_search(int no_of_fields, char* fieldname, char* value);
char * get_transfer(int no_of_fields, char* fieldname, char* value);
char * get_to(int no_of_fields, char* fieldname, char* value);
char * get_language(int no_of_fields, char* fieldname, char* value);
char * get_kml(int no_of_fields, char* fieldname, char* value);
char * get_help(int no_of_fields, char* fieldname, char* value);
char * get_version(int no_of_fields, char* fieldname, char* value);
char * get_lock(int no_of_fields, char* fieldname, char* value);
char * get_title(int no_of_fields, char* fieldname, char* value);
char * get_username(int no_of_fields, char* fieldname, char* value);
char * get_piechart(int no_of_fields, char* fieldname, char* value);
char * get_speak(int no_of_fields, char* fieldname, char* value);
double get_savings_interest_rate(int no_of_fields,
								 char* fieldname, char* value);
double get_loans_interest_rate(int no_of_fields,
							   char* fieldname, char* value);
double get_inflation_rate(int no_of_fields,
						  char* fieldname, char* value);

int get_recur(int no_of_fields, char* fieldname, char* value);

char * get_quantity(int no_of_fields, char* fieldname, char* value);
char * get_undo(int no_of_fields, char* fieldname, char* value);
char * get_edit(int no_of_fields, char* fieldname, char* value);

char * get_latitude(int no_of_fields, char* fieldname, char* value);
char * get_longitude(int no_of_fields, char* fieldname, char* value);
char * get_altitude(int no_of_fields, char* fieldname, char* value);

int get_month(int no_of_fields, char* fieldname, char* value);
int get_year(int no_of_fields, char* fieldname, char* value);
int get_end_year(int no_of_fields, char* fieldname, char* value);

char * get_pull(int no_of_fields, char* fieldname, char* value);
char * get_push(int no_of_fields, char* fieldname, char* value);

char * get_style(int no_of_fields, char* fieldname, char* value);

char * get_date_format(int no_of_fields,
					   char* fieldname, char* value);
char get_field_separator(int no_of_fields,
						 char* fieldname, char* value);
char * get_monthly(int no_of_fields, char* fieldname, char* value);
char * get_yearly(int no_of_fields, char* fieldname, char* value);
void description_multiline(char * str,
						   char column_separator,
						   int start_column);
char * get_distrib(int no_of_fields, char* fieldname, char* value);
double get_maximum(int no_of_fields, char* fieldname, char* value);
void search_string_to_sql(char * search_string, char * result,
						  int year, int month_number);
char * get_volume(int no_of_fields, char* fieldname, char* value);
char * get_time(int no_of_fields, char* fieldname, char* value);
char * get_adjustments(int no_of_fields, char* fieldname, char* value);
char * get_backup(int no_of_fields, char* fieldname, char* value);
char * get_restore(int no_of_fields, char* fieldname, char* value);

#endif
