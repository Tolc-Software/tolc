# Releases #

There are two types of releases for this project:

1. The `latest` release.
  * Considered stable-ish.
  * Built from each pull request merged and updated continuously.
2. Tagged releases (e.g. `v0.3.1`).
  * Created each time a stable build is done.

## How to create a tagged release ##

1. Update the version number in `CMakeLists.txt`.
  * This will, when building, populate the `VERSION` file in the root of this project.
2. Create the file `docs/ReleaseNotes/$(cat VERSION).md` containing the release notes and add it.
  * This file will automatically be used as the release notes for that version.
  * Ideally this file will be continuously updated as time goes on.
3. Create a pull request with the changes.
4. When passing and merged the following will happen:
  * A new tag will be created as `$(cat VERSION)`
  * A new github Release will be created from that tag
  * The `cpack` generated file will be added to the release aswell as the `latest` release.
