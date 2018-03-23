#!/bin/bash

./autoclean.sh

rm -f configure

rm -f Makefile.in

rm -f config.guess
rm -f config.sub
rm -f install-sh
rm -f missing
rm -f depcomp

if [ 0 = 1 ]; then
autoscan
else

touch NEWS
touch README
touch AUTHORS
touch ChangeLog
touch config.h.in
touch install-sh

libtoolize --force --copy --install --automake
aclocal
automake -a -c
autoconf
# run twice to get rid of 'ltmain.sh not found'
autoreconf -f -i -Wall,no-obsolete
autoreconf -f -i -Wall,no-obsolete

if [ 1 = 1 ]; then
  ./configure --disable-debug
else
  which "$CC" || CC=gcc
  which "$CXX" || if [[ "$CC" =~ .*clang.* ]]; then CXX=clang++; else CXX=g++; fi
  CC=$CC CXX=$CXX ./configure --enable-debug --enable-coverage --enable-valgrind
  make clean; make; make coverage CC=$CC CXX=$CXX; make check CC=$CC CXX=$CXX; make check-valgrind CC=$CC CXX=$CXX
fi

#make clean
#make ChangeLog

#make
#make check
#make -C doc/latex/
#make dist-gzip

fi
