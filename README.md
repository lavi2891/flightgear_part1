# flightgear_part1
cs advance programming project part_1. make the plane take off!  

This project is a compiler for flightgear simulator instructions.  
## How to use:
Write txt file with instructions (below you can read the supported commands)  
Compile -> run the file with path to your tx file as argument -> open flightgear
Compilation: open terminal in the files' folder and run ```g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread``` later run ```./a.out file_path``` and open the simulator.
In simulator setting -> additional settings box, writes those lines before launch:
```
--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small   
--telnet=socket,in,10,127.0.0.1,5402,tcp
```  


the project support the following commands:
-----------------------------------------------------------------------------------------------------------------------------
### 1. openDataServer(port/expression to calculate port - no variable are allowed!)  
open connection to flightgear in the geiven port (or expression value) and get specific values from the simulator.  
**to get those, first add the attached generic_small.xml file to flightgear data/protocol directory**.  
**has to be the first command!**.  
e.g.: ```openDataServer(8080)```  or  ```openDataServer(8000 + 80)```
*****************************************************************************************************************************
### 2. connectControlClient("ip",port/expression to calculate port - no variable are allowed!)  
open connection to flightgear and send commands when needed.  
**has to be the second command!**.  
e.g.: ```connectControlClient("1.2.3.4",8081)``` or ```connectControlClient("1.2.3.4",8000 + 81)```
*****************************************************************************************************************************
### 3. var command - by the following syntax:  
  
**variable valid name has to start with letter and contain only letters, digits and _**  
a. var x -> sim("simulator_path")  
set variable x with default value of (double type)0. each time the value of x will change - the change will be sent to  flightgear simulator. (below: to_sim type variable).  
e.g.: ```var x -> sim("/sim/time/warp")```
  
b. var x <- "simulator_path"  
set variable x with default value of (double type)0. each time the value of x will be needed - it will be drawn from the given path of flightgear simulator. (below: from_sim type variable).  
e.g.: ```var x <- sim("/sim/time/warp")```
  
c. Var x = expression  
set variable x with the value of the given expressin.  
 e.g.: ```var x = 1``` or ```Var x = y + 3```
*****************************************************************************************************************************
### 4. x = expression  
after variable were create with Var command (above) it's value can be change. (there is no meaning changing from_sim type variable, but it can be done).  
e.g.: ```x = y + 7```

*****************************************************************************************************************************
### 5. Sleep(expression)  
calculate x = expression value. wait for x milliseconds before continue to next line.  
e.g.: ```Sleep(3000)``` or ```Sleep(x*100)```
*****************************************************************************************************************************
### 6. Print("string") or Print(expression)  
printing to console the given string (if the string in-between quotation marks) or calculte the expression (if there are no quotation marks).  
e.g.: ```Print("hello world!")``` or ```Print(x + 2)```
*****************************************************************************************************************************
### 7. {}  
new scope. all the variable which will be defined here will be known only inside this scope.  
e.g.:
```
{    
....  
}
```
*****************************************************************************************************************************
- condition below can be one of the two forms:  
  1. expression condition_operator expression (e.g.: x >= 7+y )
  2. expression
  - in case 1 the return value is true or false by the given condition
  - in case 2 the return value is false if the expression value is 0, otherwise it is true
the valid condition_operators are: >, >=, <, <=, ==, !=
*****************************************************************************************************************************
### 8. while condition {<\n> commands <\n>}  
continue to execute the commands in the scope {} as long as the condition is true  
e.g.:
```
while x < 1000 {    
....  
}
````
*****************************************************************************************************************************
### 8. If condition {<\n> commands <\n>}  
execute the commands in the scope {} if the condition is true  
e.g.:
```
if x < 1000 {    
....  
}
````
*****************************************************************************************************************************
