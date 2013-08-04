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

#include "parse.h"


static void to_lower(char * str)
{
	int i;
	if (str==0) return;
	for (i=0;i<strlen(str);i++) {
		str[i] = tolower(str[i]);
	}
}

static void remove_quotes(char * str)
{
	int i;
	if (str==0) return;
	if (strlen(str)>2) {
		if (str[0]=='"') {
			for (i=1;i<strlen(str);i++) {
				str[i-1] = str[i];
			}
			str[i-1]=0;
			for (i=strlen(str)-1;i>=0;i--) {
				if (str[i]=='"') {
					str[i]=0;
					break;
				}
			}
		}
	}
}

/* trim spaces, quotes and end of line characters */
void trim(char * str)
{
	int i=0,j=0;

	if (str==0) return;
	if (strlen(str)==0) return;

	while ((str[i]==' ') || (str[i]=='"')) i++;
	if (i!=0) {
		for (j=0;j<strlen(str)-i;j++) {
			str[j]=str[i+j];
		}
		str[j]=0;
	}
	i=strlen(str)-1;
	while ((i>1) && ((str[i]==' ') || (str[i]=='"') ||
					 (str[i]==10) || (str[i]==13))) i--;
	if (i!=strlen(str)-1) {
		str[i+1]=0;
	}
}

/* remove end of line characters */
void remove_eol(char * str)
{
	int i;
	if (str==0) return;
	if (strlen(str)>0) {
		for (i=1;i<=strlen(str);i++) {
			if ((str[i] == 10) || (str[i] == 13)) {
				str[i]=0;
			}
		}
	}
}

static int char_index(char * str, char ch)
{
	int i=0;
	for (i=0; i < strlen(str); i++) {
		if (str[i]==ch) break;
	}
	if (i==strlen(str)) {
		return -1;
	}
	return i;
}

/* is this a single word, or a word/value pair? */
int is_boolean_parameter(char * word, char * value)
{
	int is_boolean = 0;

	if ((strcmp(word,get_text_from_identifier(SUMMARY))==0) ||
		(strcmp(word,get_text_from_identifier(ACCOUNTS))==0)) {
		return 1;
	}

	if (strcmp(word,get_text_from_identifier(PIECHART))==0) {
		if ((strstr(value,".png")==0) &&
			(strstr(value,".jpg")==0) &&
			(strstr(value,".gif")==0) &&
			(strstr(value,".txt")==0) &&
			(strstr(value,".csv")==0)) {
			return 1;
		}
	}

	if ((strcmp(word,get_text_from_identifier(PERIOD_MONTHLY))==0) ||
		(strcmp(word,get_text_from_identifier(PERIOD_YEARLY))==0) ||
		(strcmp(word,get_text_from_identifier(VOLUME))==0) ||
		(strcmp(word,get_text_from_identifier(VOLUME_ABBREV))==0) ||
		(strcmp(word,get_text_from_identifier(ADJUSTMENTS))==0) ||
		(strcmp(word,get_text_from_identifier(ADJUSTMENTS_ABBREV))==0) ||
		(strcmp(word,get_text_from_identifier(DISTRIBUTION_ABBREV))==0) ||
		(strcmp(word,get_text_from_identifier(DISTRIBUTION))==0)) {
		if ((strstr(value,".png")==0) &&
			(strstr(value,".jpg")==0) &&
			(strstr(value,".gif")==0)) {
			is_boolean = 1;
		}
	}

	return is_boolean;
}


