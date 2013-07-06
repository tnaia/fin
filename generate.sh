#!/bin/bash

# Generates packaging for fin

rm -f Makefile rpmpackage/*.spec

packagemonkey -n "fin" --cmd --dir "." -l "gpl3" -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" --brief "Command line personal finances manager" --desc "Keeping track of personal finances is an incredibly boring task, and if you don't have much money then juggling it can be tricky. When looking at the existing software out there to manage accounts it's frequently highly complex and top heavy. Fin - an abbreviation of \"financials\" - is an attempt to do the opposite of what most accounting software does, and that's to make things incredibly simple and quick. Logging a transaction or viewing your current balance should take no more than a few seconds. This means less time wasted on bureaucracy and more time spent on what you really want to be doing. Fin can import and export to CSV files, Emacs org mode tables and GnuCash." --homepage "https://github.com/fuzzgun/fin" --repository "https://github.com/fuzzgun/fin.git" --section "utils" --version "1.51" --categories "Utility/ConsoleOnly" --compile "-lsqlite3" --builddeb "libsqlite3-dev" --dependsdeb "gnuplot, bcrypt, sqlite3" --buildarch "sqlite" --dependsarch "gnuplot, sqlite" --dependspuppy "sqlite3_3.7.9, gnuplot_4.4.3, bcrypt-1.1"
