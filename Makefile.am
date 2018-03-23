SUBDIRS= examples

ACLOCAL_AMFLAGS = -I m4

DOC_FILES= \
    $(top_srcdir)/doc/Doxyfile.in \
    $(top_srcdir)/doc/genfooter.tex \
    $(top_srcdir)/doc/genheader.tex \
    $(top_srcdir)/doc/Makefile.am \
    $(top_srcdir)/doc/Makefile.in \
    $(NULL)

include_HEADERS = \
    $(top_srcdir)/include/ciut.h \
    $(top_srcdir)/include/ciut-sio.h \
    $(top_srcdir)/include/uclog.h \
    $(top_srcdir)/include/ucport.h \
    $(top_srcdir)/include/uctime.h \
    $(NULL)


EXTRA_DIST=autogen.sh autoclean.sh AUTHORS README.md ChangeLog \
    $(top_srcdir)/m4/ax_check_compile_flag.m4 \
    $(top_srcdir)/m4/ax_check_link_flag.m4 \
    $(top_srcdir)/m4/ax_cxx_compile_stdcxx.m4 \
    $(top_srcdir)/m4/ax_valgrind_check.m4 \
    $(DOC_FILES) \
    $(NULL)

ChangeLog: .git
	if test -d $(srcdir)/.git; then                         \
	  if test -f $(srcdir)/.last-cl-gen; then               \
        git log --no-merges --date-order --date=short       \
          $$(cat $(srcdir)/.last-cl-gen)..                  \
          | sed -e  '/^commit.*$$/d'                        \
          | awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' \
          | sed -e 's/^Author: //g'                         \
          | sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' \
          | sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g' \
          > ChangeLog.tmp;                                  \
      else                                                  \
        git log --no-merges --date-order --date=short       \
          | sed -e  '/^commit.*$$/d'                        \
          | awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' \
          | sed -e 's/^Author: //g'                         \
          | sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' \
          | sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g' \
          > ChangeLog.tmp;                                  \
      fi;                                                   \
	  touch ChangeLog                                       \
	    && git rev-list -n 1 HEAD >.last-cl-gen.tmp         \
        && (echo; cat $(srcdir)/ChangeLog) >>ChangeLog.tmp  \
        && mv -f ChangeLog.tmp $(srcdir)/ChangeLog          \
        && mv -f .last-cl-gen.tmp $(srcdir)/.last-cl-gen    \
        && rm -f ChangeLog.tmp;                             \
    fi
	if test -d $(srcdir)/.hg; then                          \
        hg log --template changelog > ChangeLog;            \
        touch $(srcdir)/.last-cl-gen;                       \
    fi


if COVERAGE

COV_ARG_gcc=
COV_ARG_g++=
COV_ARG_clang=--gcov-tool $(abs_top_srcdir)/gcov-tool-clang
COV_DEPEND_gcc=
COV_DEPEND_g++=
COV_DEPEND_clang=$(abs_top_srcdir)/gcov-tool-clang

$(abs_top_srcdir)/gcov-tool-clang:
	@echo "#!/bin/bash" > $@
	@echo "exec llvm-cov gcov \"\$$@\"" >> $@
	@chmod 755 $@

cov-reset-common:
	@rm -rf $(top_builddir)/coverage
	@find $(top_builddir) -name "*.gc[nd][ao]" -delete

cov-reset: cov-reset-common
	@lcov --zerocounters --directory $(top_builddir)

cov-report: $(COV_DEPEND_$(shell echo $(CC) |  sed 's/[\._0-9-]*//g'))
	@mkdir $(top_builddir)/coverage
	@echo "PWD=`pwd`"
	lcov -c -o $(top_builddir)/coverage/coverage.info -d $(top_builddir) \
		 --rc lcov_branch_coverage=1 $(COV_ARG_$(shell echo $(CC) |  sed 's/[\._0-9-]*//g'))
	lcov -r $(top_builddir)/coverage/coverage.info '*/tests/*' '*/suites/*' '/usr*' \
		 -o $(abs_top_builddir)/coverage/coverage.cleaned.info \
		 --rc lcov_branch_coverage=1
	genhtml --num-spaces 4 --legend --branch-coverage --ignore-errors source \
			-t "$(PACKAGE_STRING)" \
			-o $(top_builddir)/coverage/html \
			-p `readlink -m $(abs_top_srcdir)`/src \
			$(top_builddir)/coverage/coverage.cleaned.info
	@echo "Coverage Report at $(top_builddir)/coverage/html" >&2

coverage:
	@$(MAKE) cov-reset
	@$(MAKE) check
	@$(MAKE) cov-report

clean-local-coverage: cov-reset-common
#	@rm -f $(abs_top_builddir)/gcov-tool-clang

else
clean-local-coverage:
	-rm -rf dummy.cpp

coverage:
	@echo "reconfigure with --enable-coverage"
endif

clean-local: clean-local-coverage


