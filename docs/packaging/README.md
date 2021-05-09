# tolc documentation #

This is where the documentation lives that gets showed on the site. It uses [mkdocs](https://www.mkdocs.org/) together with [the material theme.](https://squidfunk.github.io/mkdocs-material/getting-started/). Both need to be installed to build the documentation.

## Installation ##

Installing the material theme automatically installs `mkdocs` as a dependency:

```shell
$ pip install mkdocs-material
```

## Usage ##

To serve the page during development:

```shell
$ mkdocs serve
```

To build the finished documentation for packaging:

```shell
$ mkdocs build
```

The static page will be available under the `site` directory.
