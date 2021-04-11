## Find all headers (files with [.h, .hpp, .H, .HPP] extension) in a set of directories
## and combine them with includes in a new header
##
## Example output:
##   #include </path/to/header1.hpp>
##   #include </path/to/header2.hpp>
##
## Takes two parameters:
##   The output header (will contain include statements to the headers found)
##   Directories to search for headers

from pathlib import Path
import argparse


def parseArguments():
    parser = argparse.ArgumentParser(
        description="Get a particular release asset id from github"
    )
    parser.add_argument(
        "--combined-header", type=str, required=True, help="The name of the output file to store the combined headers in",
    )
    parser.add_argument(
        "--includes", nargs='*', type=str, required=True, help="Include directories from which to find headers to combine",
    )
    return parser.parse_args()

def main():
    args = parseArguments()

    with open(args.combined_header, 'w') as output:
        for include_directory in args.includes:
            p = Path(include_directory)
            headers = []
            for extension in ["h", "hpp"]:
                headers += list(p.glob('**/*.{}'.format(extension)))

            for header in headers:
                output.write("#include \"{}\"\n".format(header))

if __name__ == "__main__":
    main()

