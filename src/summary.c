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

#include "summary.h"

double get_current_balance(char * account, char * currency, int year)
{
	int retval,col,cols;
	char database_filename[STRING_BLOCK];
	char query[STRING_BLOCK_LONG];
	double total=0;
	sqlite3_stmt *stmt;
	sqlite3 *handle;

	if (account_exists(account)==0) {
		return 0;
	}

	if (year <= 0) {
		sprintf((char*)query,
				"select sum(receive-spend) as totalbalance from "	\
				"transactions where currency=\"%s\";",currency);
	}
	else {
		sprintf((char*)query,
				"select sum(receive-spend) as totalbalance from "	\
				"transactions where ((currency=\"%s\") and " \
				"(CAST(substr(date,1,4) as INT) < %d));",currency, year);
	}

	/* if no account is specified use the default one */
	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	if (strlen(account)==0) {
		printf("No account specified\n");
		return 0;
	}
	database_name(account, (char*)database_filename);
	retval = sqlite3_open_v2((char*)database_filename,&handle,
							 SQLITE_OPEN_READONLY,NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		sqlite3_close(handle);
		return 0;
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if(retval) {
		printf("Unable to retrieve total balance information.  " \
			   "Does this account '%s' exist?\n",account);
		sqlite3_close(handle);
		return 0;
	}

	cols = sqlite3_column_count(stmt);

	while(1) {
		retval = sqlite3_step(stmt);

		if (retval == SQLITE_ROW) {
			for(col=0; col < cols; col++) {
				char *val = (char*)sqlite3_column_text(stmt,col);
				if (is_value(val)!=0) {
					total += atof(val);
				}
			}
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running balance query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	sqlite3_close(handle);
	return total;
}

int show_summary_totals(char * account)
{
	char currency_str[STRING_BLOCK];
	char receive_str[STRING_BLOCK];
	char spend_str[STRING_BLOCK];
	char balance_str[STRING_BLOCK];
	int retval,col,cols;
	char database_filename[STRING_BLOCK];
	char query[] = "select currency, sum(spend) as totalspend, " \
		"sum(receive) as totalreceive, sum(receive-spend) " \
		"as balance from transactions group by currency;";
	int row=0;
	sqlite3_stmt *stmt;
	sqlite3 *handle;

	if (account_exists(account)==0) {
		return 0;
	}

	/* if no account is specified use the default one */
	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	if (strlen(account)==0) {
		printf("No account specified\n");
		return 0;
	}
	database_name(account, (char*)database_filename);
	retval = sqlite3_open_v2((char*)database_filename,&handle,
							 SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
							 NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		sqlite3_close(handle);
		return 0;
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if(retval) {
		printf("Unable to retrieve balance information.  " \
			   "Does this account '%s' exist?\n",account);
		sqlite3_close(handle);
		return 0;
	}

	cols = sqlite3_column_count(stmt);

	row = 0;
	while(1) {
		retval = sqlite3_step(stmt);

		if (retval == SQLITE_ROW) {
			for(col=0; col < cols; col++) {
				char *val = (char*)sqlite3_column_text(stmt,col);
				if (col==0) {
					sprintf((char*)currency_str,"%s",val);
				}
				if (col==1) {
					pad_value(val, (char *)spend_str,
							  LEADING_SPACES, TRAILING_ZEROS);
				}
				if (col==2) {
					pad_value(val, (char *)receive_str,
							  LEADING_SPACES, TRAILING_ZEROS);
				}
				if (col==3) {
					pad_value(val, (char *)balance_str,
							  LEADING_SPACES, TRAILING_ZEROS);
				}
			}

#ifdef USE_COLOURS
			printf("%s",WHITE);
#endif
			printf(get_text_from_identifier(SUMMARY_BALANCE),
				   (char*)currency_str,(char*)receive_str,
				   (char*)spend_str,(char*)balance_str);
#ifdef USE_COLOURS
			printf("%s",NORMAL);
#endif
			printf("\n");

			row++;
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running balance query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	sqlite3_close(handle);
	return 1;
}

int show_recent_transactions(char * account, int no_of_transactions,
							 char column_separator)
{
	int retval,col,cols,row,i;
	char database_filename[STRING_BLOCK];
	char receive_str[STRING_BLOCK];
	char spend_str[STRING_BLOCK], date_str[STRING_BLOCK];
	char query[] = "select transaction_number, date, receive, spend, " \
		"status, description, supplier from transactions " \
		"order by transaction_number desc;";
	char * date_format = settings_get_date_format();
	int start_column=45;
	sqlite3_stmt *stmt;
	sqlite3 *handle;

	if (account_exists(account)==0) {
		return 0;
	}

	clear_transaction_numbers();

	/* if no account is specified use the default one */
	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	if (strlen(account)==0) {
		printf("No account specified\n");
		return 0;
	}
	database_name(account, (char*)database_filename);
	retval = sqlite3_open_v2((char*)database_filename,
							 &handle,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
							 NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		sqlite3_close(handle);
		return 0;
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if(retval) {
		printf("Selecting recent transactions from %s account failed\n",
			   account);
		sqlite3_close(handle);
		return 0;
	}

	cols = sqlite3_column_count(stmt);

	/* title */
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	printf(get_text_from_identifier(TITLE_RECENT_TRANSACTIONS),
		   column_separator,column_separator,column_separator,
		   column_separator,column_separator,column_separator,
		   column_separator);
	printf("\n");
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	for (i=0;i<MAX_ROW_LENGTH;i++) {
		if ((column_separator==' ') ||
			((i!=3) && (i!=15) && (i!=25) && (i!=35) && (i!=start_column))) {
			printf("-");
		}
		else {
			printf("+");
		}
	}
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	printf("\n");

	row=0;
	while(1) {
		retval = sqlite3_step(stmt);

		if(retval == SQLITE_ROW) {
			if (column_separator!=' ') {
				printf("%c",column_separator);
			}

			/* transaction number */
			if (row+1 < 10) printf(" ");
			if (row+1 < 100) printf(" ");
			printf("%d%c",row+1,column_separator);

			/* store the transaction number */
			if (cols>0) {
				char *val = (char*)sqlite3_column_text(stmt,0);
				save_transaction_number(val);
			}

#ifdef USE_COLOURS
			if ((column_separator==' ') && (cols>3)) {
				if ((char*)sqlite3_column_text(stmt,2)!=NULL) {
					if (atof((char*)sqlite3_column_text(stmt,2))>0) {
						printf("%s",
							   settings_get_theme_colour(COLOUR_POSITIVE));
					}
				}
				if ((char*)sqlite3_column_text(stmt,3)!=NULL) {
					if (atof((char*)sqlite3_column_text(stmt,3))>0) {
						printf("%s",
							   settings_get_theme_colour(COLOUR_NEGATIVE));
					}
				}
			}
#endif

			for(col=1;col<cols;col++) {
				char * val = (char*)sqlite3_column_text(stmt,col);
				if (col==2) {
					pad_value(val, (char *)receive_str,
							  LEADING_SPACES, TRAILING_ZEROS);
					printf("%s %c",receive_str,column_separator);
				}
				else {
					if (col==3) {
						pad_value(val, (char *)spend_str,
								  LEADING_SPACES, TRAILING_ZEROS);
						printf("%s %c",spend_str,column_separator);
					}
					else {
						if (col==4) {
							/* status */
							pad_text(val,8);
						}
						if (col<cols-2) {
							if (col==1) {
								convert_standard_to_date(val,
														 date_format,
														 date_str);
								printf("%s %c",date_str,column_separator);
							}
							else {
								printf("%s %c",val,column_separator);
							}
						}
						else {
							if (col < cols-1) {
								description_multiline(val,
													  column_separator,
													  start_column);
							}
							else {
								if (strlen(val)>0) {
									printf("(%s)%c",val,column_separator);
								}
							}

						}
					}
				}
			}

#ifdef USE_COLOURS
			if (column_separator==' ') {
				printf("%s",NORMAL);
			}
#endif
			printf("\n");
			row++;
			if (row>=no_of_transactions) break;
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running balance query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	sqlite3_close(handle);
	return 1;
}


int show_summary_balance(char * account)
{
	int i,retval = 1;
	char total_spend[MAX_CURRENCIES][30];
	char total_receive[MAX_CURRENCIES][30];
	char total_balance[MAX_CURRENCIES][30];
	char currency[MAX_CURRENCIES][30];

	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	retval = get_balance(account,(char**)currency,(char**)total_spend,
						 (char**)total_receive,(char**)total_balance);
	if (retval!=0) {
		i=0;
		while (i<MAX_CURRENCIES) {
			if (atof((char*)&total_spend[i]) +
				atof((char*)&total_receive[i]) == 0) {
				break;
			}
			printf(get_text_from_identifier(SUMMARY_BALANCE),
				   (char*)&currency[i],(char*)&total_receive[i],
				   (char*)&total_spend[i],(char*)&total_balance[i]);
			printf("\n");
			i++;
		}
	}
	return retval;
}


int show_summary(char * account, int no_of_transactions,
				 char column_separator)
{
	int i,retval = 1;
	char total_spend[MAX_CURRENCIES][30];
	char total_receive[MAX_CURRENCIES][30];
	char total_balance[MAX_CURRENCIES][30];
	char currency[MAX_CURRENCIES][30];
	char str[100],str2[100];

	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	retval = get_balance(account,(char**)currency,(char**)total_spend,
						 (char**)total_receive,(char**)total_balance);
	if (retval != 0) {

		if (atof(((char*)&total_receive[0])) +
			atof(((char*)&total_spend[0]))==0.0) {
			printf("No records\n");
			return 0;
		}

		if (column_separator == ' ') {
			sprintf((char*)str, "%s\n",
					get_text_from_identifier(TITLE_ACCOUNT_SUMMARY));
		}
		else {
			sprintf((char*)str, "* %s\n",
					get_text_from_identifier(TITLE_ACCOUNT_SUMMARY));
		}
		sprintf((char*)str2, str, account);
		str2[0] = toupper(str2[0]);
		printf("%s", (char*)str2);
		if (column_separator == ' ') {
			for (i = 0; i < MAX_ROW_LENGTH; i++) {
				printf("-");
			}
		}
		printf("\n");

		retval = show_recent_transactions(account, no_of_transactions, column_separator);

		if (column_separator == ' ') {
			for (i = 0; i < MAX_ROW_LENGTH; i++) {
				printf("-");
			}
		}
		printf("\n");

		if (retval != 0) {
			retval = show_summary_totals(account);
		}
	}
	return retval;
}

int show_recent_adjustments(char * account, int no_of_transactions,
							char column_separator,
							char * search_string,
							int year, int month_number)
{
	int retval,col,cols,row,i;
	char database_filename[STRING_BLOCK];
	char receive_str[STRING_BLOCK];
	char spend_str[STRING_BLOCK], date_str[STRING_BLOCK];
	char search_sql[STRING_BLOCK];
	char query_base[STRING_BLOCK_LONG];
	char query[STRING_BLOCK_LONG];
	char * date_format = settings_get_date_format();
	int start_column=43;
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char * str;

	if (account_exists(account)==0) {
		return 0;
	}

	/* if no account is specified use the default one */
	if (strlen(account)==0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	if (strlen(account)==0) {
		printf("No account specified\n");
		return 0;
	}
	database_name(account, (char*)database_filename);
	retval = sqlite3_open_v2((char*)database_filename,
							 &handle,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
							 NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		sqlite3_close(handle);
		return 0;
	}

	sprintf(query_base,"%s","select transaction_number, " \
			"date, receive, spend, " \
			"status, description, supplier, " \
			"adjustment_date, adjustment_type" \
			" from adjustments ");

	if (search_string==0) {
		sprintf(query,"%s order by adjustment_date desc;",query_base);
	}
	else {
		if (strlen(search_string)==0) {
			sprintf(query,"%s order by adjustment_date desc;",query_base);
		}
		else {
			search_string_to_sql(search_string, search_sql,
								 year, month_number);
			sprintf(query,"%s " \
					"where (%s) or " \
					"(date like \"%s\") " \
					"order by adjustment_date desc;",
					query_base, search_sql, search_string);
		}
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if(retval) {
		printf("Selecting recent adjustments from %s account failed\n",
			   account);
		sqlite3_close(handle);
		return 0;
	}

	cols = sqlite3_column_count(stmt);

	for (i=0;i<MAX_ROW_LENGTH;i++) {
		printf("-");
	}
	printf("\n");

	/* title */
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	printf(get_text_from_identifier(TITLE_ADJUSTMENTS),
		   column_separator,column_separator,column_separator,
		   column_separator,column_separator,column_separator,
		   column_separator);
	printf("\n");
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	for (i = 0; i < MAX_ROW_LENGTH; i++) {
		if ((column_separator==' ') ||
			((i!=1) && (i!=13) && (i!=23) && (i!=33) && (i!=start_column))) {
			printf("-");
		}
		else {
			printf("+");
		}
	}
	if (column_separator!=' ') {
		printf("%c",column_separator);
	}
	printf("\n");

	row=0;
	while(1) {
		retval = sqlite3_step(stmt);

		if(retval == SQLITE_ROW) {
			if (column_separator!=' ') {
				printf("%c",column_separator);
			}

			if ((char*)sqlite3_column_text(stmt,8)!=NULL) {
				printf("%s%c",
					   (char*)sqlite3_column_text(stmt,8),
					   column_separator);
			}
			else {
				printf("-%c",column_separator);
			}

#ifdef USE_COLOURS
			if (column_separator==' ') {
				str = (char*)sqlite3_column_text(stmt,8);
				if (str!=NULL) {
					if (str[0]==ADJUSTMENT_TYPE_BEFORE) {
						printf("%s",
							   settings_get_theme_colour(COLOUR_BEFORE));
					}
					if (str[0]==ADJUSTMENT_TYPE_AFTER) {
						printf("%s",
							   settings_get_theme_colour(COLOUR_AFTER));
					}
					if (str[0]==ADJUSTMENT_TYPE_DELETE) {
						printf("%s",
							   settings_get_theme_colour(COLOUR_DELETE));
					}
				}
			}
#endif

			for(col=1;col<cols-2;col++) {
				char * val = (char*)sqlite3_column_text(stmt,col);
				if (col==2) {
					pad_value(val, (char *)receive_str,
							  LEADING_SPACES, TRAILING_ZEROS);
					printf("%s %c",receive_str,column_separator);
				}
				else {
					if (col==3) {
						pad_value(val, (char *)spend_str,
								  LEADING_SPACES, TRAILING_ZEROS);
						printf("%s %c",spend_str,column_separator);
					}
					else {
						if (col==4) {
							/* status */
							pad_text(val,8);
						}
						if (col==5) {
							description_multiline(val,
												  column_separator,
												  start_column);
						}
						else {
							if (col<cols-2) {
								if (col==1) {
									convert_standard_to_date(val,
															 date_format,
															 date_str);
									printf("%s %c",date_str,column_separator);
								}
								else {
									printf("%s %c",val,column_separator);
								}
							}
							else {
								if (strlen(val)>0) {
									printf("(%s)%c",val,column_separator);
								}
							}
						}
					}
				}
			}

#ifdef USE_COLOURS
			if (column_separator==' ') {
				printf("%s",NORMAL);
			}
#endif
			printf("\n");
			row++;
			if (row>=no_of_transactions) break;
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running " \
				   "adjustments summary query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	sqlite3_close(handle);
	return 1;
}
