#  TwinScript Language Documentation (Teensy Edition)

## Introduction

This documentation details a custom script language designed specifically for efficient execution on a microcontroller environment. Given the constraints of microcontroller systems and the critical need for performance, this language aims to minimize overhead while delivering essential scripting capabilities.

## Key Considerations

1. **Performance is Paramount**: The microcontroller on which the scripts run has other tasks to perform, so it's essential to keep the scripts lightweight and efficient.

2. **Pre-computation**: To save execution time, scripts are typically tokenized during the boot phase of the system. The result is a pre-parsed structure that can be efficiently interpreted at run-time.

3. **Run-time Execution**: During run-time, only variables, conditional constructs, and a few other elements require interpretation.

4. **Data Types and Integration**: The script language needs to support a set of specific data types (integers, floats, vec2, vec3, mat2x2, mat3x3). These data types are already implemented as C++ types in the microcontroller environment. However, the script language does not use a modular object design like C++.

5. **String Efficiency**: Currently, operations with strings are inefficient. Optimized representation and operations for supported data types are crucial.

---

## Syntax Overview

### 1. Variables:

**Definition**: 
```plaintext
VAR <name> = <value>
```

When declaring a new global variable, default values are mandatory to determine the type of the variable.

**Assignment**:
```plaintext
<name> = <value>
```

Assignation could also be use to declare local variables. If the identifier is not recognized, the interpreter will create a new local variable in the current scope.

Note : The interpreter will declare a global variable if the assignation is done in the global scope. 

**Example**:
```plaintext
VAR x = 5 #float
VAR U = [100,200] #array
VAR V = (100,200) #vec2
VAR W = (100,200, 42) #vec3
```

#### Notes :
Type are determined by the interpreter. every numbers are represented as floats.
Dynamic array's are not supported.  vec2, vec3 and matrices are.

### 2. Conditional Statements:

**If**:
```plaintext
IF <condition> THEN
    <statements>
END
```

**If-else**:
```plaintext
IF <condition> THEN
    <statements>
ELSE
    <statements>
END
```

#### Notes :
Condition are evaluated at runtime but precomputation is computed during parsing. You can use whatever math you want inside and access system constants.

### 3. Loops:

**While loop**:
```plaintext
WHILE <condition> DO
    <statements>
END
```

**For loop**:
```plaintext
FOR <name> FROM <start> TO <end> DO
    <statements>
END
```

### 4. Operators:

- Arithmetic: `+`, `-`, `*`, `/`
- Comparison: `==`, `!=`, `>`, `<`, `>=`, `<=`

### 5. Comments:

```plaintext
# This is a comment
```

### 6. Functions:

**Definition**:
```plaintext
FUNC <name> (<args>) DO
    <statements>
END
```

**Call**:
```plaintext
CALL <name> WITH <args>
```

### 7. Block Statements

#### Purpose:

`BLOCK` statements are designed for one-time conditional execution of a series of statements. If the condition of the block evaluates to `true` when the block is encountered, its statements are executed. If the condition is `false`, the execution of the block's statements is postponed to a later time. If it's determined that the condition can never become true, the block is discarded.

#### Syntax:

```plaintext
BLOCK (condition) DO
    statements
END
```

#### Example:

```plaintext
BLOCK (x > 10) DO
    print("x is greater than 10")
END
```

In this example, if the variable `x` is greater than 10 when the block is encountered, the `print` statement will execute, but only once. If the condition is not met at that point, the block's execution is deferred. If it later becomes apparent that `x` can never exceed 10, the block is discarded.

#### Execution Flow:

1. **Immediate Execution**: When a `BLOCK` is encountered, the interpreter checks its condition. If true, the block's statements execute immediately and the block is marked as executed or removed.

2. **Deferred Execution**: If the condition is false:
   - The `BLOCK` is placed in a deferred execution list.
   - As the script progresses and the system's state evolves, the interpreter occasionally re-evaluates the conditions of deferred blocks.
   - If a deferred block's condition becomes true, its statements execute once, and the block is then removed from the deferred list.

3. **Removal**: 
   - If it becomes clear that a block's condition will never evaluate to true, it's removed from the deferred list to conserve resources.

#### Notes:

- **One-time Execution**: A `BLOCK` executes its statements only once. After its execution, whether immediate or deferred, it won't be reconsidered even if the condition becomes true again.

- **Efficiency**: In a microcontroller environment, performance is critical. Efficiently managing deferred blocks ensures that the interpreter doesn't use unnecessary resources.

- **Circular Dependencies**: Caution is needed for blocks with conditions reliant on variables or states affected by other blocks. This might create scenarios where the order of block evaluation becomes crucial.

- **Max Deferred Blocks**: To prevent potential resource exhaustion, consider setting a limit on the number of blocks that can be deferred.


---

## Implementation Guidance

1. **Tokenizer**: 
    - During the boot phase, tokenize the script.
    - Convert variable names, function names, and other symbols to unique token IDs. Store them in a compact table.
    - Store literals and constants in a separate table.

2. **Interpreter Design**:
    - Utilize a simple switch-case structure based on token IDs, ensuring fast interpretation.
    - For supported data types, directly use their C++ implementations where possible, minimizing overhead.
    - Avoid string manipulations at runtime. If a conversion is necessary, use efficient lookup tables or direct mapping.

3. **Memory Management**:
    - Given the constrained environment, be mindful of memory allocation and deallocation. If possible, pre-allocate memory chunks during the boot phase and reuse them during runtime.

4. **Error Handling**:
    - Use simple error codes for common issues, like "UNKNOWN VARIABLE", "SYNTAX ERROR", etc. Consider using compact numeric codes for faster error handling.

### 8. System constants and functions

//WIP
