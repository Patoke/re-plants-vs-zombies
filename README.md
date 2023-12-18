
# re-plants-vs-zombies

A project focused on decompiling the latest functionality from the first PvZ title and expand upon the game and its engine

The SexyAppFramework dating as back as 2005 is a very old game engine and it does not follow proper C++ conventions as per modern standards nor does it use a modern renderer backend

This project aims to modernize the engine by using features from the latest C++ standards aswell as replacing the old legacy DirectDraw and Direct3D7 renderers for the modern [GLFW](https://www.glfw.org/) cross-platform wrapper aswell as expanding upon an old (now deleted) decompilation project of PvZ version 0.9.9 by [Miya aka Kopie](https://github.com/rspforhp) to get the best possible PvZ experience both for modders and players alike

# DISCLAIMER

This project does not condone piracy

This project does not include any IP from PopCap outside of their open source game engine, this will only output the executable for a decompiled, fan version of PvZ

To play the game using this project you need to have access to the original game files by [purchasing it](https://store.steampowered.com/app/3590/Plants_vs_Zombies_GOTY_Edition/)

## Roadmap

#### Currently focused on
- [x] Add x64 support for the base game **(Partial)**
- [ ] Replace the old renderer backend for GLFW **(WIP)**
- [ ] Replace all Windows only code for cross-platform GLFW counterparts **(WIP)**

#### Left for when we have a working x64 build using GLFW
- [ ] Add all functionality from the GOTY version of the game
  - [x] Achievements **(Partial)**
  - [ ] Zombatar

#### Possible future features
- [ ] Create an easy to use modding API for the game
  - [ ] Parse zombies from files
  - [ ] Parse plants from files
  - [ ] Parse maps from files
  - [ ] Add scripting for custom sequences

## Installation

### Visual Studio Community

Open the folder containing the `CMakeSettings.json`, wait until cache finishes generating and build the project

### Other (Sublime, Visual Studio Code, etc..)

Run the following commands (assuming you have CMake installed with Ninja) where the `CMakeSettings.json` file is located

`cmake -G Ninja -B cmake-build`

`cmake --build cmake-build`

If running these commands does not create a successful build please [create an issue](https://github.com/Patoke/re-plants-vs-zombies/issue) and detail your problem

After you build, the output executable should be in the `Debug` or `Release` (depending on your build target) folder inside `SexyAppFramework`

Then you want to copy that executable inside of the original game's root folder (or copy the contents of the original game folder inside the previously mentioned folder)

After that you should be able to just open the built executable and enjoy re-pvz!

## Contributing

When contributing please follow the following guides:

<details><summary>SexyAppFramework coding philosophy</summary>

#### From the SexyAppFramework docs:

<br>
The framework differs from many other APIs in that some class properties are not wrapped in accessor methods, but rather are made to be accessed directly through public member data.   The window caption of your application, for example, is set by assigning a value to the std::string mTitle in the application object before the application’s window is created.  We felt that in many cases this reduced the code required to implement a class.  Also of note is the prefix notation used on variables: “m” denotes a class member, “the” denotes a parameter passed to a method or function, and “a” denotes a local variable.
</br>
</details>

<details><summary>Contributor markings</summary>

<br>
Whenever you need to leave a comment for other developers to find you should do so with the following grammar:

* Always include the name of the contributor as in:
  * `@Contributor`
* For todos include the todo marking as in:
  * `@Contributor todo`
* Always add a colon to specify that the start of the comment starts there
  * `@Contributor todo: Thing went wrong!`
* If a new function has been reversed and you have found the address in the latest version of the game (or have reversed a certain class member offset) please note it as follows:
  * `@Contributor GOTY: 0xADDRESS`
</br>
</details>


## Thanks to

- [@rspforhp](https://www.github.com/octokatherine) for their amazing work decompiling the 0.9.9 version of PvZ
- [@ruslan831](https://github.com/ruslan831) for archiving the [0.9.9 decompilation of PvZ](https://github.com/ruslan831/PlantsVsZombies-decompilation)
- The GLFW team for their amazing work
- PopCap for creating the amazing PvZ franchise (and making their game engine public)
- All the contributors which have worked or are actively working in this amazing project
