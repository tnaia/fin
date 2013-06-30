APP=fin
VERSION=1.51
ARCH_TYPE=`uname -m`

all:
	gcc -Wall -ansi -pedantic -O3 -o $(APP) src/*.c -Isrc -lsqlite3
 
debug:
	gcc -Wall -ansi -pedantic -g -o $(APP) src/*.c -Isrc -lsqlite3

source:
	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs

install:
	mkdir -p $(DESTDIR)/usr
	mkdir -p $(DESTDIR)/usr/bin
	mkdir -p $(DESTDIR)/usr/share
	mkdir -p $(DESTDIR)/usr/share/man
	mkdir -p $(DESTDIR)/usr/share/man/man1
	install -m 755 $(APP) $(DESTDIR)/usr/bin
	install -m 644 man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1
	install -m 755 --strip $(APP) $(DESTDIR)/usr/bin
	mkdir -m 755 -p $(DESTDIR)/usr/share
	mkdir -m 755 -p $(DESTDIR)/usr/share/man
	mkdir -m 755 -p $(DESTDIR)/usr/share/man/man1

clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
	rm -f rpmpackage/*.src.rpm archpackage/*.gz
	rm -fr deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f rpmpackage/*.src.rpm archpackage/*.gz puppypackage/*.gz puppypackage/*.pet
