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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "tests.h"
#include "settings.h"
#include "international.h"
#include "undo.h"
#include "spend.h"
#include "receive.h"
#include "import.h"
#include "calendar.h"
#include "summary.h"
#include "monthly.h"
#include "yearly.h"
#include "search.h"
#include "transfer.h"
#include "edit.h"
#include "lock.h"
#include "piechart.h"
#include "narrator.h"
#include "help.h"

int main(int argc, char* argv[])
{
	char fieldname[MAX_ARGS*STRING_BLOCK];
	char value[MAX_ARGS*STRING_BLOCK];
	int no_of_fields,recur_period;
	int month_number,year,end_year;
	int account_specified,currency_specified;
	char separator = ':';
	char column_separator = ' ';
	char * result = 0;
	int progress=0;
	char date[STRING_BLOCK], timestr[STRING_BLOCK];
	char quantity[STRING_BLOCK];
	char * account = 0;
	char * description = 0;
	char str[STRING_BLOCK],titlestr[STRING_BLOCK];
	int retval, no_of_transactions = 5, no_of_transactions_monthly=12;
	int no_of_transactions_volume = 120;
	double inflation_rate=0, savings_interest_rate=0;
	double loans_interest_rate=0, balance=0;
	double maximum = 100;
	int monthly_flag = 0;

	char * currency = 0;
	char * status = 0;
	char * invoice_number = 0;
	char * sales_number = 0;
	char * purchase_number = 0;
	char * works_number = 0;
	char * job_number = 0;
	char * serial_number = 0;
	char * part_number = 0;
	char * tracking_number = 0;
	char * latitude = 0;
	char * longitude = 0;
	char * altitude = 0;
	char * supplier = 0;
	char * customer = 0;
	char * VATrate = 0;
	int VATrate_specified = 0;
	char * server = 0;
	char * date_format = 0;
	char * export_filename = 0;
	char * to_account = 0;
	char * language = 0;
	char * kml_filename = 0;
	char * image_filename = 0;
	char * title = 0;
	char * piechart_filename = 0;
	char * style = 0;
	char * speak = 0;
	char * distribution_filename = 0;
	char * volume_filename = 0;
	char * username = 0;
	char * backup_filename = 0;
	char * restore_filename = 0;

	char * srv;
	char command_str[STRING_BLOCK];
	char fin_directory[STRING_BLOCK];
	int command_run = 0;
	int date_format_specified;
	int language_specified;
	int timestr_specified = 0;
	int date_specified = 0;
	char field_separator = ',';
	time_t now;
	struct tm *d;
	int distribution[DISTRIBUTION_SIZE];

	if (unlock()!=0) {
		return 0;
	}

	timestr[0]=0;

	set_language(ENGLISH_UK);  

	sprintf((char*)fin_directory,"%s/.fin",getenv("HOME"));

	if (directory_exists((char*)fin_directory)==0) {
		create_sqlite_database();
		settings_set_language("English UK");
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		settings_set_currency(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY));
		settings_set_vat_rate(get_text_from_identifier(SETTINGS_DEFAULT_VAT_RATE));
		settings_set_date_format(get_text_from_identifier(SETTINGS_DEFAULT_DATE_FORMAT));
		settings_save((char*)fin_directory);
	}

	if (settings_load((char*)fin_directory)!=0) {
		account = settings_get_account();
		currency = settings_get_currency();
		VATrate = settings_get_vat_rate();
		server = settings_get_server();
		date_format = settings_get_date_format();
		language = settings_get_language();
		if (valid_language(language)>0) {
			set_language_from_string(language);
		}
	}

	if (argc<=1) {
		show_summary(account,no_of_transactions, column_separator);
		balance = get_current_balance(account, currency, 0);
		if (balance>=0) {
			narrate_spending(account, currency, loans_interest_rate,
							 savings_interest_rate, DEFAULT_INFLATION_RATE);
		}
		else {
			narrate_spending(account, currency, loans_interest_rate,
							 savings_interest_rate, DEFAULT_USURY_RATE);
		}
		narrate_relative_cashflow(account, currency);

		return 1;
	}

	/* parse the command line arguments */
	no_of_fields = parse(argc, argv, (char*)fieldname,
						 (char*)value, separator, 1, MAX_ARGS);

	if (get_help(no_of_fields, (char*)fieldname, (char*)value)!=0) {
		show_help();
		return 1;
	}

	if (get_version(no_of_fields, (char*)fieldname, (char*)value)!=0) {
		printf(get_text_from_identifier(SHOW_VERSION),VERSION_NUMBER);
		printf("\n");
		return 1;
	}

	/* Number of transactions to show in the summary */
	if (get_summary_transactions(no_of_fields,
								 (char*)fieldname, (char*)value)>0) {
		no_of_transactions =
			get_summary_transactions(no_of_fields,
									 (char*)fieldname, (char*)value);
		no_of_transactions_monthly = no_of_transactions;
		no_of_transactions_volume = no_of_transactions;
	}

	/* VAT rate */
	VATrate = get_VAT_rate(no_of_fields, (char*)fieldname, (char*)value);
	if (strlen(VATrate)==0) {
		VATrate = settings_get_vat_rate();
		if (strlen(VATrate)==0) {
			settings_set_vat_rate(get_text_from_identifier(SETTINGS_DEFAULT_VAT_RATE));
			VATrate = settings_get_vat_rate();
		}
	}
	else {
		VATrate_specified = 1;
	}

	/* Date format */
	date_format_specified=0;
	date_format =
		get_date_format(no_of_fields, (char*)fieldname, (char*)value);
	if (strlen(date_format)==0) {
		date_format = settings_get_date_format();
		if (strlen(date_format)==0) {
			settings_set_date_format(get_text_from_identifier(SETTINGS_DEFAULT_DATE_FORMAT));
			date_format = settings_get_date_format();
			if (strlen(date_format)>0) {
				date_format_specified=1;
			}
		}
	}
	else {
		date_format_specified=1;
	}

	/* period of re-occurance */
	recur_period =
		get_recur(no_of_fields, (char*)fieldname, (char*)value);

	/* get the quantity */
	sprintf((char*)quantity,"%d",1);
	if (strlen(get_quantity(no_of_fields,
							(char*)fieldname, (char*)value))>0) {
		sprintf((char*)quantity,"%s",
				get_quantity(no_of_fields,
							 (char*)fieldname, (char*)value));
	}

	/* get the date */
	time(&now);
	d = localtime(&now);
	strftime(date, 15, date_format, d);
	if (strlen(get_date(no_of_fields, (char*)fieldname, (char*)value))>0) {
		date_specified = 1;
		sprintf((char*)date,"%s",get_date(no_of_fields,
										  (char*)fieldname, (char*)value));
	}

	/* get the description */
	description = get_description(no_of_fields,
								  (char*)fieldname, (char*)value);

	/* get the account */
	account = settings_get_account();
	account_specified=0;
	if (get_account_name(no_of_fields,
						 (char*)fieldname, (char*)value)!=0) {
		account =
			get_account_name(no_of_fields, (char*)fieldname, (char*)value);
		if (account!=0) {
			if (strlen(account)>0) {
				account_specified=1;
			}
		}
	}

	/* display style */
	if (get_style(no_of_fields, (char*)fieldname, (char*)value)!=0) {
		style = get_style(no_of_fields, (char*)fieldname, (char*)value);
		if ((strcmp(style,get_text_from_identifier(STYLE_EMACS))==0) ||
			(strcmp(style,get_text_from_identifier(STYLE_ORG))==0) ||
			(strcmp(style,get_text_from_identifier(STYLE_ORGMODE))==0)) {
			column_separator = '|';
		}
	}

	/* currency */
	currency = settings_get_currency();
	currency_specified=0;
	if (strlen(get_currency(no_of_fields,
							(char*)fieldname, (char*)value))>0) {
		currency = get_currency(no_of_fields,
								(char*)fieldname, (char*)value);
		if (currency!=0) {
			if (strlen(currency)>0) {
				currency_specified=1;
			}
		}
	}

	/* set the language */
	language_specified=0;
	language = get_language(no_of_fields, (char*)fieldname, (char*)value);
	if (language!=0) {
		if (valid_language(language)>0) {
			language_specified=1;
		}
		else {
			printf("%s is not a valid language\n",language);
		}
	}

	sprintf(timestr,"%02d:%02d:%02d",d->tm_hour,d->tm_min,d->tm_sec);
	if (get_time(no_of_fields, (char*)fieldname, (char*)value)!=0) {
		timestr_specified = 1;
		sprintf(str,"%s",get_time(no_of_fields,
								  (char*)fieldname, (char*)value));
		/* convert the time given into a standard format */
		convert_time_to_standard(str, timestr,
								 get_text_from_identifier(TIME_AM),
								 get_text_from_identifier(TIME_PM));		
	}
	volume_filename = get_volume(no_of_fields,
								 (char*)fieldname, (char*)value);
	distribution_filename = get_distrib(no_of_fields,
										(char*)fieldname, (char*)value);
	if (get_maximum(no_of_fields, (char*)fieldname, (char*)value)!=0) {
		maximum = get_maximum(no_of_fields, (char*)fieldname, (char*)value);
	}
	inflation_rate = get_inflation_rate(no_of_fields,
										(char*)fieldname, (char*)value);
	savings_interest_rate = get_savings_interest_rate(no_of_fields,
													  (char*)fieldname,
													  (char*)value);
	loans_interest_rate = get_loans_interest_rate(no_of_fields,
												  (char*)fieldname,
												  (char*)value);
	status = get_status(no_of_fields, (char*)fieldname, (char*)value);
	invoice_number = get_invoice_number(no_of_fields,
										(char*)fieldname, (char*)value);
	sales_number = get_sale_number(no_of_fields,
								   (char*)fieldname, (char*)value);
	purchase_number = get_purchase_number(no_of_fields,
										  (char*)fieldname, (char*)value);
	works_number = get_works_number(no_of_fields,
									(char*)fieldname, (char*)value);
	job_number = get_job_number(no_of_fields,
								(char*)fieldname, (char*)value);
	serial_number = get_serial_number(no_of_fields,
									  (char*)fieldname, (char*)value);
	part_number = get_part_number(no_of_fields,
								  (char*)fieldname, (char*)value);
	tracking_number = get_tracking_number(no_of_fields,
										  (char*)fieldname, (char*)value);
	progress = get_progress(no_of_fields,
							(char*)fieldname, (char*)value);
	to_account = get_to(no_of_fields,
						(char*)fieldname, (char*)value);
	username = get_username(no_of_fields, (char*)fieldname, (char*)value);
	title = get_title(no_of_fields, (char*)fieldname, (char*)value);
	piechart_filename = get_piechart(no_of_fields,
									 (char*)fieldname, (char*)value);

	latitude = get_latitude(no_of_fields, (char*)fieldname, (char*)value);
	longitude = get_longitude(no_of_fields, (char*)fieldname, (char*)value);
	altitude = get_altitude(no_of_fields, (char*)fieldname, (char*)value);

	supplier = get_supplier(no_of_fields, (char*)fieldname, (char*)value);
	customer = get_customer(no_of_fields, (char*)fieldname, (char*)value);

	month_number = get_month(no_of_fields, (char*)fieldname, (char*)value);
	year = get_year(no_of_fields, (char*)fieldname, (char*)value);
	end_year = get_end_year(no_of_fields, (char*)fieldname, (char*)value);

	export_filename = get_export(no_of_fields,
								 (char*)fieldname, (char*)value);
	kml_filename = get_kml(no_of_fields, (char*)fieldname, (char*)value);
	field_separator = get_field_separator(no_of_fields,
										  (char*)fieldname, (char*)value);
	backup_filename =
		get_backup(no_of_fields, (char*)fieldname, (char*)value);


	/* restore */
	restore_filename =
		get_restore(no_of_fields, (char*)fieldname, (char*)value);
	if (restore_filename != 0) {
		if (strlen(restore_filename) > 0) {
			command_run = 1;
			if (database_restore(restore_filename) > -1) {

				printf(get_text_from_identifier(RESTORED_FROM_FILE),
					   restore_filename);
			}
			else {
				printf(get_text_from_identifier(FILE_NOT_FOUND),
					   restore_filename);
			}
		}
		else {
			printf("%s",get_text_from_identifier(NOT_RESTORED));
		}
		return 1;
	}

	/* speak */
	speak = get_speak(no_of_fields, (char*)fieldname, (char*)value);
	if (speak!=0) {
		if (strlen(speak)==0) {
			narrate_balance(account, currency);
			return 1;
		}
		if ((strcmp(speak,get_text_from_identifier(COMMAND_SELL))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_SOLD))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_INCOME))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_EARN))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_EARNED))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_EARNINGS))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_RECEIVE))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_RECEIVED))==0)) {
			narrate_income(account, currency);
			return 1;
		}
		if ((strcmp(speak,get_text_from_identifier(SPENDING))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_SPENT))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_SPEND))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_OUTGOING))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_PAID))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_PURCHASE))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_PURCHASED))==0) ||
			(strcmp(speak,get_text_from_identifier(COMMAND_BUY))==0)) {
			narrate_spending(account, currency, loans_interest_rate, 
							 savings_interest_rate, inflation_rate);
			return 1;
		}
		if (strcmp(speak,get_text_from_identifier(INFLATION_RATE))==0) {
			narrate_relative_cashflow(account, currency);
			return 1;
		}
		return 1;
	}

	/* undo */
	result = get_undo(no_of_fields, (char*)fieldname, (char*)value);
	if ((result!=0) && (command_run==0)) {
		command_run=1;

		if (database_delete_record(account, atoi(result))!=0) {
			/*if (command_undo(account, atoi(result), date, date_format)!=0) {*/
			printf(get_text_from_identifier(RECORD_UNDONE), result);
			printf("\n");
		}
		else {
			printf("Failed to undo %s\n", result);
		}
	}

	/* lock */
	result = get_lock(no_of_fields, (char*)fieldname, (char*)value);
	if ((result!=0) && (command_run==0) && (backup_filename == 0)) {
		command_run=1;
		lock();
		return 1;
	}

	/* backup */
	if (backup_filename != 0) {
		if (strlen(backup_filename) > 0) {
			command_run = 1;
			database_backup(backup_filename, result);
			printf(get_text_from_identifier(BACKUP_CREATED),
				   backup_filename);
		}
		else {
			printf("%s",get_text_from_identifier(NO_BACKUP));
		}
		return 1;
	}

	/* edit */
	result = get_edit(no_of_fields, (char*)fieldname, (char*)value);
	if ((result!=0) && (command_run==0)) {
		command_run=1;

		if (timestr_specified == 0) {
			timestr[0] = 0;
		}
		if (currency_specified == 0) {
			currency[0] = 0;
		}
		if (VATrate_specified == 0) {
			VATrate[0] = 0;
		}
		if (date_specified == 0) {
			date[0]=0;
		}

		retval = command_edit(account,atoi(result),
							  date, (char*)timestr, date_format,
							  status, description,
							  currency, VATrate,
							  progress, supplier,
							  0, 0);
		if (retval > 0) {
			printf(get_text_from_identifier(RECORD_EDITED), result);
			printf("\n");
		}
		else {
			if (retval!=-1) printf("Failed to edit %s\n", result);
		}
	}

	/* spending */
	if (command_run==0) {
		result = get_spend(no_of_fields, (char*)fieldname, (char*)value);
		if (result!=0) {
			command_run=1;
			if (command_spend(result, recur_period, quantity,
							  (char*)date, (char*)timestr,
							  date_format, account,
							  description, currency, status,
							  invoice_number,sales_number,
							  purchase_number,works_number,
							  job_number,serial_number,
							  part_number, tracking_number,
							  progress, latitude, longitude, altitude,
							  supplier, customer, VATrate)!=0) {
				settings_save((char*)fin_directory);
				sprintf((char*)str,"%s\t",
						get_text_from_identifier(MESSAGE_SPENT));
				printf(str,result);
				show_summary_balance(account);
			}
		}
	}

	/* search */
	if ((command_run==0) &&
		(distribution_filename==0) &&
		(volume_filename==0) &&
		(piechart_filename==0) &&
		(get_adjustments(no_of_fields, (char*)fieldname, (char*)value)==0)) {
		result = get_search(no_of_fields, (char*)fieldname, (char*)value);
		if (strlen(result)>0) {
			command_run=1;

			image_filename = get_monthly(no_of_fields,
										 (char*)fieldname, (char*)value);
			if (image_filename == 0) {
				search(account, result, month_number, year, 10000,
					   date_format, export_filename, field_separator,
					   kml_filename, column_separator);
			}
			else {
				search_monthly(account, result, year, 10000, date_format,
							   export_filename, field_separator,
							   image_filename, column_separator);
			}
		}
	}

	/* receive */
	if (command_run==0) {
		result = get_receive(no_of_fields, (char*)fieldname, (char*)value);
		if (result!=0) {
			command_run=1;
			if (command_receive(result, recur_period, quantity,
								(char*)date,(char*)timestr,
								date_format, account, description,
								currency, status,
								invoice_number,sales_number,
								purchase_number,works_number,
								job_number,serial_number,
								part_number, tracking_number,
								progress, latitude, longitude, altitude,
								supplier, customer, VATrate)!=0) {
				settings_save((char*)fin_directory);
				sprintf((char*)str,"%s\t",
						get_text_from_identifier(MESSAGE_RECEIVED));
				printf(str,result);
				show_summary_balance(account);
			}
		}
	}

	/* Change date format */
	if ((no_of_fields==1) &&
		(date_format_specified!=0) &&
		(command_run==0)) {
		command_run=1;
		settings_set_date_format(date_format);
		settings_save((char*)fin_directory);
		printf(get_text_from_identifier(DATE_FORMAT_CHANGED),date_format);
		printf("\n");
	}

	/* Change default account */
	if ((no_of_fields==1) &&
		(account_specified!=0) &&
		(command_run==0)) {
		command_run=1;
		settings_set_account(account);
		settings_save((char*)fin_directory);
		printf(get_text_from_identifier(ACCOUNT_CHANGED),account);
		printf("\n");
	}

	/* Change default currency */
	if ((no_of_fields==1) && (currency_specified!=0) && (command_run==0)) {
		command_run=1;
		settings_set_currency(currency);
		settings_save((char*)fin_directory);
		printf(get_text_from_identifier(CURRENCY_CHANGED),currency);
		printf("\n");
	}

	/* Change default language */
	if ((no_of_fields==1) && (language_specified!=0) && (command_run==0)) {
		command_run=1;
		set_language_from_string(language);
		settings_set_language(language);
		settings_set_vat_rate(get_text_from_identifier(SETTINGS_DEFAULT_VAT_RATE));
		settings_set_currency(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY));
		settings_set_date_format(get_text_from_identifier(SETTINGS_DEFAULT_DATE_FORMAT));
		settings_save((char*)fin_directory);
		printf(get_text_from_identifier(LANGUAGE_CHANGED),language);
		printf("\n");
	}

	/* push database to server */
	if (command_run==0) {
		srv = get_push(no_of_fields, (char*)fieldname, (char*)value);
		if (srv!=0) {
			command_run=1;
			if (strlen(srv)>0) {
				settings_set_server(srv);
				settings_save((char*)fin_directory);
				server = srv;
			}
			if (server!=0) {
				if (username != 0) {
					sprintf((char*)command_str,
							"scp -r -o User=%s %s %s",
							username, (char*)fin_directory, server);
				}
				else {
					sprintf((char*)command_str,"scp -r %s %s",
							(char*)fin_directory, server);
				}
				retval = system(command_str);
				if (retval) {
					printf("Command failed\n");
					printf("%s\n", command_str);
				}
			}
			else {
				printf("%s", get_text_from_identifier(WARNING_NO_SERVER));
				printf("\n");
			}
		}
	}

	/* pull database from server */
	if (command_run==0) {
		srv = get_pull(no_of_fields, (char*)fieldname, (char*)value);
		if (srv!=0) {
			command_run=1;
			if (strlen(srv)>0) {
				settings_set_server(srv);
				settings_save((char*)fin_directory);
				server = srv;
			}
			if (server!=0) {
				if (directory_exists((char*)fin_directory)==0) {
					sprintf((char*)command_str,
							"mkdir %s", (char*)fin_directory);
					retval = system(command_str);
				}
				if (username!=0) {
					/* use username */
					sprintf((char*)command_str,
							"scp -r -o User=%s %s/.fin %s",
							username, server, getenv("HOME"));
				}
				else {
					/* assume that the remote username is the same */
					sprintf((char*)command_str,"scp -r %s/.fin %s",
							server, getenv("HOME"));
				}
				retval = system(command_str);
				if (retval) {
					printf("Command failed\n");
					printf("%s\n", command_str);
				}
			}
			else {
				printf("%s",get_text_from_identifier(WARNING_NO_SERVER));
				printf("\n");
			}
		}
	}

	/* transfer */
	if (command_run==0) {
		result = get_transfer(no_of_fields, (char*)fieldname, (char*)value);
		if (strlen(result)>0) {
			command_run=1;
			if (transfer(account, to_account,
						 result, date, timestr, date_format,
						 currency, VATrate)!=0) {
				printf(get_text_from_identifier(TRANSFER_COMPLETE),
					   result,account,to_account);
				printf("\n");
			}
		}
	}

	/* import */
	if (command_run==0) {
		result = get_import(no_of_fields, (char*)fieldname, (char*)value);
		if (strlen(result)>0) {
			command_run=1;
			retval = command_import(account,result,field_separator);
			printf(get_text_from_identifier(TRANSACTIONS_IMPORTED),
				   retval,result);
			printf("\n");
		}
	}

	/* adjustments */
	if (command_run==0) {
		result = get_adjustments(no_of_fields, (char*)fieldname, (char*)value);
		if (result!=0) {
			command_run=1;
			result = get_search(no_of_fields, (char*)fieldname, (char*)value);
			printf("%s\n",get_text_from_identifier(RECENT_EDITS));
			show_recent_adjustments(account, no_of_transactions,
									column_separator, result,
									year, month_number);
		}
	}

	/* pie chart */
	if ((command_run==0) &&
		(distribution_filename == 0) &&
		(volume_filename == 0)) {
		result = get_search(no_of_fields, (char*)fieldname, (char*)value);
		if (strlen(result)>0) {
			command_run=1;
			if ((strlen(piechart_filename)==0) ||
				((strlen(piechart_filename)>0) &&
				 ((strstr(piechart_filename,".csv")!=0) ||
				  (strstr(piechart_filename,".txt")!=0)))) {
				retval = piechart(title, account, result, month_number,
								  year, field_separator,
								  column_separator, currency,
								  1, piechart_filename);
			}
			else {
				plot_piechart(account,
							  result, year, month_number,
							  field_separator, currency,
							  1, title, 512, 512,
							  piechart_filename);
			}
		}
	}

	/* transaction volumes */
	if ((command_run==0) && (volume_filename != 0)) {
		result = get_search(no_of_fields, (char*)fieldname, (char*)value);
		monthly_flag = 0;
		if (get_monthly(no_of_fields,
						(char*)fieldname, (char*)value)!=0) {
			monthly_flag = 1;
		}

		if (strlen(volume_filename)>0) {
			sprintf(str,"%s","gnuplot3.csv");
			/*field_separator = ' ';*/
		}
		else {
			if (export_filename!=0) {
				sprintf(str,"%s",export_filename);
			}
			else {
				str[0]=0;
			}
		}
		if (transaction_volume(account, currency, year,
							   monthly_flag, no_of_transactions_volume,
							   field_separator, column_separator,
							   result, str)!=0) {
			if (strlen(volume_filename)>0) {

				if (monthly_flag==0) {
					/* yearly */
					sprintf(titlestr,"%s",
							get_text_from_identifier(TITLE_YEARLY_VOLUMES));
				}
				else {
					/* monthly */
					sprintf(titlestr,"%s",
							get_text_from_identifier(TITLE_MONTHLY_VOLUMES));
				}

				/* subtitle with the name of the account */
				sprintf(str,
						get_text_from_identifier(TITLE_ACCOUNT),
						account);
				str[0] = toupper(str[0]); /* capital first letter */

				/* if a custom title has been specified */
				if (title!=0) {
					sprintf(titlestr,"%s",title);
				}

				plot_volume(titlestr, str,
							"gnuplot3.csv",
							1024, 480,
							volume_filename);

				printf(get_text_from_identifier(EXPORT_IMAGE),
					   volume_filename);
				printf("\n");
			}
		}
		command_run=1;
	}

	/* spending distribution */	
	if ((command_run==0) &&
		(distribution_filename != 0)) {
		if ((year==0) && (end_year==0)) {
			/* use this year */
			year = d->tm_year+YEAR_BASELINE;
			end_year = year;
		}
		if ((year==0) && (end_year>0)) {
			year = end_year;
		}
		if ((year>0) && (end_year==0)) {
			end_year = year;
		}

		/* get any search criteria */
		result = get_search(no_of_fields,
							(char*)fieldname,
							(char*)value);

		/* get the distribution in an array */
		if (get_distribution(account, currency,
							 year, end_year,
							 distribution, maximum,
							 DISTRIBUTION_SIZE, result)==0) {
			return 0;
		}

		if (strlen(value) > 0) {
			command_run=1;

			if (year == end_year) {
				/* single year */
				sprintf(titlestr,
						get_text_from_identifier(TITLE_DISTRIBUTION),
						year);
			}
			else {
				/* range of years */
				sprintf(titlestr,
						get_text_from_identifier(TITLE_DISTRIBUTION_YEARS),
						year, end_year);
			}
			/* subtitle with the name of the account */
			sprintf(str,
					get_text_from_identifier(TITLE_ACCOUNT),
					account);
			str[0] = toupper(str[0]); /* capital first letter */

			/* if a custom title has been specified */
			if (title!=0) {
				sprintf(titlestr,"%s",title);
			}

			/* create the plot image */
			plot_distribution(titlestr, str,
							  1024,480,
							  value,
							  distribution,
							  maximum,
							  DISTRIBUTION_SIZE);

			printf(get_text_from_identifier(EXPORT_IMAGE),
				   distribution_filename);
			printf("\n");
		}
		if (command_run==0) {
			command_run=1;
			show_distribution(distribution, DISTRIBUTION_SIZE,
							  maximum,
							  field_separator, column_separator);
		}
	}

	/* run unit tests */
	if (strcmp(argv[1],"tests")==0) {
		return run_tests();
	}

	if (command_run==0) {
		image_filename = get_monthly(no_of_fields,
									 (char*)fieldname, (char*)value);
		if (image_filename != 0) {
			return show_monthly_summary(account, no_of_transactions_monthly,
										year, export_filename,
										field_separator,
										currency, image_filename,
										column_separator);
		}
		else {
			if (get_yearly(no_of_fields,
						   (char*)fieldname, (char*)value)!=0) {
				return show_yearly_summary(account, no_of_transactions,
										   export_filename, field_separator,
										   currency, column_separator);
			}
			else {
				return show_summary(account,
									no_of_transactions,
									column_separator);
			}
		}
	}

	return 1;
}

