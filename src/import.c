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

#include "import.h"

enum {
	FIELD_DATE=0,
	FIELD_AMOUNT,
	FIELD_SPEND,
	FIELD_RECEIVE,
	FIELD_DESCRIPTION,
	FIELD_STATUS,
	FIELD_CURRENCY,
	FIELD_TYPE,
	FIELD_TIME,
	FIELDS
};

static void parse_value(char * str, int * fields, int field_number,
						char * date, char * timestr,
						char * receive, char * spend,
						char * status, char * description,
						char * currency, char * field_type)
{
	int i,j;

	remove_eol(str);
	trim(str);

	for (i=0;i<FIELDS;i++) {
		if (fields[i]==field_number) {
			switch(i) {
			case FIELD_DATE: {
				sprintf(date,"%s",str);
				break;
			}
			case FIELD_AMOUNT: {
				if (atof(str)>=0) {
					sprintf(receive,"%s",str);
				}
				else {
					sprintf(spend,"%s",(char*)&str[1]);
				}
				break;
			}
			case FIELD_SPEND: {
				sprintf(spend,"%s",str);
				break;
			}
			case FIELD_RECEIVE: {
				sprintf(receive,"%s",str);
				break;
			}
			case FIELD_DESCRIPTION: {
				sprintf(description,"%s",str);
				break;
			}
			case FIELD_STATUS: {
				sprintf(status,"%s",str);
				break;
			}
			case FIELD_TIME: {
				sprintf(timestr,"%s",str);
				break;
			}
			case FIELD_TYPE: {
				sprintf(field_type,"%s",str);
				for (j = 0; j < strlen(field_type); j++) {
					field_type[j] = tolower(field_type[j]);
				}
				break;
			}
			case FIELD_CURRENCY: {
				sprintf(currency,"%s",str);
				/* currency should be three characters */
				if (strlen(currency)!=3) {
					currency[0]=0;
					break;
				}
				/* currency should be upper case */
				for (j = 0; j < strlen(currency); j++) {
					currency[j] = toupper(currency[j]);
				}
				break;
			}
			}
			break;
		}
	}
}

static void categorize_string(char * str, int * fields, int field_number)
{
	remove_eol(str);
	trim(str);

	if (strcmp(str,get_text_from_identifier(DATE))==0) {
		fields[FIELD_DATE] = field_number;
	}
	if ((strcmp(str,get_text_from_identifier(MONEY_IN))==0) ||
		(strcmp(str,get_text_from_identifier(CREDIT))==0) ||
		(strcmp(str,get_text_from_identifier(IMPORT_IN))==0) ||
		(strcmp(str,get_text_from_identifier(COMMAND_RECEIVED))==0)) {
		fields[FIELD_RECEIVE] = field_number;
	}
	if ((strcmp(str,get_text_from_identifier(MONEY_OUT))==0) ||
		(strcmp(str,get_text_from_identifier(DEBIT))==0) ||
		(strcmp(str,get_text_from_identifier(IMPORT_OUT))==0) ||
		(strcmp(str,get_text_from_identifier(COMMAND_SPENT))==0) ||
		(strcmp(str,get_text_from_identifier(COMMAND_SPEND))==0)) {
		fields[FIELD_SPEND] = field_number;
	}

	if (strcmp(str,get_text_from_identifier(AMOUNT))==0) {
		fields[FIELD_AMOUNT] = field_number;
	}

	if ((strcmp(str,get_text_from_identifier(MEMO))==0) ||
		(strcmp(str,get_text_from_identifier(IMPORT_DESCRIPTION))==0) ||
		(strcmp(str,get_text_from_identifier(IMPORT_NAME))==0) ||
		(strcmp(str,get_text_from_identifier(NOTES))==0)) {
		fields[FIELD_DESCRIPTION] = field_number;
	}
	if ((strcmp(str,get_text_from_identifier(SUBCATEGORY))==0) ||
		(strcmp(str,get_text_from_identifier(CATEGORY))==0) ||
		(strcmp(str,get_text_from_identifier(STATUS))==0)) {
		fields[FIELD_STATUS] = field_number;
	}
	if ((strcmp(str,get_text_from_identifier(CURRENCY))==0) ||
		(strcmp(str,get_text_from_identifier(CURRENCY_ABBREV))==0)) {
		fields[FIELD_CURRENCY] = field_number;
	}
	if (strcmp(str,get_text_from_identifier(IMPORT_TYPE))==0) {
		fields[FIELD_TYPE] = field_number;
	}
	if (strcmp(str,get_text_from_identifier(TIME))==0) {
		fields[FIELD_TIME] = field_number;
	}
}

