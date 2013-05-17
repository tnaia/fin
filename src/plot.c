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

#include "plot.h"

static int plot_cleanup()
{
	char commandstr[STRING_BLOCK];
	int retval=0,i;
	char str[STRING_BLOCK];

	if (file_exists("gnuplot.plot")!=0) {
		sprintf(commandstr,"%s","rm gnuplot.plot");
		retval = system(commandstr);
	}
	for (i=1;i<=3;i++) {
		sprintf(str,"gnuplot%d.csv",i);
		if (file_exists(str)!=0) {
			sprintf(commandstr,"rm %s",str);
			retval = system(commandstr);
		}
	}
	return retval;
}

int plot(char * title, char * subtitle, int balance_field,
		 char * export_filename,
		 int width, int height,
		 char * image_filename)
{
	char command[STRING_BLOCK],data_filename[STRING_BLOCK];
	char xlabelstr[STRING_BLOCK],ylabelstr[STRING_BLOCK];
	FILE * fp;
	int retval=0;
	char * plot_filename = "gnuplot.plot";

	/* remove the header */
	sprintf((char*)data_filename,"%s","gnuplot2.csv");
	trim_file_header(export_filename, data_filename);

	fp = fopen(plot_filename,"w");
	if (fp!=NULL) {

		/* create the gnuplot file */

		fprintf(fp,"%s","reset\n");
		fprintf(fp,"set title \"%s\"\n",title);

		if (subtitle != NULL) {
			fprintf(fp,"set label \"%s\" at screen 0.47, screen 0.90\n",
					subtitle);
		}

		sprintf(xlabelstr,"%s",	get_text_from_identifier(DATE));
		xlabelstr[0] = toupper(xlabelstr[0]);

		sprintf(ylabelstr,"%s",	get_text_from_identifier(BALANCE));
		ylabelstr[0] = toupper(ylabelstr[0]);

		fprintf(fp,"%s","set lmargin 11\n");
		fprintf(fp,"%s","set rmargin 2\n");
		fprintf(fp,"set xlabel \"%s\"\n",xlabelstr);
		fprintf(fp,"set ylabel \"%s\"\n",ylabelstr);
		fprintf(fp,"%s","set grid\n");
		fprintf(fp,"%s","set key off\n");
		fprintf(fp,"%s","set xdata time\n");    
		fprintf(fp,"%s","set timefmt \"%Y-%m\"\n");
		fprintf(fp,"%s","set format x \"%Y-%m\"\n");
		fprintf(fp,"set terminal png size %d,%d\n",width,height);
		fprintf(fp,"set output \"%s\"\n",image_filename);
		fprintf(fp,"plot \"%s\" using 1:%d with lines\n",
				data_filename,balance_field+1);

		fclose(fp);

		/* run gnuplot */

		sprintf((char*)command,"gnuplot %s",plot_filename);
		retval=system(command);
		plot_cleanup();
	}
	return retval;
}

