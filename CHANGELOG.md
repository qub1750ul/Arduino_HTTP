# Change Log  
All notable changes to this project will be documented in this file.  

The format is based on [Keep a Changelog](http://keepachangelog.com/)  
and this project adheres to [Semantic Versioning](http://semver.org/).  

[![Keep a Changelog specification badge](https://img.shields.io/badge/Keep%20a%20Changelog%20Specification-1.0.0-orange.svg)](http://keepachangelog.com)
[![Semantic Versioning specification badge](https://img.shields.io/badge/Semantic%20Versioning%20Specification-2.0.0-orange.svg)](http://semver.org)

## [Unreleased] ##
### Added ###

- This Changelog
- Event log functionality

### Fixed ###

- Missing 50x Server Error status codes are now available
- A bug that generated a BAD_REQUEST response if the request method is not implemented by the server
- A bug that generated a link-time error when using deserialization functions

## [1.0.0] - 2018-06-15 ##

First stable release.
Only Server and parsing functionality available.