static void parse_header(char * header, char separator, int * fields)
{
	int i,start=0,field_number=0;
	int reading_text=0;
	char str[STRING_BLOCK];

	for (i=0;i<FIELDS;i++) {
		fields[i]=-1;
	}

	for (i=0;i<strlen(header);i++) {
		/* ignore separators if they're inside quotes */
		if (header[i]=='"') {
			reading_text = 1-reading_text;
		}
		if ((reading_text==0) && (header[i]==separator)) {
			str[i-start]=0;
			categorize_string(str, fields, field_number);
			start=i+1;
			field_number++;
		}
		else {
			str[i-start] = tolower(header[i]);
		}
	}
	str[i-start]=0;
	categorize_string(str, fields, field_number);
}

static int valid_field_type(char * field_type)
{
	if (field_type[0]==0) return 1;
	/* ignore currency conversions - they add confusion */
	if (strcmp(field_type,get_text_from_identifier(CURRENCY_CONVERSION))==0) {
		return 0;
	}
	return 1;
}

static int parse_row(char * account, char * line, char separator, int * fields,
					 char * date, char * timestr,
					 char * receive, char * spend, char * status,
					 char * description, char * currency, char * field_type)
{
	int i,start=0,field_number=0;
	int reading_text=0;
	char str[STRING_BLOCK],date_str[STRING_BLOCK];
	char time_str[STRING_BLOCK];
	int transaction_added=0;
	char * date_format = settings_get_date_format();

	date[0]=0;
	timestr[0]=0;
	sprintf(receive,"%s","0");
	sprintf(spend,"%s","0");
	status[0]=0;
	description[0]=0;
	currency[0]=0;
	field_type[0]=0;

	for (i=0;i<strlen(line);i++) {
		/* ignore separators if they're inside quotes */
		if (line[i]=='"') {
			reading_text = 1-reading_text;
		}
		if ((reading_text==0) && (line[i]==separator)) {
			str[i-start]=0;
			parse_value(str, fields, field_number,
						date, timestr, receive, spend,
						status, description, currency, field_type);
			start=i+1;
			field_number++;
		}
		else {
			str[i-start] = line[i];
		}
	}
	str[i-start]=0;
	parse_value(str, fields, field_number,
				date, timestr, receive, spend,
				status, description, currency, field_type);

	if (valid_field_type(field_type)!=0) {
		/* does this look like a date? */
		if ((is_date(date, date_format)!=0) &&
			(atof(receive)+atof(spend)!=0)) {

			/* convert the date into a standard format */
			convert_date_to_standard(date, date_format, date_str);

			/* convert the time into a standard format */
			convert_time_to_standard(timestr, time_str,
									 get_text_from_identifier(TIME_AM),
									 get_text_from_identifier(TIME_PM));

			/* default currency if none specified */
			if (currency[0]==0) {
				sprintf(currency,"%s",settings_get_currency());
			}

			/* transaction doesn't already exist */
			if (database_transaction_exists(
											account,
											date_str,
											time_str,
											receive, spend,
											description,
											currency)==0) {

				/* add the transaction */
				if (database_save_short(account,
										date, time_str, date_format,
										status, description,
										currency,
										settings_get_vat_rate(),
										0, "",
										receive, spend)!=0) {
					transaction_added=1;
				}
			}
		}
	}
	return transaction_added;
}

/* extract data from an xml field */
static void extract_xml_field(char * line, char * result)
{
	int i,ctr=0;

	result[0]=0;

	/* find the beginning */
	for (i=0;i<strlen(line);i++) {
		if (line[i]=='>') break;
	}
	i++;
	/* update result until the end is reached */
	while (i<strlen(line)-1) {
		if ((line[i]=='<') && (line[i+1]=='/')) break;
		result[ctr++] = line[i];
		i++;
	}
	result[ctr]=0;
	trim(result);
}

