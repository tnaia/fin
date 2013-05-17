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

#include "search.h"

int search(char * account, char * search_string, int month_number,
		   int year, int max_rows, char * date_format,
		   char * export_filename, char field_separator,
		   char * kml_filename, char column_separator)
{
	int retval, col, cols, i;
	char database_filename[STRING_BLOCK];
	char query[STRING_BLOCK*2];
	int row=0;
	char str[STRING_BLOCK],str2[STRING_BLOCK];
	double total_receive=0,total_spend=0;
	char currency_str[STRING_BLOCK];
	char total_receive_str[STRING_BLOCK];
	char total_spend_str[STRING_BLOCK];
	char total_balance_str[STRING_BLOCK];
	char receive_str[STRING_BLOCK];
	char spend_str[STRING_BLOCK],date_str[STRING_BLOCK];
	char fields[STRING_BLOCK];
	char search_sql[STRING_BLOCK];
	FILE * fp_export, * fp_kml;
	int start_column = 45;
	sqlite3_stmt *stmt;
	sqlite3 *handle;

	sprintf((char*)fields,"%s",
			"transaction_number, date, receive, " \
			"spend, status, description, supplier");

	fp_export = 0;
	fp_kml = 0;

	/* open file for export */
	if (export_filename!=0) {
		if (strlen(export_filename)>2) {
			fp_export = fopen(export_filename,"w");
			if (fp_export!=0) {
				sprintf((char*)str,"%s",fields);
				for (i=0;i<strlen(str);i++) {
					if (str[i]==',') {
						str[i]=field_separator;
					}
				}
				fprintf(fp_export,"%s\n",str);
			}
		}
	}

	if (kml_filename!=0) {
		if (strlen(kml_filename)>2) {
			fp_kml = fopen(kml_filename,"w");
			if (fp_kml!=0) {
				sprintf((char*)fields,"%s",
						"transaction_number, date, receive, " \
						"spend, status, description, supplier, " \
						"longitude, latitude, altitude");

				fprintf(fp_kml,"%s",
						"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
				fprintf(fp_kml,"%s",
						"<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
				fprintf(fp_kml,"%s",
						"  <Document>\n");
			}
		}
	}

	clear_transaction_numbers();

	if ((fp_export==0) && (fp_kml==0)) {
		if (column_separator==' ') {
			sprintf((char*)str,"%s\n",
					get_text_from_identifier(TITLE_SEARCH_RESULTS));
		}
		else {
			sprintf((char*)str,"* %s\n",
					get_text_from_identifier(TITLE_SEARCH_RESULTS));
		}
		sprintf((char*)str2,str,account);
		str2[0] = toupper(str2[0]);
		printf("%s",(char*)str2);

		if (column_separator==' ') {
			for (i=0;i<MAX_ROW_LENGTH;i++) {
				printf("-");
			}
		}
		printf("\n");

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
				((i!=4) && (i!=16) && (i!=26) &&
				 (i!=36) && (i!=start_column))) {
				printf("-");
			}
			else {
				printf("+");
			}
		}
		printf("%c\n",column_separator);
	}

	search_string_to_sql(search_string, search_sql, year, month_number);

	sprintf((char*)query,
			"select %s from transactions where " \
			"(%s) order by date desc;",
			fields,search_sql);

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
	sprintf((char*)database_filename,"%s/.fin/%s.sqlite3",
			getenv("HOME"),account);
	retval = sqlite3_open_v2(database_filename,&handle,
							 SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
							 NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		return 0;
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if (retval) {
		printf("Unable to retrieve search information.  " \
			   "Does this account '%s' exist?\n",account);
		return 0;
	}
    
	cols = sqlite3_column_count(stmt);

	row = 0;
	while(1) {
		retval = sqlite3_step(stmt);
        
		if(retval == SQLITE_ROW) {
			if ((fp_export==0) && (fp_kml==0)) {
				if (column_separator!=' ') {
					printf("%c",column_separator);
				}
				if (row+1<10) printf(" ");
				if (row+1<100) printf(" ");
				printf("%d ",row+1);
			}

			if ((fp_export!=0) || (fp_kml!=0)) {
				if (fp_export!=0) {
					for(col=0; col<cols; col++) {
						char *val = (char*)sqlite3_column_text(stmt,col);

						if (col>0) {
							fprintf(fp_export,"%c",field_separator);
						}

						if (col==1) {
							convert_standard_to_date(val, date_format,
													 date_str);
							fprintf(fp_export,"%s",date_str);
						}
						else {
							fprintf(fp_export,"%s",val);
						}
					}
					fprintf(fp_export,"%s","\n");
				}
				if (fp_kml!=0) {
					if (atof((char*)sqlite3_column_text(stmt,7)) +
						atof((char*)sqlite3_column_text(stmt,7)) != 0) {
						fprintf(fp_kml,"%s","    <Placemark>\n");
						if (atof((char*)sqlite3_column_text(stmt,2))!=0) {
							sprintf((char*)str,
									get_text_from_identifier(MESSAGE_RECEIVED),
									(char*)sqlite3_column_text(stmt,2));
						}
						else {
							sprintf((char*)str,
									get_text_from_identifier(MESSAGE_SPENT),
									(char*)sqlite3_column_text(stmt,3));
						}
						fprintf(fp_kml,"      <name>%s</name>\n",
								(char*)str);
						fprintf(fp_kml,
								"      <description>%s</description>\n",
								(char*)sqlite3_column_text(stmt,5));
						fprintf(fp_kml,"%s","      <Point>\n");
						fprintf(fp_kml,
								"        <coordinates>%s,%s,%s</coordinates>\n",
								(char*)sqlite3_column_text(stmt,7),
								(char*)sqlite3_column_text(stmt,8),
								(char*)sqlite3_column_text(stmt,9));
						fprintf(fp_kml,"%s","      </Point>\n");
						fprintf(fp_kml,"%s","      <TimeStamp>\n");
						fprintf(fp_kml,"      <when>%s</when>\n",
								(char*)sqlite3_column_text(stmt,1));
						fprintf(fp_kml,"%s","      </TimeStamp>\n");
						fprintf(fp_kml,"%s","    </Placemark>\n");
					}
				}
			}
			else {

				if (cols>0) {
					char *val = (char*)sqlite3_column_text(stmt,0);
					save_transaction_number(val);
				}

				if (column_separator!=' ') {
					printf("%c",column_separator);
				}

#ifdef USE_COLOURS
				if (column_separator==' ') {
					if (atof((char*)sqlite3_column_text(stmt,2))>0) {
						printf("%s",COLOUR_POSITIVE);
					}
					if (atof((char*)sqlite3_column_text(stmt,3))>0) {
						printf("%s",COLOUR_NEGATIVE);
					}
				}
#endif

				for(col=1; col<cols; col++) {
					char *val = (char*)sqlite3_column_text(stmt,col);
					if (col==2) {
						total_receive += atof(val);
					}
					if (col==3) {
						total_spend += atof(val);
					}

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
								pad_text(val,8);
							}
							if (col<cols-2) {
								if (col==1) {
									convert_standard_to_date(val,
															 date_format,
															 date_str);
									printf("%s %c",date_str,
										   column_separator);
								}
								else {
									printf("%s %c",val,column_separator);
								}
							}
							else {
								if (col < cols-1) {
									/*printf("%s%c",val,column_separator);*/
									description_multiline(val,
														  column_separator,
														  start_column);

								}
								else {	 
									if (strlen(val)>0) {
										printf("(%s)%c",val,
											   column_separator);
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

			}
			if ((fp_export==0) && (fp_kml==0)) {
				printf("\n");
			}
			row++;
			if (row>=max_rows) break;
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running search query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	if ((fp_export==0) && (fp_kml==0)) {
		if (column_separator==' ') {
			for (i=0;i<MAX_ROW_LENGTH;i++) {
				printf("-");
			}
		}
		printf("\n");
  
		sprintf((char*)currency_str,"%s",
				get_text_from_identifier(TOTALS));
		sprintf((char*)total_receive_str,"%.2f",total_receive);
		sprintf((char*)total_spend_str,"%.2f",total_spend);
		sprintf((char*)total_balance_str,"%.2f",
				total_receive-total_spend);
		printf(get_text_from_identifier(SUMMARY_BALANCE),
			   (char*)currency_str,(char*)total_receive_str,
			   (char*)total_spend_str,(char*)total_balance_str);
		printf("\n");
	}
	else {
		if (fp_export!=0) {
			fclose(fp_export);
			printf(get_text_from_identifier(EXPORT_COMPLETED),
				   export_filename);
		}
		if (fp_kml!=0) {
			fprintf(fp_kml,"%s","  </Document>\n");
			fprintf(fp_kml,"%s","</kml>\n");
			fclose(fp_kml);
			printf(get_text_from_identifier(EXPORT_COMPLETED),
				   kml_filename);
		}
		printf("\n");    
	}

	sqlite3_close(handle);

	return 1;
}

int search_grouped(char * account, char * search_string, int year,
				   int max_rows, char * date_format,
				   char * export_filename, char field_separator,
				   char * image_filename, char column_separator,
				   int monthly)
{
	int retval, col, cols, i;
	char database_filename[STRING_BLOCK];
	char query[STRING_BLOCK*2];
	int row=0;
	char str[STRING_BLOCK],str2[STRING_BLOCK],titlestr[STRING_BLOCK];
	double total_receive=0,total_spend=0;
	char currency_str[STRING_BLOCK];
	char total_receive_str[STRING_BLOCK];
	char total_spend_str[STRING_BLOCK];
	char total_balance_str[STRING_BLOCK];
	char receive_str[STRING_BLOCK];
	char spend_str[STRING_BLOCK];
	char fields[STRING_BLOCK];
	char title[STRING_BLOCK];
	char alt_export_filename[STRING_BLOCK];
	FILE * fp_export;
	int line_length=30;
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char search_sql[STRING_BLOCK];

	if (monthly != 0) {
		/* monthly */
		sprintf((char*)fields,"%s",
				"substr(date,1,7) as yearmonth, " \
				"sum(receive) as totreceive, " \
				"sum(spend) as totspend");
	}
	else {
		/* yearly */
		sprintf((char*)fields,"%s",
				"substr(date,1,4) as yearmonth, " \
				"sum(receive) as totreceive, " \
				"sum(spend) as totspend");
	}

	if (image_filename != 0) {
		if (strlen(image_filename) > 0) {
			field_separator = 9;
			if (strlen(export_filename) == 0) {
				sprintf((char*)alt_export_filename,"gnuplot.csv");
				export_filename = alt_export_filename;	
			}
		}
	}

	fp_export = 0;

	/* open file for export */
	if (export_filename != 0) {
		if (strlen(export_filename) > 2) {
			fp_export = fopen(export_filename,"w");
			if (fp_export != 0) {
				if (monthly != 0) {
					sprintf((char*)str,"%s","Month,receive,spend");
				}
				else {
					sprintf((char*)str,"%s","Year,receive,spend");
				}
				for (i = 0; i < strlen(str); i++) {
					if (str[i] == ',') {
						str[i] = field_separator;
					}
				}
				fprintf(fp_export, "%s\n", str);
			}
		}
	}

	if (fp_export == 0) {
		if (column_separator == ' ') {
			sprintf((char*)str, "%s\n",
					get_text_from_identifier(TITLE_SEARCH_RESULTS));
		}
		else {
			sprintf((char*)str, "* %s\n",
					get_text_from_identifier(TITLE_SEARCH_RESULTS));
		}
		sprintf((char*)str2, str, account);
		str2[0] = toupper(str2[0]);
		printf("%s", (char*)str2);

		if (column_separator == ' ') {
			for (i = 0; i < line_length; i++) {
				printf("-");
			}
		}
		printf("\n");

		if (column_separator != ' ') {
			printf("%c", column_separator);
		}
		printf(get_text_from_identifier(TITLE_RECENT_TRANSACTIONS_MONTHLY),
			   column_separator, column_separator, column_separator);
		printf("\n");

		if (column_separator != ' ') {
			printf("%c", column_separator);
		}
		for (i = 0;i < line_length; i++) {
			if ((i==line_length-2) && (column_separator == '|')) {
				break;
			}
			if ((column_separator == ' ') ||
				((i != 8) && (i != 18))) {
				printf("-");
			}
			else {
				printf("+");
			}
		}
		printf("%c\n",column_separator);
	}

	search_string_to_sql(search_string, search_sql, 0, 0);

	if (year == 0) {
		sprintf((char*)query,
				"select %s from transactions where " \
				"(%s) or " \
				"(date like \"%s\") group by yearmonth " \
				"order by yearmonth desc;",
				fields, search_sql, search_string);
	}
	else {
		if (strcmp(search_string,"*") == 0) {
			sprintf((char*)query,
					"select %s from transactions where " \
					"(date like \"%d-%c-%c\") group by " \
					"yearmonth order by yearmonth desc;",
					fields, year, '%', '%');
		}
		else {
			sprintf((char*)query,
					"select %s from transactions where " \
					"(%s) and " \
					"(date like \"%d-%c-%c\") group by " \
					"yearmonth order by yearmonth desc;",
					fields, search_sql, year, '%', '%');
		}
	}

	/* if no account is specified use the default one */
	if (strlen(account) == 0) {
		settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT)) == 0) {
			printf("Unable to get default account name\n");
			return 0;
		}
		account = settings_get_account();
	}

	if (strlen(account) == 0) {
		printf("No account specified\n");
		return 0;
	}
	sprintf((char*)database_filename,"%s/.fin/%s.sqlite3",
			getenv("HOME"), account);
	retval = sqlite3_open_v2(database_filename,
							 &handle,
							 SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
							 NULL);
	/* If connection failed, handle returns NULL */
	if (retval) {
		printf("Sqlite3 database connection failed\n");
		return 0;
	}

	retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
	if (retval) {
		printf("Unable to retrieve search information.  " \
			   "Does this account '%s' exist?\n", account);
		return 0;
	}
    
	cols = sqlite3_column_count(stmt);

	row = 0;
	while(1) {
		retval = sqlite3_step(stmt);
        
		if(retval == SQLITE_ROW) {

			if (fp_export != 0) {
				for(col = 0; col < cols; col++) {
					char *val = (char*)sqlite3_column_text(stmt, col);
					if (col > 0) {
						fprintf(fp_export, "%c", field_separator);
					}
					fprintf(fp_export, "%s", val);
				}
				fprintf(fp_export, "%s", "\n");
			}
			else {
				if (column_separator != ' ') {
					printf("%c", column_separator);
				}
				for(col = 0; col < cols; col++) {
					char *val = (char*)sqlite3_column_text(stmt, col);
					if (col == 1) {
						total_receive += atof(val);
					}
					if (col == 2) {
						total_spend += atof(val);
					}

					if (col == 1) {
						pad_value(val, (char *)receive_str,
								  LEADING_SPACES, TRAILING_ZEROS);
						printf("%s %c", receive_str, column_separator);
					}
					else {
						if (col == 2) {
							pad_value(val, (char *)spend_str,
									  LEADING_SPACES, TRAILING_ZEROS);
							printf("%s %c", spend_str, column_separator);
						}
						else {
							if (strlen(val) > 0) {
								if ((col == 0) && (monthly == 0)) {
									printf("%s    %c", val, column_separator);
								}
								else {
									printf("%s %c", val, column_separator);
								}
							}
						}
					}
				}
			}
			if (fp_export == 0) {
				printf("\n");
			}
			row++;
			if (row >= max_rows) break;
		}
		else if(retval == SQLITE_DONE) {
			break;
		}
		else {
			printf("Some error encountered while running search query\n");
			sqlite3_close(handle);
			return 0;
		}
	}

	if (fp_export == 0) {
		if (column_separator == ' ') {
			for (i = 0; i < 58; i++) {
				printf("-");
			}
		}
		printf("\n");
  
		sprintf((char*)currency_str, "%s",
				get_text_from_identifier(TOTALS));
		sprintf((char*)total_receive_str, "%.2f", total_receive);
		sprintf((char*)total_spend_str, "%.2f", total_spend);
		sprintf((char*)total_balance_str, "%.2f", total_receive - total_spend);
		printf(get_text_from_identifier(SUMMARY_BALANCE),
			   (char*)currency_str, (char*)total_receive_str,
			   (char*)total_spend_str, (char*)total_balance_str);
		printf("\n");
	}
	else {
		fclose(fp_export);

		if (image_filename == 0) {
			printf(get_text_from_identifier(EXPORT_COMPLETED),
				   export_filename);
			printf("\n");
		}

		if (image_filename != 0) {
			if (strlen(image_filename) > 0) {
				for (i = 0; i < strlen(search_string); i++) {
					if (search_string[i] == '%') {
						search_string[i] = '*';
					}
				}

				/* default title */
				sprintf((char*)title,
						get_text_from_identifier(TITLE_SPENDING_ON_ACCOUNT_SEARCH),
						search_string, account);

				/* if a custom title has been specified */
				if (strlen(title) != 0) {
					sprintf(titlestr, "%s", title);
				}

				plot(titlestr, "", 2, export_filename, 1024,
					 480, image_filename);

				printf(get_text_from_identifier(EXPORT_IMAGE),
					   image_filename);
				printf("\n");
			}
		}
	}

	sqlite3_close(handle);

	return 1;
}
