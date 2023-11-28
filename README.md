# UnicycleRobotSimulation
School project to apply control laws on a unycle robot on matlab

# Matlab Code
The matlab code is used to generate a trajectory to follow and simulate a robot that tries to follow that trajectory.
Different types of noise are added on the motors, sensors, environnement and trajectory (as if the robot was remote controled) to add realism to the simulation.
The "realism level" can be either : 0, 1, 2, 3, 4. It can be changed in main.m with the variable "niveau".

The trajectory to follow can be saved with the function _savetrajectory_.
It is used in main.m with the line (which needs to be uncommented)
savetrajectory(niveau, trajectoire, postureRelle);

# C++ code (with SFML)
The c++ code is for to display the robot following the trajectory (delayed, not live). It uses the saved trajectories in binSave.
You can choose which "realism level" to display with the variable "N" in main.cpp
