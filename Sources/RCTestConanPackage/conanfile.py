import os

from conans import ConanFile, CMake, tools


class CppembeddedrcTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "cmake_paths", "virtualenv"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        activate_cmd = "activate" if self.settings.os == "Windows" else "true"
        self.run("{} && ctest -C Release".format(activate_cmd))
