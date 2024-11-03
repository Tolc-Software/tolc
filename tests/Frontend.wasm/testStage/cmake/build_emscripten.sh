#!/bin/sh
## Maintainer: Simon Rydell
## Date created: Feb 07, 2022

# Exit on any error
set -o errexit
# Print and exit on undefined variable
set -o nounset

echo "======================"
emsdk_path="$1"
shift
echo "emsdk_path=${emsdk_path}"
version="$1"
shift
echo "version=${version}"
out_dir="$1"
shift
echo "out_dir=${out_dir}"
module_name="$1"
shift
echo "module_name=${module_name}"
sources="$*"
shift
echo "sources=${sources}"
echo "======================"

# Download and install the ${version} of SDK tools.
"${emsdk_path}/emsdk" install "${version}" > /dev/null 2>&1

# Make the "${version}" SDK "active" for the current user. (writes .emscripten file)
"${emsdk_path}/emsdk" activate "${version}" > /dev/null 2>&1

# Activate PATH and other environment variables in the current terminal
. "${emsdk_path}/emsdk_env.sh" > /dev/null 2>&1

# Actually build the code
em++ -lembind  -o "${out_dir}/${module_name}.js" ${sources}
