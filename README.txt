Made by Emanuel Fares -
        Omer Parnes - 


GAME SIMULATOR
==============

A simulation system for managing vehicles (trucks, state troopers, and choppers) navigating between warehouse locations.

GETTING STARTED
--------------

Launch the simulator:
./main -w depot.dat -t truckfile.txt

BASIC COMMANDS
--------------

exit                    - Exit the simulator
print                   - Display status of all vehicles and warehouses
go                      - Advance simulation time by one hour

VEHICLE CREATION
----------------

Create a state trooper:
create trooper1 State_trooper Frankfurt

Create a chopper:
create c1 Chopper (10,15)

MAP CONTROLS
------------

default                 - Reset map to default settings
size 20                 - Set map size (range: 7-30)
zoom 3.5                - Adjust zoom level
pan 0 0                 - Set map origin coordinates
show                    - Display map with all objects

VEHICLE CONTROL
---------------

Set vehicle course:
c1 course 90 200        - Set chopper c1 course to 90° at 200 km/h

Set vehicle position:
c1 position (15,20) 180 - Move chopper c1 to coordinates (15,20) at 180 km/h

Set vehicle destination:
trooper1 destination Lille    - Direct trooper to Lille warehouse
c1 destination (5,5)         - Direct chopper to coordinates (5,5)

Execute attack:
c1 attack Godzilla           - Chopper c1 attacks truck Godzilla

Stop vehicle:
c1 stop                      - Stop chopper c1

IMPORTANT NOTES
---------------

- Vehicle names must be 12 characters or less
- Choppers require speed parameter for course and position commands
- Only choppers can perform attacks on trucks
- Only choppers and state troopers support destination commands
- All coordinates are specified in kilometers
- Simulation time advances in 1-hour increments 