/* parses a value of the type num/denom into a decimal string */
static void parse_gnucash_value(char * value, char * result)
{
	int i,state=0,ctr=0,zeroes=0;
	char numerator[128];

	for (i = 0; i < strlen(value); i++) {
		if (value[i]=='/') {
			/* terminate the numerator string */
			numerator[ctr]=0;
			state++;
			ctr = 0;
		}
		else {
			if (state==0) {
				/* update the numerator */
				numerator[ctr++] = value[i];
			}
			else {
				/* a simple hack is to count the zeros */
				if (value[i]=='0') zeroes++;
			}
		}
	}
	/* if no denominator was found */
	if (state==0) {
		sprintf(result,"%s",value);
		return;
	}
	/* copy the numerator to the result, inserting the decimal point */
	ctr=0;
	for (i = 0; i < strlen(numerator); i++) {
		result[ctr++] = numerator[i];
		/* insert the decimal at the appropriate place */
		if (i==strlen(numerator)-zeroes-1) {
			result[ctr++]='.';
		}
	}
	result[ctr]=0;
}

/* lists the gnucash accounts which have some transactions */
static int show_gnucash_accounts(char * filename)
{
	FILE * fp;
	char line[STRING_BLOCK];
	char account_name[STRING_BLOCK];
	char current_guid[STRING_BLOCK];
	char transaction_guid[STRING_BLOCK];
	char names[200][STRING_BLOCK];
	char guids[200][STRING_BLOCK];
	int hits[200];
	int i,j,no_of_accounts=0;

	fp = fopen(filename,"r");
	if (fp!=0) {
		while (!feof(fp)) {
			if (fgets(line , STRING_BLOCK-1 , fp) != NULL) {
				if (line!=NULL) {
					trim(line);
					/* name of the account */
					if (strstr(line,"<act:name>")!=0) {
						extract_xml_field(line,account_name);

						for (i=0;i<no_of_accounts;i++) {
							if (strcmp(account_name,(char*)&names[i])==0) {
								break;
							}
						}
						if (i==no_of_accounts) {
							for (j=0;j<strlen(account_name);j++) {
								names[i][j]=account_name[j];
							}
						}
					}
					/* guid for the account */
					if (strstr(line,"<act:id type=\"guid\">")!=0) {
						extract_xml_field(line,current_guid);

						for (i=0;i<no_of_accounts;i++) {
							if (strcmp(current_guid,(char*)guids[i])==0) {
								break;
							}
						}
						if (i==no_of_accounts) {
							for (j=0;j<strlen(current_guid);j++) {
								guids[i][j]=current_guid[j];
							}
							hits[i]=0;
							if (no_of_accounts<200) no_of_accounts++;
						}
					}
					/* guid for the transaction */
					if (strstr(line,"<split:account type=\"guid\">")!=0) {
						extract_xml_field(line,transaction_guid);

						for (i=0;i<no_of_accounts;i++) {
							if (strcmp(transaction_guid,(char*)guids[i])==0) {
								hits[i]++;
								break;
							}
						}
					}
				}
			}
		}
		fclose(fp);
	}

	for (i=0;i<no_of_accounts;i++) {
		if (hits[i]>0) {
			printf(" * %s\n",names[i]);
		}
	}
	return no_of_accounts;
}

