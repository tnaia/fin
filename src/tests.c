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

#include "tests.h"

static int same_string(char * str1, char * str2)
{
	int i;

	if (strlen(str1)!=strlen(str2)) return 0;
	for (i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i]) return 0;
	}
	return 1;
}

static int test_get_text()
{
	int result;

	printf("test_get_text...");

	result = strcmp(get_text_from_identifier(SPEAK),"speak");
	assert(result==0);

	result = strcmp(get_text_from_identifier(NUMBER),"number");
	assert(result==0);

	result = strcmp(get_text_from_identifier(DATE),"date");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(COMMAND_RECEIVED),
			   "received");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(COMMAND_RECUR),
			   "recur");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(PERIOD_2YEARLY),
			   "2yearly");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(WARNING_NO_SERVER),
			   "No server has been specified");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(KML),
			   "kml");
	assert(result==0);

	result = strcmp(get_text_from_identifier(ACCOUNT),
					"account");
	assert(result==0);

	result =
		strcmp(get_text_from_identifier(STYLE_ORGMODE),
			   "org-mode");
	assert(result==0);

	printf("ok\n");
	return 1;
}

static int test_parse()
{
	int argc = 4;
	char * argv[] = {
		"arg0", "arg1", "account:personal", "arg3"
	};

	char fieldname[32*STRING_BLOCK];
	char value[32*STRING_BLOCK];
	int no_of_fields, result;

	printf("test_parse...");

	no_of_fields = parse(argc, argv, (char*)fieldname,
						 (char*)value, ':', 0, 32);

	result =
		strcmp(get_field_string_from_identifier(ACCOUNT,no_of_fields,
												(char*)fieldname,
												(char*)value),
			   "personal");

	assert(result==0);

	printf("ok\n");

	return 1;
}

static int test_is_integer()
{
	char str[20];

	printf("test_is_integer...");
	sprintf(str,"%s","123");
	assert(is_integer(str)!=0);
	sprintf(str,"%s","abc");
	assert(is_integer(str)==0);
	sprintf(str,"%s","123abc");
	assert(is_integer(str)==0);
	sprintf(str,"%s","123.456");
	assert(is_integer(str)==0);
	printf("ok\n");
	return 1;
}

static int test_is_value()
{
	char str[20];

	printf("test_is_value...");
	sprintf(str,"%s","123");
	assert(is_value(str)!=0);
	sprintf(str,"%s","abc");
	assert(is_value(str)==0);
	sprintf(str,"%s","123abc");
	assert(is_value(str)==0);
	sprintf(str,"%s","123.456");
	assert(is_value(str)!=0);
	printf("ok\n");
	return 1;
}

static int test_is_money()
{
	char str[20],curr[20];

	printf("test_is_money...");
	sprintf(str,"%s","$123");
	assert(is_money(str,curr)!=0);
	sprintf(str,"%s","$123.2.4");
	assert(is_money(str,curr)==0);
	sprintf(str,"%s","$123.2");
	assert(is_money(str,curr)!=0);
	sprintf(str,"%s","$1A3.2");
	assert(is_money(str,curr)==0);
	printf("ok\n");
	return 1;
}

static int test_is_date()
{
	char str[20],date_format[20];

	printf("test_is_date...");

	sprintf(date_format,"%s","%d/%m/%Y");
	sprintf(str,"%s","$123");
	assert(is_date(str,date_format)==0);
	sprintf(str,"%s","02/05/2016");
	assert(is_date(str,date_format)!=0);
	sprintf(str,"%s","17/13/2018");
	assert(is_date(str,date_format)==0);
	sprintf(str,"%s","2008/21/24");
	assert(is_date(str,date_format)==0);

	sprintf(date_format,"%s","%m/%d/%Y");
	sprintf(str,"%s","02/09/2014");
	assert(is_date(str,date_format)!=0);
	sprintf(str,"%s","15/09/2014");
	assert(is_date(str,date_format)==0);
	sprintf(str,"%s","02/07/20");
	assert(is_date(str,date_format)==0);
	sprintf(str,"%s","02/09/5123");
	assert(is_date(str,date_format)==0);

	printf("ok\n");
	return 1;
}

static int test_remove_currency_symbol()
{
	char original[20],result[20];
	int i;

	printf("test_remove_currency_symbol...");
	sprintf(original,"%s","$428");
	sprintf(result,"%s",original);
	assert(strlen(original)==strlen(result));
	remove_currency_symbol(original);
	assert(strlen(original)==strlen(result)-1);
	for (i=0;i<strlen(original);i++) {
		assert(original[i]==result[i+1]);
	}
	printf("ok\n");
	return 1;
}

static int test_valid_language()
{
	printf("test_valid_language...");
	assert(valid_language("english uk")!=0);
	assert(valid_language("klingon")==0);
	assert(valid_language("russian")!=0);
	printf("ok\n");
	return 1;
}

