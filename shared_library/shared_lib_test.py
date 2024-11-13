import ctypes
from ctypes import POINTER, Structure, c_int, c_double, c_long, byref
import pathlib
import logging

logging.basicConfig(
    level = logging.DEBUG,
    format = '[%(asctime)s] [%(levelname)s] [%(funcName)s:%(lineno)d] %(message)s'
    )

current = pathlib.Path.cwd()

logging.info(f'current: {current}')

dll = ctypes.windll.LoadLibrary(current.joinpath('bin/shared_lib.dll'))

if not dll:
    logging.error(f'LoadLibrary bin/shared_lib.dll error')
    exit(1)

dll.add.argtypes = [c_long, c_long]
dll.add.restype = c_long

def add(a: int, b: int) -> int:
    return dll.add(a, b)

logging.info(f'add: {add(1, 2)}')

class Point(Structure):
    _fields_ = [('x', c_int),
                ('y', c_int)]

# dll.Point_init.argtypes = [Point]
pt = Point(1, 1)

logging.info(f'{pt.x} {pt.y}')

dll.Point_init(byref(pt))

logging.info(f'{pt.x} {pt.y}')

pt1 = Point(0, 0)
pt2 = Point(3, 4)

dll.Point_distance.argtypes = [POINTER(Point), POINTER(Point)]
dll.Point_distance.restype = c_double

distance = dll.Point_distance(byref(pt1), byref(pt2))

logging.info(f'distance: {distance}')

exit(0)
