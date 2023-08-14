# Matrix-Calculator
Programmed a calculator that computes matrix expressions such as addition, multiplication, and transposition.

<h2>Description</h2>
When working with matrices, students often have to spend lots of time evaluating the entire expression. Hence, to help students with their assignments, this program permits users to enter the matrix problems in a text file, which would be scanned and evaluated. This program could solve order operations involving addition, multiplication, and transposition. Every time the program creates a new matrix or solves an expression, it adds the result to a matrix binary search tree. The program prints out the solution of the latest expression when it reaches the end of the file.
<br><br>
<b>Notes/Warnings:</b>
<ul>
    <li>Lines in the text file must not contain leading spaces</li>
    <li>Matrix name can only be an alphabetical letter</li>
    <li>Each matrix creation or expression in the text file must be written in the following format:</li>
    <ul>
        <li>Matrix Creation: [matrix name] = [number of rows] [number of columns] [elements listed in row-major format and enclosed with brackets]</li>
        <b>Example:</b> A = 3 3 [1 2 3; 4 5 6; 7 8 9;]
        <li>Matrix Expression: [matrix name] = [expression]</li>
        <b>Example:</b> C = A + B
    </ul>
    <li>Excluding all blank lines, the last line of the text file must not be a matrix creation</li>
</ul>


<h2>Get Started</h2>
<h3>Dependencies</h3>
<ul>
    <li>Windows or Linux operating system</li>
    <li><a href="https://www.scaler.com/topics/c/c-compiler-for-windows/">GNU Compiler Collection (GCC)</a></li>
    <li>Text Editor or IDEs</li>
    <h4>Recommend:</h4>
    <ul>
        <li>Notepad (Worst Case)</li>
        <li><a href="https://www.codeblocks.org/downloads/binaries/">Code::Blocks</a> (Please download the one with GCC compiler)</li>
        <li><a href="https://www.eclipse.org/downloads/">Eclipse</a></li>
        <li><a href="https://code.visualstudio.com/Download">Visual Studio Code</a></li>
    </ul>
</ul>

<h3>Installing</h3>
<strong>Reference: </strong><a href="https://docs.github.com/en/get-started/quickstart/fork-a-repo">https://docs.github.com/en/get-started/quickstart/fork-a-repo</a>
<ol>
    <li>Fork this repository by clicking the Fork button located on the top of the page</li>
    <li>Navigate to "Your Repositories" on GitHub and open the forked repo</li>
    <li>In the terminal, git clone the repo using the URL or SSH key</li>
</ol>

<h3>Executing Program</h3>
<h4>Run with makeFile: </h4>
In the terminal, type <code>make</code> and <code>make test</code> to run the test cases written in unit_tests.c. Individuals may use the existing code as an example and write their test cases.<br><br>

<b>Note:</b> To run your test cases, include your input file in tests.in and the expected output in tests.out.exp.

<h4>Run with GCC: </h4>
In the terminal, type <code>gcc [source file]</code>. By default, this will produce an executable file named a.out (Linux) or a.exe (Windows). If you want the executable file to have a custom name, type <code>gcc -o [name of the executable] [source file]</code>. After compiling the program, run the executable file by entering <code>./[executable file] [input file]</code>.

<h2>Author</h2>
<strong>Zhen Wei Liao</strong> - <a href="https://www.linkedin.com/in/zhenwei-liao-148baa273">@Zhen</a><br>
Builts the main program and parts of MatrixCalc.h/unit_test.c.

<h3>Contributor:</h3>
<strong>Professor Kevin McDonnell</strong> - <a href="https://www.cs.stonybrook.edu/people/faculty/KevinMcDonnell">@KevinMcDonnell</a><br>
Builts the majority of unit_tests.c, unit_tests.h, makeFile, and .gitignore.

<h2>License</h2>
This project is under the MIT License. <br>Please check the LICENSE.md file for more information.
