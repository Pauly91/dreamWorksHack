all:
	g++ display.cpp Boid.cpp Flocking.cpp Simulation.cpp PathFinder.cpp MapLoader.cpp -std=c++0x -D__LINUX_COMPILE -o flock-solve -lopenvdb -lHalf -ltbb -lGL -lglut -lGLU -g

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/

