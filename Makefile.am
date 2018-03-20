SUBDIRS= doc examples

#DOC_FILES= \
#    $(top_srcdir)/doc/attrtype.htm \
#    $(NULL)

include_HEADERS = \
    $(top_srcdir)/include/ciut.h \
    $(top_srcdir)/include/ciut-sio.h \
    $(top_srcdir)/include/uclog.h \
    $(top_srcdir)/include/ucport.h \
    $(top_srcdir)/include/uctime.h \
    $(NULL)


EXTRA_DIST=autogen.sh autoclean.sh AUTHORS README.md ChangeLog \
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
cov-reset-common:
	@rm -rf $(top_builddir)/coverage
	@find $(top_builddir)/examples -name "*.gcda" -delete

cov-reset: cov-reset-common
	@lcov --zerocounters --directory $(top_builddir)

cov-report:
	@mkdir $(top_builddir)/coverage
	lcov -c -o $(top_builddir)/coverage/coverage.info -d $(top_builddir) \
		 --rc lcov_branch_coverage=1
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
else
coverage:
	@echo "reconfigure with --enable-coverage"
endif

