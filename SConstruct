env = Environment(CPPPATH=['#include'])

# Conan setup
conandeps = SConscript('./build/generators/SConscript_conandeps')
flags = conandeps["conandeps"]
env.MergeFlags(flags)

compiler = env.subst('$CXX')
if 'cl' in compiler:
    # MSVC flags
    env.Append(CXXFLAGS=['/std:c++latest', '/W4', '/EHsc', '/MD'])
else:
    # GCC/Clang flags
    env.Append(CXXFLAGS=['-std=c++23', '-Wall', '-Wextra'])

# Build directories
env.VariantDir('build/src', 'src', duplicate=0)
env.VariantDir('build/test', 'test', duplicate=0)

# Filter out main.cpp to compile other source files with test.cpp
all_src = Glob('build/src/*.cpp')
core_src = [f for f in all_src if 'main.cpp' not in str(f)]
main_src = 'build/src/main.cpp'

# Main App
app = env.Program(target='build/img-svg', source=[main_src] + core_src)

# Test Target
test_src = Glob('build/test/*.cpp')
test_app = env.Program(target='build/run-tests', source=test_src + core_src)

# Documentation
env.Alias('doc', None, 'doxygen Doxyfile')
env.AlwaysBuild('doc')
