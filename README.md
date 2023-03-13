# odd raycaster
This project is a raycaster game, inspired by Wolfenstein 3D.  
Project is written in C, it uses SDL2 and pthread.  
The goal of the project is to showcase raycasting capabilities, not the playability of the game.  

## screenshots
![](/screenshots/1.png "menu.")
![](/screenshots/2.png "game.")
## features
walls, floor and ceiling rendering with textures  
head bobbing  
sprites  
directional walls  
simple lightning/fog

## build
create build directory, and inside it run:  
```cmake ..```  
```make```

### mentions
Raycasting math and techniques based on Lode's tutorial:  
https://lodev.org/cgtutor/raycasting.html.  

fizzlefade animation explained:  
https://fabiensanglard.net/fizzlefade/  

collision detection, cute_c2.h:  
https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h

### dependencies
SDL2  
SDL2-ttf
