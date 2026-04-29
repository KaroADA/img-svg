env = Environment(CXX='g++', CXXFLAGS=['-std=c++23', '-Wall', '-Wextra'], CPPPATH=['#include'])

env.Append(LIBS=['boost_program_options'])

env.VariantDir('build', 'src', duplicate=0)
src_files = Glob('build/*.cpp')
app = env.Program(target='build/img-svg', source=src_files)

env.Alias('doc', None, 'doxygen Doxyfile')
env.AlwaysBuild('doc')
