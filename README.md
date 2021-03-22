# PawsEngine
## Info
This is a working personal project. The purpose is to learn, and build a useable, cross-platform 2D game engine.
Currently not accepting any pull requests.

## Coding convention
* **Pointers** are only used when the client *doesn't control the life time* of the object.
* `Object` provides functions of *Reflection*. Pure data classes does not inherit `Object`.
* Inherit `ManualObject` or `ReferencedObject` instead of `Object` to indicate the method of controlling the memory life time. `ReferencedObject` are used with `ReferencePtr`, which is essentially a *intrusive pointer*.

## Used libraries
* Make system: [CMake](https://cmake.org)
* Unit testing: [DocTest](https://github.com/onqtam/doctest)
* String formating: [fmt](https://github.com/fmtlib/fmt)
* Compressing data: [zlib](https://github.com/madler/zlib)
* PNG image procressing: [libpng](https://github.com/glennrp/libpng)
* C# scripting embedding: [mono](mono-project.com)

## Referenced materials
* [Godot Engine](https://godotengine.org/)
* [The Cherno Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)