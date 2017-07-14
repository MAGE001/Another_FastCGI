env = Environment()

env.Append(CPPFLAGS = ['-ggdb3', '-fPIC', '-Wall', '-g'])
env.Append(CPPPATH = ['/usr/include', '.', 'jsoncpp/include'])
env.Append(LIBS = ['pthread', 'rt', 'fcgi', 'json'])
env.Append(LIBPATH = ['/usr/lib64', '/usr/local/lib', 'jsoncpp'])

bin = env.Program('CgiBO', ['AuthProc.cpp', 'FcgiMain.cpp', 'Utf8Code.cpp',])