# MovingColoredShapes


Installation requirements:

```bash
brew install cppcheck, clang-format
```


To sync C++ static analysis with the Cmake C++ Configuration (and do not create a new .json),
add this to vscode -> setting.json:

```
"C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
```