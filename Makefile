APP=fin
VERSION=1.51
RELEASE=1
ARCH_TYPE=$(shell uname -m)
PREFIX?=/usr/local
ARCH_BUILD_DIR=${HOME}/abs/${APP}
CURR_DIR=$(shell pwd)

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o ${APP} src/*.c -Isrc -lsqlite3
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o ${APP} src/*.c -Isrc -lsqlite3
source:
	tar -cvf ../${APP}_${VERSION}.orig.tar ../${APP}-${VERSION} --exclude-vcs
	gzip -f9n ../${APP}_${VERSION}.orig.tar
arch:
	rm -f ${APP} *.xz *.sig
	@if [ ! -d ${ARCH_BUILD_DIR} ]; then\
		mkdir -p ${ARCH_BUILD_DIR};\
	fi
	rm -rf ${ARCH_BUILD_DIR}/*
	tar -pczf ${ARCH_BUILD_DIR}/${APP}-${VERSION}.tar.gz . --exclude-vcs --exclude "/.git/*"
	cp PKGBUILD ${ARCH_BUILD_DIR}
	gpg -ba ${ARCH_BUILD_DIR}/${APP}-${VERSION}.tar.gz
	sed -i "s|arch=()|arch=('${ARCH_TYPE}')|g" ${ARCH_BUILD_DIR}/PKGBUILD
	cd ${ARCH_BUILD_DIR}; updpkgsums; makepkg -f -c -s; makepkg --printsrcinfo > .SRCINFO
	unxz ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar.xz
	tar -vf ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar --delete .BUILDINFO
	xz ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar
	gpg -ba ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar.xz
install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 755 -s ${APP} ${DESTDIR}${PREFIX}/bin
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1
uninstall:
	rm -f ${PREFIX}/share/man/man1/${APP}.1.gz
	rm -rf ${PREFIX}/share/${APP}
	rm -f ${PREFIX}/bin/${APP}
clean:
	rm -f ${APP} *.xz *.sig \#* \.#* gnuplot* *.png