/* returns the guid assocuated with the given account name */
static void get_gnucash_account_guid(char * filename,
									 char * gnucash_account_name,
									 char * guid,
									 int * assets_account)
{
	FILE * fp;
	char account_name[STRING_BLOCK];
	char current_guid[STRING_BLOCK];
	char line[STRING_BLOCK];
	int i;

	account_name[0]=0;
	current_guid[0]=0;
	guid[0]=0;
	*assets_account=0;

	fp = fopen(filename,"r");
	if (fp!=0) {
		while (!feof(fp)) {
			if (fgets(line , STRING_BLOCK-1 , fp) != NULL) {
				if (line!=NULL) {
					trim(line);
					/* asset account */
					if ((strstr(line,"<act:type>ASSET")!=0) ||
						(strstr(line,"<act:type>INCOME")!=0) ||
						(strstr(line,"<act:type>CREDIT")!=0) ||
						(strstr(line,"<act:type>CASH")!=0) ||
						(strstr(line,"<act:type>EQUITY")!=0) ||
						(strstr(line,"<act:type>BANK")!=0)) {
						*assets_account = 1;
					}
					if ((strstr(line,"<act:type>EXPENSE")!=0) ||
						(strstr(line,"<act:type>LIABILITY")!=0)){
						*assets_account = 0;
					}
					/* name of the account */
					if (strstr(line,"<act:name>")!=0) {
						extract_xml_field(line,account_name);
						for (i = 0; i < strlen(account_name); i++) {
							account_name[i] = tolower(account_name[i]);
						}
					}
					/* guid for the account */
					if (strstr(line,"<act:id type=\"guid\">")!=0) {
						extract_xml_field(line,current_guid);
					}
					/* end of the account record */
					if (strstr(line,"</gnc:account>")!=0) {
						if (account_name[0]!=0) {
							if (strcmp(account_name,
									   gnucash_account_name)==0) {
								sprintf(guid,"%s",current_guid);
								break;
							}
						}
						account_name[0]=0;
						current_guid[0]=0;
					}
				}
			}
		}
		fclose(fp);
	}
}

/* parses a single line of a gnucash xml file */
static int parse_gnucash_transaction(char * account,
									 char * line,
									 int * reading_record,
									 char * date,
									 char * timestr,
									 char * receive,
									 char * spend,
									 char * quantity,
									 char * description,
									 char * currency,
									 int * field_type,
									 int * value_found,
									 char * gnucash_account_guid,
									 int assets_account)
{
	char result[STRING_BLOCK],datestr[STRING_BLOCK];
	char guid[STRING_BLOCK];
	char * date_format;

	/* is the line blank? */
	if (strlen(line)==0) return 0;

	/* remove any spaces */
	trim(line);

	if (*reading_record==0) {
		if (strncmp(line,"<gnc:transaction",16)==0) {
			/* the beginning of a transaction */
			*reading_record = 1;
			/* clear fields */
			date[0]=0;
			timestr[0]=0;
			receive[0]=0;
			spend[0]=0;
			description[0]=0;
			currency[0]=0;
			*field_type=-1;
			*value_found = 0;
		}
	}
	else {
		if (strncmp(line,"</gnc:transaction",17)==0) {
			/* the end of a transaction */
			*reading_record = 0;
			if ((*value_found!=0) && (date[0]!=0)) {

				/* default currency if none specified */
				if (strlen(currency)!=3) {
					sprintf(currency,"%s",settings_get_currency());
				}

				/* transaction doesn't already exist */
				if (database_transaction_exists(account,
												date, timestr,
												receive, spend,
												description,
												currency)==0) {

					date_format = settings_get_date_format();
					convert_standard_to_date(date, date_format,
											 datestr);

					/* add the transaction */
					if (database_save_short(account,
											datestr, timestr,
											date_format,
											"", description,
											currency,
											settings_get_vat_rate(),
											0, "",
											receive, spend)!=0) {
						return 1;
					}
				}
			}
			return 0;
		}
		/* currency */
		if (strcmp(line,"<trn:currency>")==0) {
			*field_type = FIELD_CURRENCY;
			return 0;
		}
		if ((*field_type == FIELD_CURRENCY) &&
			(strstr(line,"<cmdty:id>")!=0)) {
			extract_xml_field(line,currency);
			*field_type = -1;
			return 0;
		}
		/* date */
		if (strcmp(line,"<trn:date-posted>")==0) {
			*field_type = FIELD_DATE;
			return 0;
		}
		if ((*field_type == FIELD_DATE) &&
			(strstr(line,"<ts:date>")!=0) &&
			(date[0]==0)) {
			/* gnucash dates already seem to be in the standard format
			   so don't need converting */
			extract_xml_field(line,date);
			if (strlen(date) > 10) {
				date[10]=0;
				strncpy(timestr,&date[11],8);
				timestr[8]=0;
			}
			else {
				date[10]=0;
				timestr[0]=0;
			}
			if (is_date(date,"%Y-%m-%d")==0) {
				date[0]=0;
			}
			*field_type = -1;
			return 0;
		}
		/* description */
		if (strstr(line,"<trn:description>")!=0) {
			extract_xml_field(line,description);
			*field_type = -1;
			return 0;
		}
		/* amount */
		if ((strstr(line,"<split:reconciled-state>")!=0) &&
			(*value_found==0)) {
			*field_type = FIELD_SPEND;
			return 0;
		}
		if ((*field_type == FIELD_SPEND) &&
			(strstr(line,"<split:value>")!=0)) {
			extract_xml_field(line,spend);
			return 0;
		}
		if ((*field_type == FIELD_SPEND) &&
			(strstr(line,"<split:account type=\"guid\">")!=0)) {
			extract_xml_field(line,guid);

			/* quantity and value usually appear to be the same */
			if (strcmp(guid, gnucash_account_guid)==0) {
				if (strlen(spend) > 0) {
					if (spend[0] != '-') {
						if (assets_account != 0) {
							sprintf(receive, "%s", spend);
							spend[0] = '0';
							spend[1] = 0;
						}
						else {
							receive[0] = '0';
							receive[1] = 0;
						}
					}
					else {
						spend[0] = ' ';
						if (assets_account != 0) {
							trim(spend);
							receive[0] = '0';
							receive[1] = 0;
						}
						else {
							sprintf(receive, "%s", spend);
							trim(receive);
							spend[0] = '0';
							spend[1] = 0;
						}
					}

					parse_gnucash_value(receive, result);
					sprintf(receive,"%s",result);

					parse_gnucash_value(spend, result);
					sprintf(spend,"%s",result);

					*value_found=1;
				}
				else {
					receive[0]=0;
					spend[0]=0;
				}
			}
			*field_type = -1;
			return 0;
		}
	}

	return 0;
}

