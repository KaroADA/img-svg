from conan import ConanFile

class ImgSvgRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "SConsDeps"

    def requirements(self):
        self.requires("boost/1.90.0")
        self.requires("stb/cci.20240531")
        self.requires("catch2/3.14.0")

    def layout(self):
        self.folders.generators = "build/generators"
