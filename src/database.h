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

#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "parse.h"
#include "settings.h"
#include "conversions.h"
#include "database.h"

void database_directory(char * directory);
void database_name(char * account, char * filename);
int create_sqlite_database();
int directory_exists(char * name);
int file_exists(char * name);
int account_exists(char * name);
int database_save_transaction(
    char * account, char * date_format, char * quantity,
	char * description, char * spend, char * receive,
	char * date, char * timestr, int recur,
	char * currency, char * status,
	char * invoice_number, char * sales_number,
	char * purchase_number, char * works_number,
	char * job_number, char * serial_number,
	char * part_number, char * tracking_number,
	float progress, char * latitude,
	char * longitude, char * altitude,
	char * supplier, char * customer, char * VATrate);
int get_balance(char * account, char ** currency, char ** total_spent,
				char ** total_received, char ** balance);
int clear_transaction_numbers();
int save_transaction_number(char * transaction_number);
int get_transaction_number_from_record_number(int record_number,
											  char * transaction_number);
int database_save_short(char * account, 
						char * date, char * timestr,
						char * date_format,
						char * status, char * description,
						char * currency, char *VATrate,
						float progress, char * supplier,
						char * receive, char * spend);

int database_save_adjustment(
    char * account,
	char * current_datetime,
    char adjustment_type,
	char * transaction_identifier,
	char * quantity, char * description,
	char * spend, char * receive,
	char * date, char * timestr, int recur,
	char * currency, char * status,
	char * invoice_number, char * sales_number,
	char * purchase_number, char * works_number,
	char * job_number, char * serial_number,
	char * part_number, char * tracking_number,
	float progress, char * latitude,
	char * longitude, char * altitude,
	char * supplier, char * customer, char * VATrate);

int database_save_adjustment_short(
    char * account,
	char * current_datetime,
	char adjustment_type,
	char * transaction_identifier,
	char * date, char * timestr,
	char * status, char * description,
	char * currency, char *VATrate,
	float progress, char * supplier,
	char * receive, char * spend);

int database_get_record(
						char * account,
						int record_number,
						char * date, char * timestr,
						char * transaction_number,
						char * status, char * description,
						char * currency, char *VATrate,
						char * progress, char * supplier,
						char * receive, char * spend);

int database_edit_record(
						 char * account,
						 int record_number,
						 char * prev_date, char * prev_timestr,
						 char * prev_status, char * prev_description,
						 char * prev_currency, char *prev_VATrate,
						 float prev_progress, char * prev_supplier,
						 char * prev_receive, char * prev_spend,
						 char * date, char * timestr,
						 char * status, char * description,
						 char * currency, char *VATrate,
						 float progress, char * supplier,
						 char * receive, char * spend,
						 char * date_format);

int database_delete_record(char * account, int record_number);

int database_transaction_exists(
								char * account,
								char * date, char * timestr,
								char * receive, char * spend,
								char * description,
								char * currency);

int get_annual_cashflow(char * account, char * currency,
						int year, int month,
						double * total_receive, double * total_spend);

int get_distribution(char * account, char * currency,
					 int start_year, int end_year,
					 int * distribution, double maximum,
					 int distribution_size,
					 char * search_string);
void show_distribution(int * distribution, int distribution_size,
					   double maximum,
					   char field_separator, char column_separator);
int get_month_totals(char * account, char * currency,
					 int year, int month,
					 double * month_receive, double * month_spend);
int transaction_volume(char * account, char * currency,
					   int monthly, int max_transactions,
					   char field_separator, char column_separator,
					   char * search_string, char * export_filename);
int create_sqlite_adjustments_table(char * account);
int get_totals(char * account, char * search_string,
			   int month_number, int year,
			   char * currency,
			   double * total_receive, double * total_spend);

#endif
