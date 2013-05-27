APP=fin
VERSION=1.51

all:
	gcc -Wall -ansi -pedantic -O3 -o $(APP) src/*.c -Isrc -lsqlite3
 
debug:
	gcc -Wall -ansi -pedantic -g -o $(APP) src/*.c -Isrc -lsqlite3

source:
	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs

install:
	cp $(APP) /usr/bin
	cp man/$(APP).1.gz /usr/share/man/man1
	chmod 755 /usr/bin/$(APP)
	chmod 644 /usr/share/man/man1/$(APP).1.gz

clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png
	rm -rf deb.* debian/$(APP)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
