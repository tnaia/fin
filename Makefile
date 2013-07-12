APP=fin
VERSION=1.51
RELEASE=1
ARCH_TYPE=`uname -m`

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o ${APP} src/*.c -Isrc -lsqlite3
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o ${APP} src/*.c -Isrc -lsqlite3
source:
	tar -cvzf ../${APP}_${VERSION}.orig.tar.gz ../${APP}-${VERSION} --exclude-vcs
install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}/usr/bin
	install -m 755 --strip ${APP} ${DESTDIR}/usr/bin
	mkdir -m 755 -p ${DESTDIR}/usr/share
	mkdir -m 755 -p ${DESTDIR}/usr/share/man
	mkdir -m 755 -p ${DESTDIR}/usr/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}/usr/share/man/man1
uninstall:
	rm -f /usr/share/man/man1/${APP}.1.gz
	rm -rf /usr/share/${APP}
	rm -f /usr/bin/${APP}
clean:
	rm -f ${APP} \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -fr deb.* debian/${APP} rpmpackage/${ARCH_TYPE}
	rm -f ../${APP}*.deb ../${APP}*.changes ../${APP}*.asc ../${APP}*.dsc
	rm -f rpmpackage/*.src.rpm archpackage/*.gz archpackage/*.xz
	rm -f  puppypackage/*.gz puppypackage/*.pet slackpackage/*.txz
