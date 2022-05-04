# cse101-pt.s22/pa5

The following is a set of performance tests to run on your Shuffle program. It
takes three input values and compares your results to our correct model
outputs. We have made this available to you to check your work before making
your final submission.

## Installation

Run the following in your working directory (the directory you wrote your code
in) to get the test script and example input files:

```bash
curl https://raw.githubusercontent.com/agavgavi/cse101-pt.s22/master/pa5/pa5.sh > pa5.sh
chmod +x pa5.sh
```

## Usage

After installation, you can run the script with this line:

```bash
./pa5.sh
```

It will print out the difference between your output and the correct output,
using the `diff` command. Lack of any output between the set of "=========="
means that your program is running correctly.

Any lines prefixed with `-` are from your own output, and are incorrect. Any
lines prefixed with `+` are from the correct output, and are missing in your
output.

## Removal

The following command will remove all text files and shell scripts in your
directory. Since you should not have any files that end in `.txt` or `.sh`
anyway, this should serve to delete all the files we gave you.

```bash
rm -f *.txt *.sh
```
