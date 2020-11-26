Import('rtconfig')
from building import *

cwd = GetCurrentDir()
src	= Glob('*.c')

CPPPATH = [cwd]

group = DefineGroup('uC-Modbus', src, depend = ['PKG_USING_UC_MODBUS'], CPPPATH = CPPPATH)

Return('group')