static int test_trim()
{
	char str[20];

	printf("test_trim...");
	sprintf(str,"%s","  test string   ");
	trim(str);
	assert(str[0]=='t');
	assert(str[strlen(str)-1]=='g');
	printf("ok\n");
	return 1;
}

static int test_remove_eol()
{
	char str[20];
	int i;

	printf("test_remove_eol...");
	sprintf(str,"%s","test string");
	str[11]=13;
	str[12]=0;
	remove_eol(str);
	assert(strlen(str)==11);
	for (i=0;i<strlen(str);i++) {
		assert(str[i]!=13);
	}
	printf("ok\n");
	return 1;
}

static int test_money_to_integer()
{
	char money[20];
	char value[20];
	int i;

	printf("test_money_to_integer...");
	sprintf(money,"%s","12.95");
	money_to_integer((char*)money,value);
	assert(value[0]=='1');
	assert(value[1]=='2');
	assert(value[2]=='9');
	assert(value[3]=='5');
	assert(strlen(value)-2==DECIMAL_PLACES);
	for (i=4;i<strlen(value);i++) {
		assert(value[i]=='0');
	}
	printf("ok\n");
	return 1;
}

static int test_integer_to_money()
{
	char money[20];
	char value[20];
	int i;

	printf("test_integer_to_money...");
	sprintf(value,"%s","1295");
	for (i=0;i<DECIMAL_PLACES-2;i++) {
		value[4+i] = '0';
	}
	value[4+i] = 0;

	integer_to_money(value,money);
	assert(money[0]=='1');
	assert(money[1]=='2');
	assert(money[2]=='.');
	assert(money[3]=='9');
	assert(money[4]=='5');
	for (i=5;i<5 + DECIMAL_PLACES - 2;i++) {
		assert(money[i]=='0');
	}
	printf("ok\n");
	return 1;
}

static int test_quantity_to_integer()
{
	char qty[20];
	char value[20];
	int i;

	printf("test_quantity_to_integer...");
	sprintf(qty,"%s","12.95");
	quantity_to_integer((char*)qty,value);
	assert(value[0]=='1');
	assert(value[1]=='2');
	assert(value[2]=='9');
	assert(value[3]=='5');
	assert(strlen(value)-2==DECIMAL_PLACES_QUANTITY);
	for (i=4;i<strlen(value);i++) {
		assert(value[i]=='0');
	}
	printf("ok\n");
	return 1;
}

static int test_integer_to_quantity()
{
	char qty[20];
	char value[20];
	int i;

	printf("test_integer_to_quantity...");
	sprintf(value,"%s","1295");
	for (i=0;i<DECIMAL_PLACES-2;i++) {
		value[4+i] = '0';
	}
	value[4+i] = 0;

	integer_to_quantity(value,qty);
	assert(qty[0]=='1');
	assert(qty[1]=='2');
	assert(qty[2]=='.');
	assert(qty[3]=='9');
	assert(qty[4]=='5');
	for (i=5;i<5 + DECIMAL_PLACES_QUANTITY - 2;i++) {
		assert(qty[i]=='0');
	}
	printf("ok\n");
	return 1;
}

static int test_pad_text()
{
	char str[20];
	int i;

	printf("test_pad_text...");
	sprintf(str,"%s","text");
	assert(strlen(str)==4);
	pad_text(str,16);
	assert(strlen(str)==16);
	for (i=5;i<16;i++) {
		assert(str[i]==' ');
	}
	printf("ok\n");
	return 1;
}

static int test_pad_value()
{
	char str[30], padded[30];
	int i;
	const int leading_spaces=8;
	const int trailing_zeros=5;

	printf("test_pad_value...");
	sprintf(str,"%s","7.35");
	assert(strlen(str)==4);
	pad_value(str,padded,leading_spaces,trailing_zeros);
	assert(strlen(padded)==leading_spaces+trailing_zeros);
	for (i=0;i<leading_spaces-2;i++) {
		assert(padded[i]==' ');
	}
	for (i=leading_spaces+2;i<leading_spaces+trailing_zeros;i++) {
		assert(padded[i]=='0');
	}
	printf("ok\n");
	return 1;
}

static int test_convert_date_to_standard()
{
	char datestr[20],date_format[20];
	char converted[20];

	printf("test_convert_date_to_standard...");

	sprintf(date_format,"%s","%d/%m/%Y");
	sprintf(datestr,"%s","20/05/2017");
	convert_date_to_standard(datestr,date_format,converted);
	assert(strlen(converted)==10);
	assert(converted[0]=='2');
	assert(converted[1]=='0');
	assert(converted[2]=='1');
	assert(converted[3]=='7');
	assert(converted[4]=='-');
	assert(converted[5]=='0');
	assert(converted[6]=='5');
	assert(converted[7]=='-');
	assert(converted[8]=='2');
	assert(converted[9]=='0');
	assert(converted[10]==0);
	printf("ok\n");
	return 1;
}

