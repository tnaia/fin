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

#include "settings.h"

int settings_loaded=0;
char currency[STRING_BLOCK];
char account[STRING_BLOCK];
char VATrate[STRING_BLOCK];
char server[STRING_BLOCK];
char date_format[STRING_BLOCK];
char language[STRING_BLOCK];
char theme[STRING_BLOCK];

int settings_load(char * directory)
{
	FILE * fp;
	char filename[STRING_BLOCK];
	char line[STRING_BLOCK];
	int i,j,k,length;
	char fieldname[STRING_BLOCK];
	char value[STRING_BLOCK];

	sprintf((char*)filename,"%s/%s",directory,
			get_text_from_identifier(SETTINGS_FILENAME));
	fp  =fopen(filename,"r");
	if (fp!=0L) {
		while (!feof(fp)) {
			if (fgets(line , 255 , fp) != NULL ) {
				length = strlen((char*)line);
				fieldname[0]=0;
				value[0]=0;

				for (i=0;i<length;i++) {
					if ((line[i]==':') || (line[i]=='=')) {
						k=0;
						j=0;
						while ((j<length) &&
							   ((line[j]==' ') || (line[j]==9))) {
							j++;
						}
						while (j<i) {
							fieldname[k++] = line[j++];
						}
						fieldname[k]=0;
						break;
					}
				}
				if (i<length) {
					i++;
					while (((line[i]==' ') ||
							(line[i]==9)) && (i<length)) {
						i++;
					}
					j=0;
					while (i<length) {
						if ((line[i]!=10) && (line[i]!=13)) {
							value[j++]=line[i];
						}
						i++;
					}
					value[j]=0;
				}

				if ((fieldname[0]!=0) && (value[0]!=0)) {
					i = strlen((char*)fieldname)-1;
					while ((i>0) && ((fieldname[i]==' ') ||
									 (fieldname[i]==9))) {
						i--;
					}
					fieldname[i+1]=0;

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_ACCOUNT))==0) {
						sprintf((char*)account,"%s",(char*)value);
					}

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_CURRENCY))==0) {
						sprintf((char*)currency,"%s",(char*)value);
					}

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_VAT_RATE))==0) {
						sprintf((char*)VATrate,"%s",(char*)value);
					}

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_SERVER))==0) {
						sprintf((char*)server,"%s",(char*)value);
					}

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_DATE_FORMAT))==0) {
						sprintf((char*)date_format,"%s",(char*)value);
					}

					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_LANGUAGE))==0) {
						sprintf((char*)language,"%s",(char*)value);
					}
					if (strcmp((char*)fieldname,
							   get_text_from_identifier(SETTINGS_THEME))==0) {
						sprintf((char*)theme,"%s",(char*)value);
					}

				}
			}
		}
		fclose(fp);
		return 1;
		settings_loaded=1;
	}
	return 0;
}

void settings_save(char * directory)
{
	FILE * fp;
	char filename[STRING_BLOCK];

	sprintf((char*)filename,"%s/%s",
			directory,get_text_from_identifier(SETTINGS_FILENAME));
	fp  =fopen(filename,"w");
	if (fp!=0L) {
		if (strlen(account)==0) {
			sprintf((char*)account,"%s",
					get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
		}
		if (strlen(currency)==0) {
			sprintf((char*)currency,"%s",
					get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY));
		}
		if (strlen(VATrate)==0) {
			sprintf((char*)VATrate,"%s",
					get_text_from_identifier(SETTINGS_DEFAULT_VAT_RATE));
		}
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_ACCOUNT),
				(char*)account);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_CURRENCY),
				(char*)currency);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_VAT_RATE),
				(char*)VATrate);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_SERVER),
				(char*)server);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_DATE_FORMAT),
				(char*)date_format);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_LANGUAGE),
				(char*)language);
		fprintf(fp,"%s: %s\n",
				get_text_from_identifier(SETTINGS_THEME),
				(char*)theme);
		fclose(fp);
		settings_loaded=1;
	}
}

char * settings_get_currency()
{
	return (char*)currency;
}

