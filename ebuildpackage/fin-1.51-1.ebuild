# $Header: $

EAPI=4

DESCRIPTION="Keeping track of personal finances is an incredibly boring task, and if you don't have much money then juggling it can be tricky. When looking at the existing software out there to manage accounts it's frequently highly complex and top heavy. Fin - an abbreviation of "financials" - is an attempt to do the opposite of what most accounting software does, and that's to make things incredibly simple and quick. Logging a transaction or viewing your current balance should take no more than a few sec"
HOMEPAGE="https://github.com/fuzzgun/fin"
SRC_URI="${PN}/${P}.tar.gz"
LICENSE="GPL3"
SLOT="0"
KEYWORDS="x86"
RDEPEND="dev-libs/popt"
DEPEND="${RDEPEND}"

src_configure() {
    econf --with-popt
}

src_install() {
    emake DESTDIR="${D}" install
    # Install README and (Debian) changelog
    dodoc README.md debian/changelog
}
