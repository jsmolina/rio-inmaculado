#!/usr/bin/env bash

unset CDPATH

source script/common-settings

# source tarball versions
BINUTILS_VERSION=230
DJCRX_VERSION=205
DJLSR_VERSION=205
DJDEV_VERSION=205

GCC_VERSION=12.2.0
GCC_VERSION_SHORT=12.20
GMP_VERSION=6.2.1
MPFR_VERSION=4.1.0
MPC_VERSION=1.2.1
AUTOCONF_VERSION=2.69
AUTOMAKE_VERSION=1.15.1

#CFLAGS="-O2 -g $CFLAGS -std=gnu11"

# tarball location
BINUTILS_ARCHIVE="${DJGPP_DOWNLOAD_BASE}/djgpp/deleted/v2gnu/bnu${BINUTILS_VERSION}s.zip"
DJCRX_ARCHIVE="${DJGPP_DOWNLOAD_BASE}/djgpp/current/v2/djcrx${DJCRX_VERSION}.zip"
DJLSR_ARCHIVE="${DJGPP_DOWNLOAD_BASE}/djgpp/current/v2/djlsr${DJLSR_VERSION}.zip"
DJDEV_ARCHIVE="${DJGPP_DOWNLOAD_BASE}/djgpp/current/v2/djdev${DJDEV_VERSION}.zip"

DJCROSS_GCC_ARCHIVE="${DJGPP_DOWNLOAD_BASE}/djgpp/rpms/djcross-gcc-${GCC_VERSION}/djcross-gcc-${GCC_VERSION}.tar.bz2"
GCC_ARCHIVE="${FTPMIRROR_GNU_DOWNLOAD_BASE}/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
GMP_ARCHIVE="${FTPMIRROR_GNU_DOWNLOAD_BASE}/gmp/gmp-${GMP_VERSION}.tar.xz"
MPFR_ARCHIVE="${FTPMIRROR_GNU_DOWNLOAD_BASE}/mpfr/mpfr-${MPFR_VERSION}.tar.xz"
MPC_ARCHIVE="${FTPMIRROR_GNU_DOWNLOAD_BASE}/mpc/mpc-${MPC_VERSION}.tar.gz"
AUTOCONF_ARCHIVE="${FTP_GNU_DOWNLOAD_BASE}/autoconf/autoconf-${AUTOCONF_VERSION}.tar.xz"
AUTOMAKE_ARCHIVE="${FTP_GNU_DOWNLOAD_BASE}/automake/automake-${AUTOMAKE_VERSION}.tar.xz"

# check required programs
REQ_PROG_LIST="${CXX} ${CC} unzip bison flex ${MAKE} makeinfo patch"

# use curl or wget?
if [ ! -z $USE_WGET ]; then
  REQ_PROG_LIST+=" wget"
else
  REQ_PROG_LIST+=" curl"
fi

for REQ_PROG in $REQ_PROG_LIST; do
  if ! which $REQ_PROG > /dev/null; then
    echo "$REQ_PROG not installed"
    exit 1
  fi
done

# check GNU sed is installed or not.
# It is for OSX, which doesn't ship with GNU sed.
if ! sed --version 2>/dev/null |grep "GNU sed" > /dev/null ;then
  echo GNU sed is not installed, need to download.
  SED_VERSION=4.2.2
  SED_ARCHIVE="${FTP_GNU_DOWNLOAD_BASE}/sed/sed-${SED_VERSION}.tar.bz2"
else
  SED_ARCHIVE=""
fi

# check zlib is installed
if ! ${CC} test-zlib.c -o test-zlib -lz; then
  echo "zlib not installed"
  exit 1
fi


