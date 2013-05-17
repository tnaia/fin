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

#include "piechart.h"

static int parse_search_string(char * search_string, char search_separator,
							   char field_separator, int start_index,
							   char * search, char * fieldname)
{
	int i = start_index;
	int j = 0, k = 0;
	int reading_fieldname = 0;

	while (i < strlen(search_string)) {
		if (search_string[i] != search_separator) {
			if (search_string[i] != field_separator) {
				if (reading_fieldname==0) {
					search[j++] = search_string[i];
				}
				else {
					fieldname[k++] = search_string[i];
				}
			}
			else {
				reading_fieldname = 1;
			}
		}
		else {
			break;
		}
		i++;
	}
	search[j]=0;
	fieldname[k]=0;
	return i+1;
}

int calc_piechart(char * account, char * search_string,
				  int month_number, int year,
				  char field_separator, char * currency,
				  int spending,
				  char ** fieldname, double * value,
				  double * total_value,
				  int max_fields,
				  int include_zeros)
{
	double total_receive=0, total_spend=0;
	int start_index = 0,i,j;
	char search[STRING_BLOCK];
	char name[STRING_BLOCK];
	int field_ctr=0;

	/* perform multiple searches */
	*total_value = 0;
	while (start_index < strlen(search_string)) {
		start_index = parse_search_string(search_string, ',', '|',
										  start_index, search,
										  name);
    
		get_totals(account, search, month_number,
				   year, currency,
				   &total_receive, &total_spend);

		/* set the value */
		if (spending!=0) {
			value[field_ctr] = total_spend;
		}
		else {
			value[field_ctr] = total_receive;
		}
		*total_value += value[field_ctr];

		/* set the field name */
		fieldname[field_ctr] = (char*)malloc(STRING_BLOCK);
		fieldname[field_ctr][0]=0;

		if (name[0] != 0) {
			for (i = 0; i < strlen(name); i++) {
				fieldname[field_ctr][i] = name[i];
			}
			fieldname[field_ctr][i] = 0;
		}
		else {
			j = 0;
			for (i = 0; i < strlen(search); i++) {
				if ((search[i] != '*') && (search[i] != '%')) {
					fieldname[field_ctr][j++] = search[i];
				}
			}
			fieldname[field_ctr][j] = 0;
		}
		if (fieldname[field_ctr][0] != 0) {
			fieldname[field_ctr][0] = toupper(fieldname[field_ctr][0]);
		}

		if ((include_zeros != 0) ||
			((include_zeros == 0) && (value[field_ctr] > 0))) {
			field_ctr++;
			if (field_ctr >= max_fields) break;
		}
	}

	return field_ctr;
}