static void	get_gnucash_account_name(char * name)
{
	int i;

	name[0]=0;
	printf("%s",get_text_from_identifier(GNUCASH_ACCOUNT_NAME));
	if (fgets(name, STRING_BLOCK, stdin)==0) return;

	if (strlen(name)==0) return;

	/* convert to lower case */
	for (i=0;i<strlen(name);i++) {
		name[i] = tolower(name[i]);
	}
	trim(name);
}

static int command_import_gnucash(char * account,
								  char * import_filename)
{
	FILE * fp;
	char line[STRING_BLOCK];
	char date[STRING_BLOCK];
	char time[STRING_BLOCK];
	char receive[STRING_BLOCK];
	char spend[STRING_BLOCK];
	char quantity[STRING_BLOCK];
	char description[STRING_BLOCK];
	char currency[STRING_BLOCK];
	int transactions_added = 0;
	int reading_record = 0;
	int field_type = 0;
	char zipped_filename[STRING_BLOCK];
	char extracted_filename[STRING_BLOCK];
	char gnucash_directory[STRING_BLOCK];
	char gnucash_account_name[STRING_BLOCK];
	char gnucash_account_guid[STRING_BLOCK];
	int assets_account=1;
	int value_found=0;

	sprintf(gnucash_directory,"%s/.fin/gnucash",getenv("HOME"));

	/* delete the previous file if it exists */
	sprintf(extracted_filename,"%s/import.xml",gnucash_directory);
	if (file_exists(extracted_filename) != 0) {
		sprintf(line,"rm %s",extracted_filename);
		if (system(line)!=0) {

		}
	}
	else {
		/* create a directory to extract data */
		if (directory_exists(gnucash_directory) == 0) {
			sprintf(line,"mkdir %s",gnucash_directory);
			if (system(line)!=0) {
			}
		}
	}

	/* unzip the file */
	sprintf(zipped_filename,"%s/import.xml.gz",gnucash_directory);
	sprintf(line,"cp %s %s",import_filename,zipped_filename);
	if (system(line)!=0) return 0;
	sprintf(line,"gunzip -f %s",zipped_filename);
	if (system(line)!=0) return 0;

	if (show_gnucash_accounts(extracted_filename)==0) return 0;

	get_gnucash_account_name(gnucash_account_name);

	if (strlen(gnucash_account_name)==0) return 0;

	get_gnucash_account_guid(extracted_filename,
							 gnucash_account_name,
							 gnucash_account_guid,
							 &assets_account);

	if (gnucash_account_guid[0]==0) {
		/* no account found with that name */
		return 0;
	}

	fp = fopen(extracted_filename,"r");
	if (fp!=0) {
		while (!feof(fp)) {
			if (fgets(line , STRING_BLOCK-1 , fp) != NULL) {
				if (parse_gnucash_transaction(account,
											  line, &reading_record,
											  (char*)date,
											  (char*)time,
											  (char*)receive,
											  (char*)spend,
											  (char*)quantity,
											  (char*)description,
											  (char*)currency,
											  &field_type,
											  &value_found,
											  gnucash_account_guid,
											  assets_account)!=0) {
					transactions_added++;
				}
			}
		}
	}
	return transactions_added;
}

