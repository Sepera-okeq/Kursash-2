# add e2k arch
sed -i "/elif defined(__hppa__)/i\\\n# elif defined (__e2k__)\n
e2k-linux-gnu" configure*
# unsupported profiling option
sed -i "s| -fprofile-correction||" configure*
# LCC profiling bug workaround
sed -i "/^Modules\\/_math.o:/{n;s|\$(CCSHARED)\$(PY_CORE_CFLAGS)|
\$(filter-out -fprofile-generate-parallel,
\$(CCSHARED) \$(PY_CORE_CFLAGS))|}" Makefile.pre.in
sed -i "s|-fprofile-generate|-fprofile-generate-parallel|"
configure*
sed -i "s/rm -f profile-clean-stamp/&; eprof -s eprof.sum/"
Makefile.pre.in
# exclude hanging tests
sed -i "s/^.*def test_stack_overflow(/
@unittest.skipIf(True, 'hangs')\n&/" Lib/test/test_faulthandler.py

# faster interpreter on Elbrus
sed -i "/#if USE_COMPUTED_GOTOS/{s|^|#undef USE_COMPUTED_GOTOS\n
#define USE_COMPUTED_GOTOS 0\n|;:a;n;ba}" Python/ceval.c
sed -i "/_unknown_opcode:/{n;n;s|$|Py_UNREACHABLE();\n
#include \"opcode_unknown.h\"|}" Python/ceval.c
awk '/_unknown_opcode/{print "case " NR-2 ":"}' 
Python/opcode_targets.h > Python/opcode_unknown.h