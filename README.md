# flightgear_part1
cs advance programming project part_1. make the plane take off!

the project is a compiler for flightgear simulator instructions.

the project support the following commands:
-----------------------------------------------------------------------------------------------------------------------------
1. OpendataServer(port/expression to calculate port - no variable are allowed!)[\n]
open connection to flightgear in the geiven port (or expression value) and get specific values from the simulator.
**to get those, first add the attached generic_small.xml file to flightgear data/protocol directory**.
*has to be the first command!*
*****************************************************************************************************************************
2. connectControlClient("ip",port/expression to calculate port - no variable are allowed!)
open connection to flightgear and send commands when needed.
*has to be the second command!*
*****************************************************************************************************************************
3. Var command - by the following syntax:
*********************
--variable valid name has to start with letter and contain only letters, digits and _
*********************
a. Var x -> "simulator_path"
set variable x with default value of (double type)0. each time the value of x will change - the change will be sent to  flightgear simulator. (below: to_sim type variable).
*********************
b. Var x <- "simulator_path"
set variable x with default value of (double type)0. each time the value of x will be needed - it will be drawn from the given path of flightgear simulator. (below: from_sim type variable).
*********************
c. Var x = expression
set variable x with the value of the given expressin.
*****************************************************************************************************************************
4. x = expression
after variable were create with Var command (above) it's value can be change. (there is no meaning changing from_sim type variable, but it can be done).
*****************************************************************************************************************************
5. Sleep(expression)
calculate x = expression value. wait for x milliseconds before continue to next line.
*****************************************************************************************************************************
6. Print("string") or Print(expression)
printing to console the given string (if the string in-between quotation marks) or calculte the expression (if there are no quotation marks).
*****************************************************************************************************************************
7. {}
new scope. all the variable which will be defined here will be known only inside this scope.
*****************************************************************************************************************************
--condition below can be one of the two forms:
I. expression condition_operator expression (e.g.: x >= 7+y )
II. expression
in case I the return value is true or false by the given condition
in case II the return value is false if the expression value is 0, otherwise it is true
the valid condition_operators are: >, >=, <, <=, ==, !=
*****************************************************************************************************************************
8. While condition {<\n> commands <\n>}
continue to execute the commands in the scope {} as long as the condition is true
*****************************************************************************************************************************
8. If condition {<\n> commands <\n>}
execute the commands in the scope {} if the condition is true
*****************************************************************************************************************************
