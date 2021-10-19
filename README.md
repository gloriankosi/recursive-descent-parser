# CS610 Assignment #2
Consider the following expression BNF:

- <expression>  ::=  <factor>  * <expression>   |   <factor>  /  <expression>   |   <factor>
- <factor>  :==  <term> + <factor>  |  <term> - <factor>  |  <term>
- <term>  ::=  {  <expression>  }  |  <literal>
- <literal>  ::=  0|1|2|3|4|5|6|7|8|9
  
Using recursive descent, and only recursive descent, scan expressions that adhere to this BNF to build their expression tree; write an integer valued function that scans the tree to evaluate the expression represented by the tree.

There are plenty of clever programs online that you can download to evaluate arithmetic expression tree; if you want zero in this assignment, download one and submit it as programming assignment #2; if you want a grade greater than zero, please follow our instructions. Thanks.

Input:

A numeric expression adhering to this BNF.
Output:

Some representation of the expression tree.
The result of evaluating the expression.