/* separate the parameters into fieldnames and values */
int parse(int argc, char * argv[],
		  char * fieldname, char * value,
		  char separator,
		  int start_index, int max_results)
{
	int results = 0;
	int i, pos, c;
	char * str, * str2;

	for (i=0;i<max_results;i++) {
		((char*)&fieldname[i*FIELD_WIDTH])[0]=0;
		((char*)&value[i*FIELD_WIDTH])[0]=0;
	}
  
	i = start_index;
	while (i < argc) {
		str = argv[i];
		if (strlen(argv[i]) > 0) {
			pos = char_index(str, separator);
			if (pos == -1) {
				sprintf((char*)&fieldname[results*FIELD_WIDTH],
						"%s",argv[i]);
				if (i < argc-1) {
					if (is_boolean_parameter(argv[i],argv[i+1])==0) {
						sprintf((char*)&value[results*FIELD_WIDTH],
								"%s",argv[i+1]);
						i++;
					}
				}
			}
			else {
				str2 = (char*)&fieldname[results*FIELD_WIDTH];
				for (c=0;c<pos;c++) {
					str2[c]=str[c];
				}
				str2[c]=0;
				str2 = (char*)&value[results*FIELD_WIDTH];
				for (c=pos+1;c<strlen(str);c++) {
					str2[c-pos-1]=str[c];
				}
				str2[c-pos-1]=0;
			}
			results++;
			if (results>=max_results) break;
		}
    
		i++;
	}

	/* remove end of line characters */
	if (results>0) {
		remove_eol((char*)&fieldname[(results-1)*FIELD_WIDTH]);
		remove_eol((char*)&value[(results-1)*FIELD_WIDTH]);
	}
	for (i = 0; i < results; i+=1) {
		remove_quotes((char*)&value[i*FIELD_WIDTH]);
	}
	/*
	  for (i = 0; i < results; i++) {
	  printf("%d  %s, %s\n",i,(char*)&fieldname[i*FIELD_WIDTH],(char*)(&value[i*FIELD_WIDTH]));
	  }
	*/
	return results;
}

char * get_field_string(char * name,
						int no_of_fields, char * fieldname, char * value)
{
	int i;
	if (name==0) return 0;
	if (strlen(name)==0) return 0;
	for (i=0;i<no_of_fields;i++) {
		/*printf("test %s = %s\n",(char*)&fieldname[i],name);*/
		if (strcmp((char*)&fieldname[i*FIELD_WIDTH],name)==0) {
			return (char*)&value[i*FIELD_WIDTH];
		}
	}
	return 0;
}

int get_field_value(char * name,
					int no_of_fields, char * fieldname, char * value)
{
	char * v = get_field_string(name,no_of_fields, fieldname, value);
	if (v!=0) {
		return atoi(v);
	}
	return 0;
}

char * get_field_string_from_identifier(int identifier,
										int no_of_fields,
										char * fieldname,
										char * value)
{
	return get_field_string(get_text_from_identifier(identifier),
							no_of_fields, fieldname, value);
}

int get_field_value_from_identifier(int identifier,
									int no_of_fields,
									char * fieldname,
									char * value)
{
	return get_field_value(get_text_from_identifier(identifier),
						   no_of_fields, fieldname, value);
}

char * get_field(int identifier, int no_of_fields,
				 char* fieldname, char* value)
{
	char * result =
		get_field_string_from_identifier(identifier,no_of_fields,
										 fieldname, value);
	if (result==0) {
		identifier++;
		result =
			get_field_string_from_identifier(identifier,
											 no_of_fields,
											 fieldname, value);
	}
	to_lower(result);
	return result;
}

char * get_summary(int no_of_fields, char* fieldname, char* value)
{
	return get_field(SUMMARY, no_of_fields, fieldname, value);
}

char * get_account_name(int no_of_fields, char* fieldname, char* value)
{
	return get_field(ACCOUNT, no_of_fields, fieldname, value);
}

char * get_description(int no_of_fields, char* fieldname, char* value)
{
	char * description = get_field(DESCRIPTION, no_of_fields,
								   fieldname, value);
	if (description==0) {
		description = get_field(IMPORT_DESCRIPTION, no_of_fields,
								fieldname, value);
		if (description==0) {
			description = get_field(MEMO, no_of_fields,
									fieldname, value);
			if (description==0) {
				description = get_field(NOTES, no_of_fields,
										fieldname, value);
				if (description==0) {
					return (char*)"";
				}
			}
		}
	}
	return description;
}