void settings_set_currency(char * curr)
{
	sprintf((char*)currency,"%s",curr);
}

char * settings_get_account()
{
	return (char*)account;
}

void settings_set_account(char * acc)
{
	sprintf((char*)account,"%s",acc);
}

char * settings_get_vat_rate()
{
	return (char*)VATrate;
}

void settings_set_vat_rate(char * vat)
{
	sprintf((char*)VATrate,"%s",vat);
}

char * settings_get_server()
{
	return (char*)server;
}

void settings_set_server(char * srv)
{
	sprintf((char*)server,"%s",srv);
}

char * settings_get_date_format()
{
	int i;
	for (i=0;i<strlen(date_format);i++) {
		if (date_format[i]=='y') date_format[i]='Y';
	}
	return (char*)date_format;
}

void settings_set_date_format(char * format)
{
	int i;
	sprintf((char*)date_format,"%s",format);
	for (i=0;i<strlen(date_format);i++) {
		if (date_format[i]=='y') date_format[i]='Y';
	}
}

char * settings_get_language()
{
	return (char*)language;
}

void settings_set_language(char * srv)
{
	sprintf((char*)language,"%s",srv);
}

char * settings_get_theme()
{
	return (char*)theme;
}

void settings_set_theme(char * theme_name)
{
	int i;

	sprintf((char*)theme,"%s",theme_name);

	/* convert to lower case */
	for (i = 0; i < strlen(theme); i++) {
		theme[i] = tolower(theme[i]);
	}
}

char * settings_get_theme_colour(int category)
{
	/* normal/default */
	if ((strcmp(theme,get_text_from_identifier(THEME_NORMAL))==0) ||
		(strcmp(theme,get_text_from_identifier(THEME_DEFAULT))==0)) {
		return "";
	}

	/* dark theme */
	if (strcmp(theme,get_text_from_identifier(THEME_DARK))==0) {
		switch(category) {
		case COLOUR_POSITIVE: { return GREEN; }
		case COLOUR_NEGATIVE: { return CYAN; }
		case COLOUR_BEFORE: { return GREEN; }
		case COLOUR_AFTER: { return YELLOW; }
		case COLOUR_DELETE: { return RED; }
		}
	}

	/* light theme */
	if (strcmp(theme,get_text_from_identifier(THEME_LIGHT))==0) {
		switch(category) {
		case COLOUR_POSITIVE: { return BLUE; }
		case COLOUR_NEGATIVE: { return BLACK; }
		case COLOUR_BEFORE: { return GREEN; }
		case COLOUR_AFTER: { return BLUE; }
		case COLOUR_DELETE: { return RED; }
		}
	}

	/* blue theme */
	if (strcmp(theme,get_text_from_identifier(THEME_BLUE))==0) {
		switch(category) {
		case COLOUR_POSITIVE: { return CYAN; }
		case COLOUR_NEGATIVE: { return BLUE; }
		case COLOUR_BEFORE: { return GREEN; }
		case COLOUR_AFTER: { return BLUE; }
		case COLOUR_DELETE: { return RED; }
		}
	}

	/* green theme */
	if (strcmp(theme,get_text_from_identifier(THEME_GREEN))==0) {
		return GREEN;
	}

	/* black theme */
	if (strcmp(theme,get_text_from_identifier(THEME_BLACK))==0) {
		return BLACK;
	}

	/* white theme */
	if (strcmp(theme,get_text_from_identifier(THEME_WHITE))==0) {
		return WHITE;
	}

	return GREEN;
}

/* returns a non-zero value if the given theme is valid */
int settings_valid_theme(char * theme_name)
{
	int i;

	/* convert to lower case */
	for (i = 0; i < strlen(theme_name); i++) {
		theme_name[i] = tolower(theme_name[i]);
	}

	if ((strcmp(theme_name,get_text_from_identifier(THEME_NORMAL))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_DEFAULT))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_DARK))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_LIGHT))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_GREEN))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_BLACK))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_BLUE))==0) ||
		(strcmp(theme_name,get_text_from_identifier(THEME_WHITE))==0)) {
		return 1;
	}
	return 0;
}
