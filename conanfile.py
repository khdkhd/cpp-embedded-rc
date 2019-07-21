import os
from conans import ConanFile, CMake, tools


class JuceConan(ConanFile):
    name = "cpp-embedded-rc"
    version = "1.0.0"
    license = "ISC"
    author = "Julien Graziano (julien@graziano.fr)"
    url = "https://github.com/khdkhd/cpp-embedded-rc"
    description = "Embed binary resources in your cpp project"
    topics = ("C++", "binary", "resources")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": "False"}
    exports_sources = "*", "!compile_commands.json"
    generators = "cmake", "cmake_paths"
    build_requires = "gtest/1.8.1@bincrafters/stable"
    requires = (
        "boost_format/1.69.0@bincrafters/stable",
        "boost_program_options/1.69.0@bincrafters/stable",
        "boost_filesystem/1.69.0@bincrafters/stable"
    )

    def configure(self):
        self.options["gtest"].build_gmock = False

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=self.source_folder, defs={
            "RC_SHARED": "ON" if self.options.shared else "OFF",
            "SKIP_CONAN_INSTALL": "ON"
        })
        cmake.build()
        cmake.test()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["rc"]
        self.cpp_info.builddirs = ["bin", os.path.join("lib", "cmake")]
