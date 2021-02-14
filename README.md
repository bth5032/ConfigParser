# ConfigParser
Simple C++ class that supports reading plain text configuration files on the fly. 

## Usage

To use configparser simply include the `ConfigParser.C` file in your project and initialize a ConfigParser object passing in the path to the configuration file. 

```ConfigParser conf("path_to_conf_file.conf")```

Load the configuration you want in the file with `conf.loadConfig("CONF_NAME")`, which will then allow you to access the keys with ```conf->get("KEY_NAME")``` which always returns a string that can be processed by your code.

An example of a config file is `test.conf` and example of reading that file in several different ways is in `test.C`.

### Config spec

A single file can hold many different "configurations", each configuration is defined by the special tag `Name`. A config has a `Name` tag and is followed by a list of line seperated keys. When the config is loaded, you can access the values for those keys using `conf->get("KEY_NAME")`. 

Keys and values are seperated by the first instance of the `=` symbol, meaning it is not possible to have a key with that symbol, but it can be included in the value. Each line below the `NAME=CONFIGNAME` line is associated with `CONFIGNAME` until the first blank line or another `NAME=` tag (e.g. the start of the next config).

An example config is given in `test.conf`.

#### Default features

For convinience, you can have default values that work for many configurations. When a line starts with the special string ```DEFAULT::```, the key-value pair is applied to all the configurations below that point in the file until the default value is overwritten. If the same key appears in a configuration it will overwrite any default values.

## Access Methods

The configs in a file can be loaded by name using `conf.loadConfig("CONF_NAME")` or sequentially using `conf.loadNextConfig()`. Examples of both methods are in `test.C`
