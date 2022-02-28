# FAQ #

## A note on const within containers ##

As of writing `Embind` does not support const within the template parameter list of most standard containers (e.g. `std::map<int, const std::string>` or `std::pair<const int, int>`).
These will be ignored from the output.
