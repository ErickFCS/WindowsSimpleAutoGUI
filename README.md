# Windows Simple Auto GUI

This simple app allows you to automate simple i/o mouse and keyboard actions in your computer. It uses a config file with the instructions.

## Status

Ended

## Why?

When you need to repeat simple actions in your pc. And the context does not change.

## Instalation

For compiling I used Ninja, but you can use any Generator of your preference. Just be careful with the path of the binary!
```sh
   cd WindowsSimpleAutoGui
   mkdir build
   cd build
   cmake .. -G Ninja
   cmake --build .
```

## Use Process

Create a config file with the desired instructions. Then run the ./build/autoClicker.exe play "path/to/the/config/file"
You can also record actions with: ./build/autoClicker.exe record "path/to/the/new/config/file"

## Usage documentation

Basic usage examples:

- Playing from a configFile

```sh
   ./build/autoClicker.exe play ./configFile
```
- Recording user actions

```sh
   ./build/autoClicker.exe record ./newConfig
```

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE.md) file for details.
