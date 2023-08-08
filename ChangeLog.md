Change Log
==========

All relevant changes to the project are documented in this file.


[v4.2][] - 2023-08-08
---------------------

### Changes
 - Update README and manual to focus on the story and game, the origin
   has its own place and need not be repeated everywhere.  Lead with
   the classic game intro.
 - Update maintainer surname and project name: advent4 -> adventure
 - Add `map.jpg` from https://rickadams.org/adventure/e_downloads.html
 - Add `Dockerfile` and auto-builder for tiny containers
 - Shrink stack usage for `gettrav()`
 - Log save game filename in error and status messages

### Fixes
 - Fix out-of-bounds problems reported in issue #2
   - Some of these fixes affect game play, e.g. moving things should now
     actually work, in particular for "odd" objects.  (What isn't odd in
     the Colossal Cave?)
 - Fix lots of other out-of-bounds issues found by Coverity Scan
 - Use `SEEK_SET` with `fseek()` instead of `0`
 - Fix missing game data when built with `--disable-builtin`


[v4.1][] - 2020-07-05
---------------------

### Changes

 - Change narrative back to original, start outside building
 - Change prompt slightly, add space after `>`
 - Change save game default to use `~/.adventure`
 - Add `.desktop` entry
 - Initial Debian packaging

### Fixes

 - Fix verb matching that broke in 4.0, issue #1
 - Fix game restore that broke in 4.0


[v4.0][] - 2018-11-25
---------------------

Initial release, based on an import from uClinux-dist-20080808.

### Changes
 - Code cleanup, ANSI-fication, reformat using Linux coding style

### Fixes
 - Fixed compiler warnings from modern C compiler

[UNRELEASED]: https://github.com/troglobit/sysklogd/compare/v4.1...HEAD
[v4.2]:       https://github.com/troglobit/sysklogd/compare/v4.1...v4.2
[v4.1]:       https://github.com/troglobit/sysklogd/compare/v4.0...v4.1
[v4.0]:       https://github.com/troglobit/sysklogd/compare/BASE...v4.0
