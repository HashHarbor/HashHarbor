# HashHarbor
## Running the app
The app must be run from the terminal on both Linux and Mac
### Prebuilt Executables
Located in /src  run the OS appropriate executable \
Executables may need the execute privilege. \
Add it with ```sudo chmod +x <Executable Name>```
### Directory Structure
Make sure to follow this exact folder structure for the app to find the assets
- \ Root Folder
  - \ src
    - ```---EXECUTABLE---```
  - \ assets
    - \ characters
    - \ font
    - \ map
    - \ npc
    - \ other
    - ```---API KEY---```
## Installation
To make the executable install the necessary dependencies, then run the make file from the /src directory
### Linux Required Dependencies
Install commands vary with Linux versions
- SDL2
- OpenCV
- MongoDB cpp drivers
- OpenSSL
### Intel Mac Required Dependencies
Run these HomeBrew commands to install all the required Mac Dependencies
- ```brew install sdl2```
- ```brew install opencv```
- ```brew install mongodb```
- ```brew install mongo-cxx-driver```
- ```brew install openssl```
