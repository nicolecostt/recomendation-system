from setuptools import setup, Extension
import pybind11

ext = Extension(
    "pyrecommend",
    sources=["bindings.cpp", "read.cpp", "matrix.cpp", "recommend.cpp"],
    include_dirs=[pybind11.get_include()],
    language="c++",
)

setup(
    name="pyrecommend",
    ext_modules=[ext],
)
