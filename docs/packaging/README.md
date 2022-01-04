# tolc documentation #

This is where the documentation lives that gets showed on the site. It uses [mkdocs](https://www.mkdocs.org/) together with [the `tolc` theme](./tolc_theme).

## Installation ##

Installing the `mkdocs`:

```shell
$ pip install mkdocs
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

## Deploying the site ##

**NOTE:** This is done by the CI/CD system and should not be done manually unless explicitly intended.

```shell
$ mkdocs gh-deploy --strict --config-file ./mkdocs.yml
```

This will push a commit to the `gh-pages` branch on the repository containing the content of the `site` directory.

