# Physisim

Physisim is a physics simulator written in C++.

![physisim.png](image\physisim.png)

## Setup

After cloning the repo, raylib has to be added to the project. To do this quick simply run the setup.ps1 script in powershell. 

Make sure to change to execution policy to enable running unsigned scripts. Enable running unsigned scripts by entering this command as an administrator into powershell.

```powershell
set-executionpolicy remotesigned
```

## Build

After running the setup script, open the solution (physisim.sln).

You can build and run the project with Visual Studio 2022.

The executable will be placed into the bin folder.

## Controls

* You can navigate through space using WASD and look around using the mouse. 

* The Simulation can be closed by pressing ESC.

* Pressing P pauses the simulation. You can still navigate around and inspect the state variables of a body.

There are 3 ways to interact with the objects using your mouse:

* By pressing the left mouse button, you attach a spring from your crosshair to the rigidbody. The force exerted on the body grows with the lenght of the spring.

* By pressing the right mouse button you can grip the body and freely move it around. You can also vary the distance at wich you hold it by scrolling the mouse wheel.

* By pressing the middle mouse button you can inspect the bodys state (e.g it's velocity or position)

## HUD

The HUD shows you some information about the simulation.

* The Camera postition in the top left corner

* The simulation time, since it was started

* The total energy of the system (not really useful right now)
