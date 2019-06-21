from conan.packager import ConanMultiPackager

def build():
    builder = ConanMultiPackager(build_policy="missing")
    builder.add_common_builds(
        pure_c=False,
        shared_option_name="cpp-embedded-rc:shared"
    )
    builder.remove_build_if(lambda build: build.settings["compiler.libcxx"] == "libstdc++")
    builder.run()

if __name__ == "__main__":
    build()