# djcross-gcc-X.XX-tar.* maybe moved from /djgpp/rpms/ to /djgpp/deleted/rpms/ directory.
OLD_DJCROSS_GCC_ARCHIVE=${DJCROSS_GCC_ARCHIVE/rpms\//deleted\/rpms\/}

# download source files
ARCHIVE_LIST="$BINUTILS_ARCHIVE $DJCRX_ARCHIVE $DJLSR_ARCHIVE $DJDEV_ARCHIVE
              $SED_ARCHIVE $DJCROSS_GCC_ARCHIVE $OLD_DJCROSS_GCC_ARCHIVE $GCC_ARCHIVE
              $GMP_ARCHIVE $MPFR_ARCHIVE $MPC_ARCHIVE
              $AUTOCONF_ARCHIVE $AUTOMAKE_ARCHIVE"


# create target directory, check writable.
echo "Make prefix directory : $DJGPP_PREFIX"
mkdir -p $DJGPP_PREFIX

if ! [ -d $DJGPP_PREFIX ]; then
  echo "Unable to create prefix directory"
  exit 1
fi

if ! [ -w $DJGPP_PREFIX ]; then
  echo "prefix directory is not writable."
  exit 1
fi

# make build dir
echo "Change to build dir"
cd build

# build binutils
echo "Building binutils"
mkdir bnu${BINUTILS_VERSION}s
cd bnu${BINUTILS_VERSION}s
cd gnu/binutils-* || exit

# exec permission of some files are not set, fix it.
for EXEC_FILE in install-sh missing; do
  echo "chmod a+x $EXEC_FILE"
  chmod a+x $EXEC_FILE || exit 1
done

sh ./configure \
           --prefix=$DJGPP_PREFIX \
           --target=i586-pc-msdosdjgpp \
           --program-prefix=i586-pc-msdosdjgpp- \
           --disable-werror \
           --disable-nls \
           || exit 1

${MAKE} configure-bfd || exit 1
${MAKE} -C bfd stmp-lcoff-h || exit 1
${MAKE} || exit 1

if [ ! -z $MAKE_CHECK ]; then
  echo "Run ${MAKE} check"
  ${MAKE} check || exit 1
fi

${MAKE} install || exit 1

cd ../../..
# binutils done

# prepare djcrx
echo "Prepare djcrx"
mkdir djcrx${DJCRX_VERSION}
cd djcrx${DJCRX_VERSION}

cd src/stub
${CC} -O2 stubify.c -o stubify || exit 1
${CC} -O2 stubedit.c -o stubedit || exit 1

cd ../..

mkdir -p $DJGPP_PREFIX/i586-pc-msdosdjgpp/sys-include || exit 1
cp -rp include/* $DJGPP_PREFIX/i586-pc-msdosdjgpp/sys-include/ || exit 1
cp -rp lib $DJGPP_PREFIX/i586-pc-msdosdjgpp/ || exit 1
cp -p src/stub/stubify $DJGPP_PREFIX/i586-pc-msdosdjgpp/bin/ || exit 1
cp -p src/stub/stubedit $DJGPP_PREFIX/i586-pc-msdosdjgpp/bin/ || exit 1

cd ..
# djcrx done

# build gcc
tar -xjvf ../download/djcross-gcc-${GCC_VERSION}.tar.bz2 || exit 1
cd djcross-gcc-${GCC_VERSION}/

BUILDDIR=`pwd`

echo "Building autoconf"
cd $BUILDDIR
tar xJf ../../download/autoconf-${AUTOCONF_VERSION}.tar.xz || exit 1
cd autoconf-${AUTOCONF_VERSION}/
./configure --prefix=$BUILDDIR/tmpinst || exit 1
${MAKE} all install || exit 1

echo "Building automake"
cd $BUILDDIR
tar xJf ../../download/automake-${AUTOMAKE_VERSION}.tar.xz || exit 1
cd automake-${AUTOMAKE_VERSION}/
PATH="$BUILDDIR//tmpinst/bin:$PATH" \
./configure --prefix=$BUILDDIR/tmpinst || exit 1
PATH="$BUILDDIR//tmpinst/bin:$PATH" \
${MAKE} all install || exit 1

# build GNU sed if needed.
SED=sed
if [ ! -z $SED_VERSION ]; then
  echo "Building sed"
  cd $BUILDDIR
  ${MAKE} all install || exit 1
  SED=$BUILDDIR/tmpinst/bin/sed
fi

cd $BUILDDIR


# gcc 4.8 or above unpack-gcc.sh needs to be patched for OSX
# patch from :
#   ( cd gnu && tar xf $top/$archive $tar_param && echo $archive >$top/s-sources )
# to :
#   ( cd gnu && tar xJf $top/$archive && echo $archive >$top/s-sources )

# In compressed file gcc-12.2.0.tar.xz, its directory :
# gcc-12.2.0/gcc/testsuite/go.test/test/fixedbugs/issue27836.dir/
# contains files with non-ASCII filename. They can't be decompressed in
# Windows system.
# Use tar's --exclude option to skip this directory.
# patch from :
#   ( cd gnu && tar xJf $top/$archive && echo $archive >$top/s-sources )
# to :
#   ( cd gnu && tar xJf $top/$archive  --exclude=issue27836.dir && echo $archive >$top/s-sources )
if uname|grep "^MINGW32" > /dev/null; then
  $SED -i "s/\(cd gnu && tar [^\&]*\)/\1 --exclude=issue27836.dir /" unpack-gcc.sh || exit 1
fi

if [ `uname` = "FreeBSD" ]; then
  # The --verbose option is not recognized by BSD patch
  $SED -i 's/patch --verbose/patch/' unpack-gcc.sh || exit 1
fi

echo "Running unpack-gcc.sh"


# copy stubify programs
cp $DJGPP_PREFIX/i586-pc-msdosdjgpp/bin/stubify $BUILDDIR/tmpinst/bin

echo "Building gmp"
cd $BUILDDIR/gmp-*/
# On some systems, building gmp will fail if CFLAGS is set.
# Unset CFLAGS during building gmp.

${MAKE} all || exit 1
if [ ! -z $MAKE_CHECK ]; then
  echo "Run ${MAKE} check"
  ${MAKE} check || exit 1
fi
${MAKE} install || exit 1

echo "Building mpfr"
cd $BUILDDIR/mpfr-${MPFR_VERSION}/
./configure --prefix=$BUILDDIR/tmpinst --with-gmp=$BUILDDIR/tmpinst --enable-static --disable-shared || exit 1
${MAKE} all || exit 1
if [ ! -z $MAKE_CHECK ]; then
  echo "Run ${MAKE} check"
  ${MAKE} check || exit 1
fi
${MAKE} install || exit 1

echo "Building mpc"
cd $BUILDDIR/mpc-${MPC_VERSION}/
if [ ! -z $MAKE_CHECK ]; then
  echo "Run ${MAKE} check"
  ${MAKE} check || exit 1
fi
${MAKE} install || exit 1

echo "Building gcc"
cd $BUILDDIR/

mkdir djcross
cd djcross

PATH="$BUILDDIR//tmpinst/bin:$PATH" \

# Workaround for MinGW. Need to fix some .deps/*.Plo files.
# Fix from
#  e\:/build-djgpp\...
# to
#  /e/build-djgpp\...

${MAKE} install-strip || exit 1

echo "Copy long name executables to short name."
(
  cd $DJGPP_PREFIX || exit 1
  SHORT_NAME_LIST="gcc g++ c++ addr2line c++filt cpp size strings"
  for SHORT_NAME in $SHORT_NAME_LIST; do
    if [ -f bin/i586-pc-msdosdjgpp-gcc ]; then
      cp bin/i586-pc-msdosdjgpp-$SHORT_NAME i586-pc-msdosdjgpp/bin/$SHORT_NAME
    fi
  done
) || exit 1

# gcc done

# build djlsr (for dxegen / exe2coff)
echo "Prepare djlsr"
cd $BUILDDIR
cd ..
cd djlsr${DJLSR_VERSION}
if [ "$CC" == "gcc" ]; then
  echo "Building DXE tools."
  cd src
  cp dxe/dxegen dxe/dxe3gen dxe/dxe3res $DJGPP_PREFIX/i586-pc-msdosdjgpp/bin/ || exit 1
  cd ..
else
  echo "Building DXE tools requires gcc, skip."
fi
cd src/stub
${CC} -o exe2coff exe2coff.c || exit 1
cp -p exe2coff $DJGPP_PREFIX/i586-pc-msdosdjgpp/bin/ || exit 1
cd ../../..
# djlsr done

# copy setenv script
(cd $BUILDDIR/../../setenv/ && ./copyfile.sh $DJGPP_PREFIX) || exit 1

echo "Testing DJGPP."
cd $BUILDDIR
cd ..
echo "Use DJGPP to build a test C program."
$DJGPP_PREFIX/bin/i586-pc-msdosdjgpp-gcc ../hello.c -o hello || exit 1

for x in $(echo $ENABLE_LANGUAGES | tr "," " ")
do
  case $x in
    c++)
      echo "Use DJGPP to build a test C++ program."
      $DJGPP_PREFIX/bin/i586-pc-msdosdjgpp-c++ ../hello-cpp.cpp -o hello-cpp || exit 1
      ;;
  esac
done

echo "build-djgpp.sh done."