int piechart(char * title, char * account, char * search_string,
			 int month_number, int year,
			 char field_separator, char column_separator,
			 char * currency,
			 int spending,
			 char * export_filename)
{
	double total_income=0, total_outgoings=0;
	double other_value=0, running_total=0;
	int i,j;
	char fieldname[STRING_BLOCK];
	char str[STRING_BLOCK];
	double value[MAX_PIECHART_FIELDS],total_value=0;
	char * field[MAX_PIECHART_FIELDS];
	int valid_filename,max_length=10;
	int no_of_fields,first_column_length;
	int second_column_length;	
	FILE * fp;

	no_of_fields = calc_piechart(account, search_string,
								 month_number, year,
								 field_separator, currency,
								 spending,
								 (char**)field, (double*)value,
								 &total_value,
								 MAX_PIECHART_FIELDS,1);

	if (no_of_fields == 0) {
		return 0;
	}

	/* maximum field name length */
	max_length=1;
	for (i = 0; i < no_of_fields; i++) {
		j = 0;
		while (field[i][j] != 0) {
			fieldname[j] = field[i][j];
			j++;
			if (j > max_length) {
				max_length = j;
			}
		}
	}
	first_column_length = strlen(get_text_from_identifier(CATEGORY));
	second_column_length = strlen(get_text_from_identifier(PERCENT));
	if (max_length < first_column_length) {
		max_length = first_column_length;
	}

	/* get the total income for the period */
	sprintf(str,"%s","*");
	get_totals(account, str,
			   month_number, year,
			   currency,
			   &total_income, &total_outgoings);
	if (spending != 0) {
		if (total_income > total_value) {
			total_value = total_income;
		}
	}
	else {
		if (total_outgoings > total_value) {
			total_value = total_outgoings;
		}
	}

	/* running total percent */
	running_total = 0;
	for (i = 0; i < no_of_fields; i++) {
		running_total += value[i]*100/total_value;
	}

	valid_filename = 0;
	if (export_filename!=0) {
		if (strlen(export_filename)>0) {
			valid_filename = 1;
		}
	}
	
	if (valid_filename==0) {
		/* write to console */
		if (title!=0) {
			printf("%s\n",title);
		}
	   
		sprintf(fieldname,"%s",get_text_from_identifier(CATEGORY));
		pad_text(fieldname,max_length+2);
		fieldname[0]=toupper(fieldname[0]);
		if (column_separator!=' ') {
			printf("%c %s",column_separator,fieldname);
		}
		else {
			printf("%s",fieldname);
		}
    
		sprintf(fieldname,"%s",get_text_from_identifier(PERCENT));
		fieldname[0]=toupper(fieldname[0]);
		if (column_separator!=' ') {
			printf("%c %s %c\n",column_separator,fieldname,column_separator);
			printf("%c",column_separator);
			for (i=0;i<first_column_length+3;i++) {
				printf("-");
			}
			printf("+");
			for (i=0;i<second_column_length+2;i++) {
				printf("-");
			}
			printf("|\n");
		}
		else {
			printf("%s\n",fieldname);

			for (i=0;i<max_length+10;i++) {
				printf("-");
			}
			printf("\n");
		}


		for (i = 0; i < no_of_fields; i++) {
			j = 0;
			while (field[i][j] != 0) {
				fieldname[j] = field[i][j];
				j++;
			}
			fieldname[j] = 0;

			pad_text(fieldname,max_length+2);

			if (column_separator!=' ') {
				printf("%c %s",column_separator,fieldname);
			}
			else {
				printf("%s",fieldname);
			}
    
			if (value[i]*100/total_value<10) {
				printf(" ");
			}
			if (column_separator!=' ') {
				printf("%c %.1f%c   %c\n",column_separator,
					   value[i]*100/total_value,'%',
					   column_separator);
			}
			else {
				printf("%.1f%c\n",value[i]*100/total_value,'%');
			}
		}

		/* 'other' category */
		if (running_total < 100) {
			sprintf(fieldname,"%s",get_text_from_identifier(CATEGORY_OTHER));
			pad_text(fieldname, max_length+2);
			if (column_separator!=' ') {
				printf("%c %s",column_separator,fieldname);
			}
			else {
				printf("%s",fieldname);
			}
			other_value = 100 - running_total;
			if (other_value < 10) {
				printf(" ");
			}
			if (column_separator!=' ') {
				printf("%c %.1f%c   %c\n",column_separator,
					   other_value,'%',
					   column_separator);
			}
			else {
				printf("%.1f%c\n",other_value,'%');
			}
		}
	}
	else {
		fp = fopen(export_filename,"w");
		if (fp!=0) {
			if (title!=0) {
				fprintf(fp,"%s\n",title);
			}

			sprintf(fieldname,"%s",get_text_from_identifier(CATEGORY));
			fieldname[0]=toupper(fieldname[0]);
			fprintf(fp,"%s%c",fieldname,field_separator);
    
			sprintf(fieldname,"%s",get_text_from_identifier(PERCENT));
			fieldname[0]=toupper(fieldname[0]);
			fprintf(fp,"%s\n",fieldname);

			for (i = 0; i < no_of_fields; i++) {
				j = 0;
				while (field[i][j] != 0) {
					fieldname[j] = field[i][j];
					j++;
				}
				fieldname[j] = 0;

				fprintf(fp,"%s%c",fieldname,field_separator);
    
				fprintf(fp,"%.4f\n",value[i]*100/total_value);
			}

			if (running_total<100) {
				fprintf(fp,"%s%c",
						get_text_from_identifier(CATEGORY_OTHER),
						field_separator);
				fprintf(fp,"%.4f\n",100-running_total);
			}

			fclose(fp);
			printf(get_text_from_identifier(EXPORT_COMPLETED),
				   export_filename);
			printf("\n");
		}
	}

	/* free allocated strings */
	for (i=0;i<no_of_fields;i++) {
		free(field[i]);
		field[i]=0;
	}

	return 1;
}


