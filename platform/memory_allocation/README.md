# Notice

Currently, the Core Memory Allocations relies on the interfaces
announced in memory_allcation.h, so if you want to redefined the behaviors
of the memory allocation, rewrite interfaces and modify the CMakeLists.txt
in the current folder.

## Step one

add a new folder which contains the defines src file for the interfaces
placing in memory_allcation.h

## Step two
add a CMakeLists.txt to operate the file you wanna do, to be noticed.
the instructions of how you should do can be seen in native/CMakeLists.txt

## Step three
modified the CMakeLists.txt in the same level
```cmake
# >>> List Methods begin
set(SUBDIR_native "native")

# Add your defines here
# Format is SUBDIR_${FOLDER_NAME} "${FOLDER_NAME}"
# Example: if you add memory_pool folder here, you should write 
# set(SUBDIR_memory_pool "memory_pool")
# Take it easy!

# <<< List Methods end
```