int get_year(int no_of_fields, char* fieldname, char* value)
{
	int year;
	char * result = get_field(COMMAND_YEAR, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		year = atoi(result);
		if ((year>1000) && (year<3000)) {
			return year;
		}
		return 0;
	}
}

int get_end_year(int no_of_fields, char* fieldname, char* value)
{
	int year;
	char * result = get_field(COMMAND_END_YEAR, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		year = atoi(result);
		if ((year>1000) && (year<3000)) {
			return year;
		}
		return 0;
	}
}

int get_month(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(COMMAND_MONTH,
							  no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		if ((strcmp(result,get_text_from_identifier(MONTH_JAN))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_JAN_ABBREV))==0)) {
			return 1;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_FEB))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_FEB_ABBREV))==0)) {
			return 2;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_MAR))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_MAR_ABBREV))==0)) {
			return 3;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_APR))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_APR_ABBREV))==0)) {
			return 4;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_MAY))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_MAY_ABBREV))==0)) {
			return 5;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_JUN))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_JUN_ABBREV))==0)) {
			return 6;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_JUL))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_JUL_ABBREV))==0)) {
			return 7;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_AUG))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_AUG_ABBREV))==0)) {
			return 8;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_SEP))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_SEP_ABBREV))==0)) {
			return 9;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_OCT))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_OCT_ABBREV))==0)) {
			return 10;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_NOV))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_NOV_ABBREV))==0)) {
			return 11;
		}
		if ((strcmp(result,get_text_from_identifier(MONTH_DEC))==0) ||
			(strcmp(result,
					get_text_from_identifier(MONTH_DEC_ABBREV))==0)) {
			return 12;
		}
		return 0;
	}
}

