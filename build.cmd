python -m venv .venv
call .venv/Scripts/activate
pip install conan
conan install . -if build -pr:b=default -pr:h=default --build=missing
cd build
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .