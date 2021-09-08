# Contributing

To ensure uniformity throughout our codebase, we have a few rules:

### Branches
1. Any new features must be in their own branch, which will branch from the `development` branch.
2. Feature branches should be named as `feature/<feature-name>`. For example, if we are adding some collision detection features, then the branch should be called `feature/collision-detection`.
3. There should only be one feature per branch.
4. Changes to documentation (adding stuff, rewording sentences, adding/fixing links, etc.) can be committed directly on the development branch. There is no need to create a separate branch for this.
5. After a feature is done, a pull request must be submitted on the `development` branch for review.
6. Before merging a pull request, make sure that it passes every single CI test.
7. Commits that consist solely of changes/additions to the documentation should skip the CI tests (as the tests don't add any value to the commit).
8. To skip the CI testing, simply prepend `[skip ci]` in the commit message.

#### Header Files and Classes 
1. Header files go in the `include` directory, and source files go in the `src` directory.
2. Don't use `using namespace std` in the global scope in header files. If it is necessary (because of very long type names), try to restrict it to the smallest scope possible.
3. [Type aliasing](https://en.cppreference.com/w/cpp/language/type_alias), on the other hand, is encouraged, especially if it improves readability of the code.
4. Make sure to indent with a tab before using an access control specifier, and then indent with another tab before defining member variables and functions. Example:
    ```
    class Foo
    {
    	Foo() = default;
    	private:
    		int bar;
    };
    ```
5. In classes, prefix all member variables with the `m_` prefix. For example, if the class has a member variable for position, it should be called `m_position`.
6. Member variables and member functions should use camelCase. That is, if the variable name consists of multiple words, the first letter of the word should be lowercase, and the first letter of all subsequent words should be uppercase. Examples: `m_angularVelocity`, `getAngularVelocity()`, `m_gravity`, `getGravity()`, `m_transformComponent`.
7. Class names should use PascalCase. That is, if the class name consists of multiple words, the first letter of every word should be in uppercase. Examples: `PhysicsWorld`, `RigidBody`. 
8. Getters and setters for classes should be marked as `inline` and be placed in the header file.
9. Getters should also use the `[[nodiscard]]` attribute.

### Unit Tests
1. Branches that add tests to new code should be named `tests/<test-name>`. For example, if we are adding some tests to test the collision detection features, then we might call the branch `tests/collision-tests`.
2. Test names should be in PascalCase.
3. All source files in their respective`tests/` directories should have `_tests` appended to the file name, to avoid name clashes. Example: If we write tests for `rigidbody.cpp`, then the test file should be called `rigidbody_tests.cpp`.

### Code Style
#### Documentation
1. Documentation in source code should be done in Doxygen style.
2. Don't document self-explanatory functions like `getPosition()`, `getVelocity()`, etc.
    - In general, getters and setters shouldn't be documented.
3. When writing a comment, make sure that there is nothing after `/**`. Everything that forms the body of the comment should go to the lines after the `/**`. Example:
    ```
    /**
     * @brief An example of how comments should be written.
     *
     * Notice how there is nothing after the /** on the first line.
     * Some random comment text to fill up more space.
     */
    ```
    - Here is a **non**-example, with the same text as before:
    ```
    /** @brief An example of how comments should be written.
     *
     * Notice how there is nothing after the /** on the first line.
     * Some random comment text to fill up more space.
     */
    ```
    - And another one:
    ```
    /** @brief An example of how comments should be written.
     *
     *  Notice how there is nothing after the /** on the first line.
     *  Some random comment text to fill up more space.
     */
    ```

#### General Programming Conventions
1. Curly braces should be on new lines.
2. Source file names should be in all lowercase. They should typically be one word (things like `physicsworld.*` and `rigidbody.*` are fine). If you find that your source file name consists of multiple names, that might be indicative of bad design.
3. Use tabs instead of spaces. Make sure that all tabs are equal to 4 spaces.
4. Put spaces around binary operators. For example, write `a += b` and not `a+=b`. The former looks less cluttered.
5. Use blank lines to logically separate portions of code.
6. Function definitions should have at least one blank line after them (an exception being if it is the last function in a class definition).
7. There should be a maximum of 80 characters per line. Break up longer lines of code into smaller pieces and indent them properly to make them easier to read.
8. For functions with empty bodies (for example, certain constructors), put the `{` and `}` on separate lines. Example:
    ```
    Foo::Foo(int a, int b) : m_a(a), m_b(b)
    {
    }
    ```
