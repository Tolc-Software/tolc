#!/bin/sh
## Find all headers (files with [.h, .hpp, .H, .HPP] extension) in a set of directories
## and combine them with includes in a new header
## Takes two parameters:
##   $1                 - The output header (containing include statements to the rest)
##   Rest of parameters - Directories to search for headers

# Exit on any error
set -o errexit
# Print and exit on undefined variable
set -o nounset

# Output
combinedHeader="$1"
shift
# Rest of parameters are directories to search for headers in
includes="$*"

echo '' > "${combinedHeader}"
for include in ${includes}; do
	find "$include" \( -iname '*.h' -o -iname '*.hpp' \) -print0 | xargs -0 -I {} echo '#include <{}>' >> "${combinedHeader}"
done
