# Usage

The current `crc_module.so` is compiled on arm64 cpu and python3.8 (compatible with the default G1 environment). Please compile the library yourself if you are running on a different platform/python environment.

To use it in your Python script (both sdk and ros), please put this file to a directory included in `$PYTHONPATH` and refer to `test_crc.py` for an example. You can also put `crc_module.pyi` to the same directory as `crc_module.so` to enable python type hint.

# Compiling

Activate the correct python environment and use `pip install pybind11` to install pybind.
Find the pybind cmake path and replace `/home/unitree/.local/lib/python3.8/site-packages/pybind11/share/cmake/pybind11` in CMakeLists.txt

```bash
cd python
mkdir build && cd build
cmake ..
make
```

# NOTE
When compiling on MAC, you need to manually specify the python include path and pybind11 path.
Use `cmake` command as follows
```bash
cmake -Dpybind11_DIR=/opt/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11 -DPYTHON_INCLUDE_DIRS=/opt/miniconda3/include/python3.12 ..
```
```bash
cmake -Dpybind11_DIR=/Users/leo/.local/share/mamba/envs/ros_env/lib/python3.11/site-packages/pybind11/share/cmake/pybind11 -DPYTHON_INCLUDE_DIRS=/Users/leo/.local/share/mamba/envs/ros_env/include/python3.11 ..
```

# TODO: check how to access `unitree/idl/hg/LowCmd_.hpp` refer to thinkpad laptop