int plot_distribution(char * title, char * subtitle,
					  int width, int height,
					  char * image_filename,
					  int * distribution, double maximum,
					  int distribution_size)
{
	char command[STRING_BLOCK],data_filename[STRING_BLOCK];
	char amountstr[STRING_BLOCK];
	FILE * fp;
	int i, retval=0;
	char * plot_filename = "gnuplot.plot";
	double increment = maximum / distribution_size;

	/* remove the header */
	sprintf((char*)data_filename,"%s","gnuplot2.csv");

	fp = fopen(data_filename,"w");
	if (fp != NULL) {
		for (i = 0; i < distribution_size; i++) {
			fprintf(fp,"%.6f %d\n",i*increment,distribution[i]);
		}
		fclose(fp);
	}

	fp = fopen(plot_filename,"w");
	if (fp!=NULL) {

		/* create the gnuplot file */

		fprintf(fp,"%s","reset\n");
		fprintf(fp,"set title \"%s\"\n",title);

		if (subtitle != NULL) {
			fprintf(fp,"set label \"%s\" at screen 0.47, screen 0.90\n",
					subtitle);
		}

		fprintf(fp,"%s","set lmargin 9\n");
		fprintf(fp,"%s","set rmargin 2\n");
		sprintf(amountstr,"%s",get_text_from_identifier(AMOUNT));
		if (strlen(amountstr)>0) {
			amountstr[0] = toupper(amountstr[0]);
			fprintf(fp,"set xlabel \"%s\"\n", amountstr);
		}
		fprintf(fp,"set ylabel \"%s\"\n",
				get_text_from_identifier(FREQUENCY));
		fprintf(fp,"%s","set grid\n");
		fprintf(fp,"%s","set key off\n");
		fprintf(fp,"set terminal png size %d,%d\n",width,height);
		fprintf(fp,"set output \"%s\"\n",image_filename);
		fprintf(fp,"plot \"%s\" using 1:2 with lines\n",
				data_filename);

		fclose(fp);

		/* run gnuplot */

		sprintf((char*)command,"gnuplot %s",plot_filename);
		retval=system(command);
		plot_cleanup();
	}
	return retval;
}

int plot_volume(char * title, char * subtitle,
				char * export_filename,
				int width, int height,
				char * image_filename)
{
	char command[STRING_BLOCK],data_filename[STRING_BLOCK];
	char xlabelstr[STRING_BLOCK],ylabelstr[STRING_BLOCK];
	FILE * fp;
	int retval=0;
	char * plot_filename = "gnuplot.plot";

	/* remove the header */
	sprintf((char*)data_filename,"%s","gnuplot2.csv");
	trim_file_header(export_filename, data_filename);

	fp = fopen(plot_filename,"w");
	if (fp!=NULL) {

		/* create the gnuplot file */

		fprintf(fp,"%s","reset\n");
		fprintf(fp,"set title \"%s\"\n",title);

		if (subtitle != NULL) {
			fprintf(fp,"set label \"%s\" at screen 0.47, screen 0.90\n",
					subtitle);
		}

		sprintf(xlabelstr,"%s",	get_text_from_identifier(DATE));
		xlabelstr[0] = toupper(xlabelstr[0]);

		sprintf(ylabelstr,"%s",	get_text_from_identifier(VOLUME));
		ylabelstr[0] = toupper(ylabelstr[0]);

		fprintf(fp,"%s","set lmargin 9\n");
		fprintf(fp,"%s","set rmargin 2\n");
		fprintf(fp,"set xlabel \"%s\"\n",xlabelstr);
		fprintf(fp,"set ylabel \"%s\"\n",ylabelstr);
		fprintf(fp,"%s","set grid\n");
		fprintf(fp,"%s","set key off\n");
		fprintf(fp,"%s","set xdata time\n");    
		fprintf(fp,"%s","set timefmt \"%Y-%m\"\n");
		fprintf(fp,"%s","set format x \"%Y-%m\"\n");
		fprintf(fp,"set terminal png size %d,%d\n",width,height);
		fprintf(fp,"set output \"%s\"\n",image_filename);
		fprintf(fp,"plot \"%s\" using 1:2 with lines\n",
				data_filename);

		fclose(fp);

		/* run gnuplot */

		sprintf((char*)command,"gnuplot %s",plot_filename);
		retval=system(command);
		plot_cleanup();
	}
	return retval;
}

int plot_piechart(char * account,
				  char * search_string, int year, int month_number,
				  char field_separator, char * currency,
				  int spending,
				  char * title,
				  int width, int height,
				  char * image_filename)
{
	double value[MAX_PIECHART_FIELDS],total_value=0;
	char * field[MAX_PIECHART_FIELDS];
	char command[STRING_BLOCK];
	char str[STRING_BLOCK];
	int no_of_fields,i,retval=0;
	double d,tot_value;
	FILE * fp;
	char * plot_filename = "gnuplot.plot";
	double total_income=0, total_outgoings=0;
	const double pie_radius = 0.4;
	const double label_radius = 0.2;
	const double character_width = 0.016;   

	no_of_fields = calc_piechart(account, search_string,
								 month_number, year,
								 field_separator, currency,
								 spending,
								 (char**)field, (double*)value,
								 &total_value,
								 MAX_PIECHART_FIELDS,
								 0);

	if (spending != 0) {
		/* add an 'other' category for the
		   remaining income not spent */
		sprintf(str,"%s","*");
		get_totals(account, str,
				   month_number, year,
				   currency,
				   &total_income, &total_outgoings);
		if (total_income > total_value) {
			field[no_of_fields] = (char*)malloc(STRING_BLOCK);
			sprintf(field[no_of_fields],"%s",
					get_text_from_identifier(CATEGORY_OTHER));
			value[no_of_fields] = total_income - total_value;			
			no_of_fields++;
			total_value = total_income;
		}
	}
	
	if (((int)total_value==0) || (no_of_fields<2)) {
		/* nothing to see */
		printf("%s\n",get_text_from_identifier(NOTHING_TO_PLOT));
		return 0;
	}

	/* create a title if none is specified */
	if (title==0) {
		if (spending!=0) {
			if (year==0) {
				title = get_text_from_identifier(PIECHART_SPENDING);
			}
			else {
				sprintf(str,
						get_text_from_identifier(PIECHART_SPENDING_YEAR),
						year);
				title = (char*)str;
			}
		}
		else {
			if (year==0) {
				title = get_text_from_identifier(PIECHART_INCOME);
			}
			else {
				sprintf(str,
						get_text_from_identifier(PIECHART_INCOME_YEAR),
						year);
				title = (char*)str;
			}
		}
	}

	/* convert values to percentages */
	value[no_of_fields]=0;
	tot_value=0;
	for (i=0;i<no_of_fields-1;i++) {
		value[i] = (int)(value[i]*1000/total_value)/10.0;
		tot_value += value[i];
	}
	value[no_of_fields-1]=100-tot_value;

	fp = fopen(plot_filename,"w");
	if (fp!=NULL) {

		fprintf(fp,"%s","reset\n");
		fprintf(fp,"set title \"%s\"\n",title);

		d = 0;
		fprintf(fp,"b=%.2f; r=%.2f; B=%.2f\n",
				pie_radius, label_radius, 0.5);
		fprintf(fp,"%s","unset border; unset tics; unset key\n");
		fprintf(fp,"set angles degree; set yrange [0:1]; " \
				"set style fill solid 1.0 border -1\n");
		for (i = 1; i <= no_of_fields; i++) {
			fprintf(fp,"set obj %d circle arc [%f:%f] fc lt %d\n",
					i, d*360,(d+(value[i-1]/100.0))*360, i);
			fprintf(fp,"set obj %d circle at " \
					"screen B,B size screen b front\n", i);
			d += value[i-1]/100.0;
		}
		d = value[0]*360/(100*2);
		for(i = 0; i < no_of_fields; i++) {
			sprintf(str,"%s %.1f%%",field[i],value[i]);

			fprintf(fp,"set label %d \"%s\" front at screen " \
					"B+(r*cos(%f))-%.2f, B+(r*sin(%f))\n",
					i+1, str, d,
					strlen(str)*0.5*character_width, d);
			d += (value[i]*360/(100*2)) +
				(value[i+1]*360/(100*2));
		}
		fprintf(fp,"set terminal png size %d,%d\n",width,height);
		fprintf(fp,"set output \"%s\"\n",image_filename);
		fprintf(fp,"plot 2\n");

		fclose(fp);

		sprintf((char*)command,"gnuplot %s",plot_filename);
		retval=system(command);
		plot_cleanup();
	}

	/* free allocated strings */
	for (i=0;i<no_of_fields;i++) {
		free(field[i]);
		field[i]=0;
	}

	return retval;
}