static int test_convert_standard_to_date()
{
	char datestr[20],date_format[20];
	char standard[20];

	printf("test_convert_standard_to_date...");

	sprintf(date_format,"%s","%d/%m/%Y");
	sprintf(standard,"%s","2017-05-20");
	convert_standard_to_date(standard,date_format,datestr);
	assert(strlen(datestr)==10);
	assert(datestr[0]=='2');
	assert(datestr[1]=='0');
	assert(datestr[2]=='/');
	assert(datestr[3]=='0');
	assert(datestr[4]=='5');
	assert(datestr[5]=='/');
	assert(datestr[6]=='2');
	assert(datestr[7]=='0');
	assert(datestr[8]=='1');
	assert(datestr[9]=='7');
	assert(datestr[10]==0);
	printf("ok\n");
	return 1;
}

static int test_reverse_file()
{
	char filename[STRING_BLOCK];
	int i;
	char line[STRING_BLOCK];
	FILE * fp;

	printf("test_reverse_file...");
	sprintf(filename,"%s","unit_test_reverse_file.txt");
	fp = fopen(filename,"w");
	assert(fp!=NULL);
	for (i=1;i<=5;i++) {
		fprintf(fp,"%d\n",i);
	}
	fclose(fp);
	reverse_file(filename);
	fp = fopen(filename,"r");
	assert(fp!=NULL);
	for (i=1;i<=5;i++) {
		if (fgets(line , 255 , fp) != NULL) {
			if (i==1) {
				assert(atoi(line)==i);
			}
			else {
				assert(atoi(line)==5-(i-2));
			}
		}
	}
	fclose(fp);
	sprintf(line,"rm %s", filename);
	assert(system(line)==0);
	printf("ok\n");
	return 1;
}

static int test_trim_file_header()
{
	char filename[STRING_BLOCK], trimmed_filename[STRING_BLOCK];
	int i;
	char line[STRING_BLOCK];
	FILE * fp;

	printf("test_trim_file_header...");
	sprintf(filename,"%s","unit_test_trim_file_header.txt");
	fp = fopen(filename,"w");
	assert(fp!=NULL);
	for (i=1;i<=5;i++) {
		fprintf(fp,"%d\n",i);
	}
	fclose(fp);
	trim_file_header(filename,trimmed_filename);
	fp = fopen(trimmed_filename,"r");
	assert(fp!=NULL);
	for (i=2;i<=5;i++) {
		if (fgets(line , 255 , fp) != NULL) {
			assert(atoi(line)==i);
		}
	}
	fclose(fp);
	sprintf(line,"rm %s %s", filename, trimmed_filename);
	assert(system(line)==0);
	printf("ok\n");
	return 1;
}

static int test_time_to_standard()
{
	char timestr[64], result[64];

	printf("test_time_to_standard...");
	sprintf(timestr,"%s","13.45");
	convert_time_to_standard(timestr, result,
							 get_text_from_identifier(TIME_AM),
							 get_text_from_identifier(TIME_PM));
	assert(same_string(result,"13:45:00")!=0);

	sprintf(timestr,"%s","17:23");
	convert_time_to_standard(timestr, result,
							 get_text_from_identifier(TIME_AM),
							 get_text_from_identifier(TIME_PM));
	assert(same_string(result,"17:23:00")!=0);

	sprintf(timestr,"%s","2pm");
	convert_time_to_standard(timestr, result,
							 get_text_from_identifier(TIME_AM),
							 get_text_from_identifier(TIME_PM));
	assert(same_string(result,"14:00:00")!=0);

	sprintf(timestr,"%s","6.30am");
	convert_time_to_standard(timestr, result,
							 get_text_from_identifier(TIME_AM),
							 get_text_from_identifier(TIME_PM));
	assert(same_string(result,"06:30:00")!=0);

	sprintf(timestr,"%s","abc");
	convert_time_to_standard(timestr, result,
							 get_text_from_identifier(TIME_AM),
							 get_text_from_identifier(TIME_PM));
	assert(same_string(result,"00:00:00")!=0);
	printf("ok\n");
	return 1;
}

int run_tests()
{
	set_language(ENGLISH_UK);  

	printf("Running tests\n");
	test_get_text();
	test_parse();
	test_is_integer();
	test_is_value();
	test_is_money();
	test_is_date();
	test_remove_currency_symbol();
	test_valid_language();
	test_trim();
	test_remove_eol();
	test_money_to_integer();
	test_integer_to_money();
	test_quantity_to_integer();
	test_integer_to_quantity();
	test_pad_text();
	test_pad_value();
	test_convert_date_to_standard();
	test_convert_standard_to_date();
	test_reverse_file();
	test_trim_file_header();
	test_time_to_standard();

	printf("All tests completed\n");
	return 1;
}
