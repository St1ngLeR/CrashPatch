# CrashPatch

<img width="2560" height="800" alt="CrashPatch_Logo" src="https://github.com/user-attachments/assets/05c8ccd4-6a72-4fcf-a75c-8be4a409b17c"/>

CrashPatch for "classic" Crashday 1.2 is the first release to provide a large number of QoL fixes for the game. CrashPatch solves a wide range of problems, from fixing critical failures, to various major and minor improvements to the game to ensure a comfortable game in modern realities, which makes the choice optimal for both new and old players.

## Fixes, improvements & additions

Changes marked with ⚙️ can be configured/toggled via the INI file. These options are enabled by default unless stated otherwise.

- ⚙️ Added rotating animation for skybox (can be enabled in the config).
- The game now shows screen vignette (filter) during regular gameplay.
- Skipped "Press ENTER to continue..." in the event loading screen.
- Fixed launch parameters. "window" paramater is properly enables windowed mode.
- Game path is runtime read by application folder and not by "crshpath.ini" file.
- Removed the creation of the empty "screen.log" file.
- Removed the dependency on the "rpmtex.tga" file.
- Increased height limit for automatic car respawning.
- Fixed track editor crash when trying to select a hidden tile.
- Added no damage option for all game modes except for the "Wrecking Match".
- The logic of the "Air Time" game mode has been changed so that the air time affects the players position.
- Restored "Oil on tires" feature. Enables when the car is overlapping the "oil" surface on tiles.
- Removed outdated label elements in the track load screen ("THE ICETRACK (By Moon Byte)").
- Fixed the interruption of the fade transition between tracks before exiting the track selection menu.
- ⚙️ Number plates for AI opponents are randomly selected (can be disabled in the config).
- Fixed the disappearance of car wheels and axles out of sight.
- The car icons on the radar are using car color instead white and gray.
- ⚙️ Added the possibiliy to use "Test Drive" game mode in multiplayer (can be enabled in the config).
- The global RNG seed is randomized (disabled for multiplayer).
- The calculated value is used for car explosion damage instead of the static value.
- Afterburner parameters are read from the "aftbur.dbs" file.
- Fixed a bug that lets you steal positions by wrecking other cars after they finish a race.
- Fixed the constant appending line break for mission objective text when restarting the race.
- Fixed a bug that resets the checkpoint counter in the "Bomb Run" mode HUD.
- ⚙️ Fixed a bug occured when driving in reverse and fastly rotate the car (can be disabled in the config).
- ⚙️ Fixed maximum car speed bug (can be disabled in the config).
- All installed cars are allowed for multiplayer.
- Added ambience sound effect support.
- The save file size limit has been increased to 4 MB (the game would freeze on startup if the memory was full).
- Added support for changing the keyboard layout.
- Removed the freeze when the window is out of focus (fixes the "rewind time" bug).
- Added bomb display in the "Vehicle Blast" mini-game.
- The view range has been increased so that the entire map area is drawn on the screen.
- Added behavior for the "Pass The Bomb" mode, in which the player cannot control the car before the start of the round and at its end.
- Added respawn after wrecking in the "Test Drive" mode.
- Added the option to play in the "Bomb Run" mode without opponents.
- Fixed incorrect display of Point-To-Point maps ("This track is not available for Race." message appearing).
- Fixed a crash with an error when choosing a car tuning ("line == Garage.List.CurrLine" error).
- Removed the 1px high empty space on top of the finish screen.
- Fixed color picker mouse issue when returning to the main garage screen while changing color.
- Removed support for hiding car parts at a certain angle ("--- VISIBILITIES ---" section in "carinfo.cca").
- The maximum terrain slope limit has been removed in the track editor.
- Restored "Turn on/off in-game HUD" feature in replays.
- Added smooth fading of the title screen.
- The calculation time of LOD models has been decreased to hide switch artifacts (i.e. during fast movement).
- Added a mode setting screen for "Test Drive".
- Added hiding/showing the GUI in the menus via Ctrl+Shift+H.
- Unhidden "Master Server Address" input box in the "Network Settings" page.
- Track editor help window is hidden on opening.
- ⚙️ Restored the display of the nickname in the main menu (can be enabled in the config).

## Install & usage
To make ASI plugin work in the game you need to use [ThirteenAG's Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) and put plugin files in to game directory or folders mentioned in ThirteenAG's loader GitHub page.

CrashPatch works automatically out of the box, but you can change the settings offered inside the `CrashPatch.ini` file.
