# Maintainer: Bob Mottram <bob@freedombone.net>
pkgname=fin
pkgver=1.51
pkgrel=1
epoch=
pkgdesc="The command line personal finances manager"
arch=()
url="https://github.com/bashrc/fin"
license=('GPL3')
groups=()
depends=('sqlite' 'gnuplot')
makedepends=('sqlite')
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.gz" "$pkgname-$pkgver.tar.gz.asc")
noextract=()
md5sums=()
sha512sums=()
validpgpkeys=()

build() {
    make
}

package() {
    make DESTDIR="$pkgdir" install
}
