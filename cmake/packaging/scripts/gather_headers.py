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
        description="Gather headers to one amalgamated header"
    )
    parser.add_argument(
        "--combined-header", type=str, required=True, help="The name of the output file to store the combined headers in",
    )
    parser.add_argument(
        "--includes", nargs='*', type=str, required=True, help="Include directories from which to find headers to combine",
    )
    parser.add_argument(
        "--extra-headers", nargs='*', type=str, required=False, help="Extra headers that gets added no matter what",
    )
    parser.add_argument(
        "--do-not-search-for-headers", action='store_true', required=False, help="Override to not search for headers",
    )

    return parser.parse_args()

def main():
    args = parseArguments()
    print(args)

    # Make sure the directory of the output exists
    Path(args.combined_header).parent.mkdir(parents=True, exist_ok=True)
    with open(args.combined_header, 'w') as output:
        if not args.do_not_search_for_headers:
            for include_directory in args.includes:
                p = Path(include_directory)
                headers = []
                for extension in ["h", "hpp"]:
                    headers += list(p.glob('**/*.{}'.format(extension)))

                for header in headers:
                    output.write("#include \"{}\"\n".format(header))

        if args.extra_headers:
            for header in args.extra_headers:
                # Make sure the path is absolute
                output.write("#include \"{}\"\n".format(Path.absolute(Path(header))))

if __name__ == "__main__":
    main()