char * get_search(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SEARCH, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

/* convert a search string into equivalent sql text */
static void search_string_to_sql_base(char * search_string, char * result)
{
	int i,ctr=0;
	char str[STRING_BLOCK*2],str2[STRING_BLOCK*2];
	int multiple_searches = 0;
	int initial=0;

	/* replace wildcards */
	for (i = 0; i < strlen(search_string); i++) {
		if ((search_string[i] == ',') ||
			(search_string[i] == '+')) {
			multiple_searches = 1;
		}
		if (search_string[i] == '*') {
			search_string[i] = '%';
		}
	}

	if (multiple_searches == 0) {
		sprintf(result,"(description like \"%s\")",search_string);
		return;
	}
	else {
		i = 0;
		result[0]=0;
		while (i < strlen(search_string)) {
			if ((i==0) || (i==strlen(search_string)-1) ||
				(search_string[i]==',') ||
				(search_string[i]=='+')) {
				if (i > 0) {
					if (i==strlen(search_string)-1) {
						str[ctr++]=search_string[i];
					}
					str[ctr++]='"';
					str[ctr++]=')';
					if (i!=strlen(search_string)-1) {
						str[ctr++]=' ';
						str[ctr++]='o';
						str[ctr++]='r';
						str[ctr++]=' ';
					}
					str[ctr++]=0;
					sprintf(str2,"%s%s",result,str);
					sprintf(result,"%s",str2);
				}
				sprintf(str,"%s","(description like \"");
				ctr = strlen(str);
				if (i==0) {
					str[ctr++]=search_string[i];					
				}
				initial=1;
			}
			else {
				if (!((initial==1) && (search_string[i]==' '))) {
					str[ctr++] = search_string[i];
					initial=0;
				}
			}
			i++;
		}
	}
}

/* convert a search string into equivalent sql text */
void search_string_to_sql(char * search_string, char * result,
						  int year, int month_number)
{
	char base_result[STRING_BLOCK];
	time_t now;
	struct tm *d;

	/* blank search string */
	if (strlen(search_string)==0) {
		result[0]=0;
		return;
	}

	search_string_to_sql_base(search_string, base_result);

	result[0]=0;
	if ((year>0) && (month_number==0)) {
		if (strlen(search_string)>1) {
			sprintf(result,"(%s) and (CAST(substr(date,1,4) as INT) is %04d)",
					base_result,year);
		}
		else {
			sprintf(result,"(CAST(substr(date,1,4) as INT) is %04d)",
					year);
		}
		return;
	}
	/* if a month but no year is specified then use the last year */
	if ((year==0) && (month_number>0)) {
		time(&now);
		d = localtime(&now);
		year = YEAR_BASELINE + d->tm_year;
		if (month_number > d->tm_mon+1) year--;
	}
	if ((year>0) && (month_number>0)) {
		if (strlen(search_string)>1) {
			sprintf(result,"(%s) and (CAST(substr(date,1,4) as INT) is %04d)"	\
					" and (CAST(substr(date,6,2) as INT) is %02d)",
					base_result,year,month_number);
		}
		else {
			sprintf(result,"(CAST(substr(date,1,4) as INT) is %04d)"	\
					" and (CAST(substr(date,6,2) as INT) is %02d)",
					year,month_number);
		}
		return;
	}

	sprintf(result,"(%s)",base_result);
}

char * get_date_format(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(DATE_FORMAT, no_of_fields, fieldname, value);
	int i;
	if (result==0) {
		return (char*)"";
	}
	else {
		for (i=0;i<strlen(result);i++) {
			if (result[i]=='y') result[i]='Y';
		}
		return result;
	}
}

int get_summary_transactions(int no_of_fields, char* fieldname, char* value)
{
	char * transactions_str = get_field(SUMMARY_TRANSACTIONS,
										no_of_fields, fieldname, value);
	if (transactions_str==0) {
		transactions_str = get_field(SUMMARY_RECORDS,
									 no_of_fields, fieldname, value);
    }
	if (transactions_str==0) {
		return 0;
	}
	else {
		return atoi(transactions_str);
	}
}

char * get_to(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(TO, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_lock(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(LOCK, no_of_fields, fieldname, value);
	return result;
}

char * get_backup(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(BACKUP, no_of_fields, fieldname, value);
	return result;
}

char * get_restore(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(RESTORE, no_of_fields, fieldname, value);
	return result;
}

char * get_VAT_rate(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(VAT_RATE, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		if (is_value(result)!=0) {
			return result;
		}
		return (char*)"";
	}
}


char * get_invoice_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(INVOICE_NUMBER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_supplier(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SUPPLIER, no_of_fields, fieldname, value);
	if (result==0) {
		result = get_field(AT, no_of_fields, fieldname, value);
	}
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_customer(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(CUSTOMER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_speak(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SPEAK, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_adjustments(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(ADJUSTMENTS, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_time(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(TIME, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_volume(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(VOLUME, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

double get_maximum(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(MAXIMUM, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return atof(result);
	}
}

char * get_distrib(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(DISTRIBUTION, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

double get_savings_interest_rate(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SAVINGS_INTEREST_RATE, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return 0.0;
	}
	else {
		return atof(result);
	}
}

double get_loans_interest_rate(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(LOANS_INTEREST_RATE, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return DEFAULT_USURY_RATE;
	}
	else {
		return atof(result);
	}
}

double get_inflation_rate(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(INFLATION_RATE, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return DEFAULT_INFLATION_RATE;
	}
	else {
		return atof(result);
	}
}

char * get_part_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(PART_NUMBER, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_theme(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(THEME, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_sale_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SALE_NUMBER, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_purchase_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(PURCHASE_NUMBER, no_of_fields,
							  fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_latitude(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(LATITUDE, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_language(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(LANGUAGE, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}


char * get_longitude(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(LONGITUDE, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_altitude(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(ALTITUDE, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_works_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(WORKS_NUMBER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_help(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(HELP, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_version(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(VERSION, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_serial_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(SERIAL_NUMBER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_job_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(JOB_NUMBER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_tracking_number(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(TRACKING_NUMBER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

int get_progress(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(PROGRESS, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return atoi(result);
	}
}

char * get_date(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(DATE, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_currency(int no_of_fields, char* fieldname, char* value)
{
	int i;
	char * result = get_field(CURRENCY, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		if (strlen(result)==3) {
			for (i=0;i<strlen(result);i++) {
				result[i]=toupper(result[i]);
			}
			return result;
		}
		else {
			printf("%s",get_text_from_identifier(WARNING_CURRENCY));
			printf("\n");
			return (char*)"";
		}
	}
}

char * get_status(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field(STATUS, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_quantity(int no_of_fields, char* fieldname, char* value)
{
	char * result =  get_field(QUANTITY, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_transfer(int no_of_fields, char* fieldname, char* value)
{
	char * result =  get_field(TRANSFER, no_of_fields, fieldname, value);
	if (result==0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_edit(int no_of_fields, char* fieldname, char* value)
{
	char * result =  get_field(EDIT, no_of_fields, fieldname, value);
	return result;
}

char * get_push(int no_of_fields, char* fieldname, char* value)
{
	char * result =  get_field(PUSH, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_pull(int no_of_fields, char* fieldname, char* value)
{
	char * result =  get_field(PULL, no_of_fields, fieldname, value);
	if (result==0) {
		return 0;
	}
	else {
		return result;
	}
}

char * get_spend(int no_of_fields, char* fieldname, char* value)
{
	int i;
	char * result = 0;
	int cmd[] = {
		COMMAND_SPEND,
		COMMAND_OUTGOING,
		COMMAND_PAID,
		COMMAND_BUY,
		COMMAND_BOUGHT,
		COMMAND_PURCHASE,
		COMMAND_PURCHASED,
		COMMAND_SPENT
	};

	for (i = 0; i < 8; i++) {
		result = get_field_string_from_identifier(cmd[i],no_of_fields, fieldname, value);
		/*printf("test %s %d\n",result,no_of_fields);*/
		if (result != 0) break;
	}

	to_lower(result);
	return result;
}

char * get_receive(int no_of_fields, char* fieldname, char* value)
{
	int i;
	char * result = 0;
	int cmd[] = {
		COMMAND_SELL,
		COMMAND_SOLD,
		COMMAND_INCOME,
		COMMAND_EARN,
		COMMAND_EARNED,
		COMMAND_EARNINGS,
		COMMAND_RECEIVE,
		COMMAND_RECEIVED
	};

	for (i = 0; i < 8; i++) {
		result = get_field_string_from_identifier(cmd[i],
												  no_of_fields,
												  fieldname, value);
		if (result != 0) break;
	}

	to_lower(result);
	return result;
}

char * get_import(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_IMPORT,
													 no_of_fields,
													 fieldname, value);
	if (result == 0) {
		return (char*)"";
	}
	else {
		return result;
	}
}

char * get_export(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_EXPORT,
													 no_of_fields,
													 fieldname, value);
	if (result == 0) {
		return (char*)"";
	}
	else {
		to_lower(result);
		return result;
	}
}

char * get_style(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_STYLE,
													 no_of_fields,
													 fieldname, value);
	return result;
}

char get_field_separator(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_SEPARATOR,
													 no_of_fields, 
													 fieldname, value);
	if (result != 0) {
		if (strlen(result)>0) {
			if (strcmp(result,get_text_from_identifier(TAB))==0) {
				return (char)9;
			}
			return result[0];
		}
	}
	return ',';
}

char * get_report(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_REPORT,
													 no_of_fields,
													 fieldname, value);
	to_lower(result);
	return result;
}

char * get_list(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_LIST,
													 no_of_fields,
													 fieldname, value);
	if (result==0) {
		result = get_field_string_from_identifier(COMMAND_LS,
												  no_of_fields,
												  fieldname, value);
	}
	to_lower(result);
	return result;
}

char * get_undo(int no_of_fields, char* fieldname, char* value)
{
	char * result;
	int i;
	int cmd[] = {
		TRANSACTION_CANCEL,
		TRANSACTION_REMOVE,
		TRANSACTION_REVERSE,
		TRANSACTION_DELETE,
		TRANSACTION_KILL,
		TRANSACTION_UNDO
	};
	for (i=0;i<6;i++) {
		result = get_field_string_from_identifier(cmd[i],no_of_fields,
												  fieldname, value);
		if (result!=0) break;
	}

	return result;
}

char * get_tests(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(COMMAND_TESTS,
													 no_of_fields,
													 fieldname, value);
	to_lower(result);
	return result;
}

char * get_monthly(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(PERIOD_MONTHLY,
													 no_of_fields,
													 fieldname, value);
	return result;
}

char * get_yearly(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(PERIOD_YEARLY,
													 no_of_fields,
													 fieldname, value);
	return result;
}

char * get_kml(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(KML,no_of_fields,
													 fieldname, value);
	return result;
}

char * get_title(int no_of_fields, char* fieldname, char* value)
{
	char * result =
		get_field_string_from_identifier(TITLE,no_of_fields,
										 fieldname, value);
	return result;
}

char * get_username(int no_of_fields, char* fieldname, char* value)
{
	char * result =
		get_field_string_from_identifier(USERNAME,
										 no_of_fields,
										 fieldname, value);
	return result;
}

char * get_piechart(int no_of_fields, char* fieldname, char* value)
{
	char * result = get_field_string_from_identifier(PIECHART,no_of_fields,
													 fieldname, value);
	return result;
}

int get_recur(int no_of_fields, char* fieldname, char* value)
{
	int period = -1;
	char * result = get_field_string_from_identifier(COMMAND_RECUR,
													 no_of_fields,
													 fieldname, value);
	to_lower(result);
	if (result != 0) {
		if (strcmp(result,get_text_from_identifier(PERIOD_DAILY))==0) {
			period = PERIOD_DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_2DAILY))==0) {
			period = PERIOD_2DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_3DAILY))==0) {
			period = PERIOD_3DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_4DAILY))==0) {
			period = PERIOD_4DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_5DAILY))==0) {
			period = PERIOD_5DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_6DAILY))==0) {
			period = PERIOD_6DAILY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_WEEKLY))==0) {
			period = PERIOD_WEEKLY;
		}
		if ((strcmp(result,get_text_from_identifier(PERIOD_BIWEEKLY))==0) ||
			(strcmp(result,get_text_from_identifier(PERIOD_2WEEKLY))==0)) {
			period = PERIOD_2WEEKLY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_MONTHLY))==0) {
			period = PERIOD_MONTHLY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_2MONTHLY))==0) {
			period = PERIOD_2MONTHLY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_3MONTHLY))==0) {
			period = PERIOD_3MONTHLY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_6MONTHLY))==0) {
			period = PERIOD_6MONTHLY;
		}
		if ((strcmp(result,get_text_from_identifier(PERIOD_YEARLY))==0) ||
			(strcmp(result,get_text_from_identifier(PERIOD_ANNUAL))==0) ||
			(strcmp(result,get_text_from_identifier(PERIOD_ANNUALLY))==0)) {
			period = PERIOD_YEARLY;
		}
		if (strcmp(result,get_text_from_identifier(PERIOD_2YEARLY))==0) {
			period = PERIOD_2YEARLY;
		}
	}
	return period;
}

/* If descriptions are longer than the maximum row length then
   show them on multiple lines */
void description_multiline(char * str, char column_separator,
						   int start_column)
{
	int i,j,max;
	int max_length = MAX_ROW_LENGTH-start_column-1;

	trim(str);

	if ((strlen(str)<max_length) || (column_separator!=' ')) {
		printf("%s%c",str,column_separator);
		return;
	}

	while (strlen(str)>0) {
		max = strlen(str);
		if (strlen(str)>=max_length) {
			i=0;
			while ((i<max_length) && (i<strlen(str))) {
				if (str[i]==' ') {
					max = i+1;
				}
				i++;
			}
		}
		i=0;
		while ((i<max_length) && (i<max)) {
			printf("%c",str[i]);
			i++;
		}
		if (i<strlen(str)) {
			for (j=i;j<strlen(str);j++) {
				str[j-i] = str[j];
			}
			str[j-i]=0;
			printf("%c\n", column_separator);
			for (j = 0; j <= start_column; j++) {
				printf(" ");
			}
		}
		else {
			str[0]=0;
		}

	}
	printf("%c",column_separator);
}
