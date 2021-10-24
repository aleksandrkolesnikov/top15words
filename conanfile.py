from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps
#from conan.tools.microsoft import MSBuildToolchain, MSBuildDeps

class Top15WordsConan(ConanFile):
    name = "Top15Words"
    settings = "build_type", "arch", "compiler", "os"

    def requirements(self):
        self.requires("qt/5.15.2")
        self.options["qt"].shared = True
        self.options["qt"].qtcharts = True
        self.options["qt"].qtdeclarative = True
        self.options["qt"].qtquickcontrols = True
        self.options["qt"].qtquickcontrols2 = True
        self.options["qt"].with_pq = False
        self.options["qt"].with_sqlite3 = False


    def generate(self):

        cmdeps = CMakeDeps(self)
        cmdeps.generate()

        cmtch = CMakeToolchain(self)
        cmtch.generate()

    