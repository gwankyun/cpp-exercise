import ctypes
from ctypes import POINTER, Structure, c_int, c_double, c_long, byref, c_char_p,\
    c_size_t
import pathlib
import logging
import unittest

logging.basicConfig(
    level = logging.DEBUG,
    format = '[%(asctime)s] [%(levelname)s] [%(funcName)s:%(lineno)d] %(message)s'
    )

class Point(Structure):
    _fields_ = [('x', c_int),
                ('y', c_int)]

g_dll: ctypes.WinDLL = None

class TestSharedLib(unittest.TestCase):
    def get_dll(self):
        return g_dll

    def test_true(self):
        self.assertTrue(True)

    def test_add(self):
        dll = self.get_dll()
        dll.add.argtypes = [c_long, c_long]
        dll.add.restype = c_long

        def add(a: int, b: int) -> int:
            return dll.add(a, b)
        self.assertEqual(add(1, 2), 3)
    
    def test_Point_init(self):
        dll = self.get_dll()
        dll.Point_init.argtypes = [POINTER(Point)]
        pt = Point(1, 1)

        dll.Point_init(byref(pt))

        self.assertEqual((pt.x, pt.y), (0, 0))

    def test_Point_distance(self):
        dll = self.get_dll()
        pt1 = Point(0, 0)
        pt2 = Point(3, 4)

        dll.Point_distance.argtypes = [POINTER(Point), POINTER(Point)]
        dll.Point_distance.restype = c_double

        distance = dll.Point_distance(byref(pt1), byref(pt2))
        self.assertEqual(int(distance), 5)

    def test_set_int(self):
        dll = self.get_dll()
        dll.set_int.argtypes = [POINTER(c_int), c_int]
        value = c_int(0)
        dll.set_int(byref(value), c_int(1))
        self.assertEqual(value.value, 1)

    def test_set_char_p(self):
        dll = self.get_dll()
        dll.set_char_p.argtypes = [POINTER(c_char_p), c_char_p]
        c = c_char_p('123'.encode())
        dll.set_char_p(byref(c), c_char_p('abc'.encode()))
        self.assertEqual(c.value.decode(), 'abc')

    def test_set_struct(self):
        dll = self.get_dll()
        dll.set_struct.argtypes = [POINTER(Point), c_int, c_int]
        pt = Point(1, 2)
        dll.set_struct(byref(pt), 3, 4)
        self.assertEqual((pt.x, pt.y), (3, 4))

    def test_set_array(self):
        dll = self.get_dll()
        dll.set_array.argtypes = [POINTER(c_int), c_size_t, c_int]
        c_int_array_type = c_int * 10
        a = c_int_array_type()
        dll.set_array(a, c_size_t(3), 4)
        self.assertEqual(a[3], 4)

if __name__ == '__main__':
    if g_dll == None:
        current = pathlib.Path.cwd()

        logging.info(f'current: {current}')

        if current.name == 'bin':
            current = current.parent

        dll_file = current.joinpath('bin/shared_lib.dll')
        if not dll_file.exists():
            logging.error(f'{dll_file} not exists')
            exit(1)

        g_dll = ctypes.windll.LoadLibrary(dll_file)

    unittest.main()

    exit(0)