static int command_import_csv(char * account,
							  char * import_filename,
							  char separator)
{
	FILE * fp;
	char line[STRING_BLOCK];
	int row=0;
	int fields[FIELDS];
	char date[STRING_BLOCK];
	char timestr[STRING_BLOCK];
	char receive[STRING_BLOCK];
	char spend[STRING_BLOCK];
	char status[STRING_BLOCK];
	char description[STRING_BLOCK];
	char currency[STRING_BLOCK];
	char field_type[STRING_BLOCK];
	int transactions_added = 0;

	fp = fopen(import_filename,"r");
	if (fp!=0) {
		while (!feof(fp)) {
			if (fgets(line , STRING_BLOCK-1 , fp) != NULL ) {
				if (row==0) {
					parse_header(line, separator, (int*)fields);
				}
				else {
					if (parse_row(account, line, separator, (int*)fields,
								  (char *) date, (char *) timestr,
								  (char *) receive,
								  (char *) spend, (char *) status,
								  (char *) description, (char *) currency,
								  (char *) field_type)!=0) {
						transactions_added++;
					}
				}
				row++;
			}
		}
		fclose(fp);
	}
	return transactions_added;
}

static int no_of_separators(char separator, char * str)
{
	int i,ctr=0;
	for (i=0;i<strlen(str);i++) {
		if (str[i]==separator) {
			ctr++;
		}
	}
	return ctr;
}

static int command_import_emacs(char * account,
								char * import_filename,
								char separator)
{
	FILE * fp;
	char line[STRING_BLOCK];
	int row=0;
	int fields[FIELDS];
	char date[STRING_BLOCK];
	char timestr[STRING_BLOCK];
	char receive[STRING_BLOCK];
	char spend[STRING_BLOCK];
	char status[STRING_BLOCK];
	char description[STRING_BLOCK];
	char currency[STRING_BLOCK];
	char field_type[STRING_BLOCK];
	int transactions_added = 0;

	fp = fopen(import_filename,"r");
	if (fp!=0) {
		while (!feof(fp)) {
			if (fgets(line , STRING_BLOCK-1 , fp) != NULL ) {
				if (no_of_separators(separator,line)>2) {
					printf("%s",line);
					if (row==0) {
						parse_header(line, separator, (int*)fields);
					}
					else {
						if (parse_row(account, line,
									  separator, (int*)fields,
									  (char *) date, (char *)timestr,
									  (char *) receive,
									  (char *) spend, (char *) status,
									  (char *) description,
									  (char *) currency,
									  (char *) field_type)!=0) {
							transactions_added++;
						}
					}
					row++;
				}
				else {
					if ((transactions_added>0) || (row>1)) {
						break;
					}
				}
			}
		}
		fclose(fp);
	}
	return transactions_added;
}

int command_import(char * account, char * import_filename, char separator)
{
	if ((strstr(import_filename,".csv")!=0) ||
		(strstr(import_filename,".CSV")!=0)) {
		return command_import_csv(account, import_filename, separator);
	}
	if (strstr(import_filename,".gnucash")!=0) {
		return command_import_gnucash(account, import_filename);
	}
	return command_import_emacs(account, import_filename, '|');
}
