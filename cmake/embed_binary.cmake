# https://stackoverflow.com/a/27206982
# Creates C resources file from files in given list

# bin: path to binary file
# output: path to output file

if(NOT EXISTS ${bin})
    message(FATAL_ERROR "File ${bin} does not exist")
endif()
# Get short filename
string(REGEX MATCH "([^/]+)$" filename ${bin})
# Replace filename spaces & extension separator for C compatibility
string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
# Read hex data from file
file(READ ${bin} filedata HEX)
if(filedata STREQUAL "")
    message(FATAL_ERROR "File ${bin} is empty")
endif()
# Convert hex data for C compatibility
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
file(WRITE ${output} "extern const char _binary_${filename}[] = {${filedata}};\nextern const size_t _binary_${filename}_size = sizeof(_binary_${filename});\n\n")