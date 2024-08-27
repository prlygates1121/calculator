# Calculator

## Syntax
- **Expression**(E): `T {"+"|"-" T}`
  - An expression consists of one or more terms separated by "+" or "-".
  
- **Term**(T): `F {"*"|"/" F}`
  - A term consists of one or more factors separated by "*" or "/".
  
- **Factor**(F): `Identifier | Double | (E) | -F`
  
  - A factor is either an identifier, a double, an expression inside brackets or a negative factor.
  
    An identifier is a char string starting with a letter and consisting of only letters and digits.

## Classes

Class `TreeNode` has the following subclasses: `Double`, `Identifier`, `Add`, `Sub`, `Mul`, `Div`, `Negate`.

- Infix operators, like `Add`, own two pointers of `TreeNode` type that point to the operands to their left and their right.
- Postfix operators, like `Negate`, own a pointer of `TreeNode` type that points to the argument negated.
- `Double` owns a double type value.
- `Identifier` owns a value and a char pointer representing its name.

All the subclasses inherit function `print()` and `eval()` from `TreeNode`.



## Tokenization

Function `scanToken()` reads one character from the input and saves it in global variable `nextToken`.





## Program Organization



