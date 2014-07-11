x64dbg-StaticAnalysis
=====================

This is a plugin for the project x64_dbg. It detect win32api calls and add useful comments at found parameters for easier analysis.
Currently it supports x86 targets only. 

See [release page](https://github.com/x64dbg/x64dbg-StaticAnalysis/release) for lastest compiled plugin. The source of the latest release is in the branch "stable".

This master branch may contains experimental code and the latest commits.

## implemented Features
- auto comments on parameters for api-function
- real stack emulator for MinGW-arguments detection
- real register emulator for x64 calling convention

## Roadmap

- version for x64 targets
- function-finder (function body, xrefs, function returns)
- case-switch detection
- "MB_OK"-like symbols
- loop-detection
- flow-heuristic against obfuscation instead of linear scanning

## License
GLPv3
