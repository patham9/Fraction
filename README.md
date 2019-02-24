# Fraction
A strategy game written in C that builds completely on a cellular automata for all game logic, including AI's pathfinding, cloud movement, ground water propagation, water propagation, citybuilder AI, forest growth, electricity etc.
Comes with functioning multiplayer, which (the server, not the client) can be started by just opening http://91.203.212.130/Server.php in your browser and keeping it open, needs 2 clients to connect before you can play. Once you are connected you can place a base, the rest will only result in a construction site sign and will be constructed by your workers.

Original Wiki: http://91.203.212.130/fraction/

Dependency: glfw-2.7.9

![Fraction1](https://user-images.githubusercontent.com/8284677/53295826-bdac1080-37fb-11e9-876a-7c1b32ddcf46.png)
old graphics:
![Fraction2](https://user-images.githubusercontent.com/8284677/53295829-c0a70100-37fb-11e9-9eee-a90a656ee6fc.png)
fixed function pipeline:
![Fraction3](https://user-images.githubusercontent.com/8284677/53295928-a1a96e80-37fd-11e9-8a7c-71c2e51c1f8e.png)
3d Mode:
![3D mode](https://user-images.githubusercontent.com/8284677/53295975-68bdc980-37fe-11e9-94a4-08c2cc8c2c58.png)
Terrain shadows are cheap and soft, "stolen" from Iñigo ‘iq’ Quilez Elevated: https://www.iquilezles.org/www/material/function2009/function2009.pdf enjoy how beautiful this can look like in the 4KB demo: https://www.youtube.com/watch?v=jB0vBmiTr6o
Graphics was however never my goal, Fraction's goal is to have a living environment like seen nowhere else, with an ecosystem that can go wild.

Comment:
Test Chamber of OpenNARS-Lab came from Fraction. Quite interesting that the Java implementation was quite a lot more ackward to do than the C implementation with macros. Often it is said that Java is more high-level, but from what I am able to do with macros I always get the exact opposite impression, if you are a skilled coder C gives you wings.
