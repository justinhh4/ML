// Identifier: SDF

// EECS 281: Project 0
// Learn about:
//   1) Command-line processing with getopt_long()
//   2) Using enum to add readability to a limited option set
//   2) The vector member functions .resize() and .reserve()
//   3) The proper way to read data until end-of-file

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>
using namespace std;

// These modes represent the different ways to process the input data.
// There are three modes and the state where no mode has been selected.
enum class Mode {
    kNone = 0,
    kResize,
    kReserve,
    kNoSize,
};  // Mode{}


// This struct is used to pass options read from the command line, to
// the functions that actually do the work. Project 0 is simple enough to
// not need this, but it is a technique that could be helpful in doing more
// complicated command-line processing in future projects.
struct Options {
    Mode mode = Mode::kNone;
};  // Options{}


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *command) {
    cout << "Usage: " << command << " [-m resize|reserve|nosize] | -h\n";
    cout << "This program is to help you learn command-line processing,\n";
    cout << "reading data into a vector, the difference between resize and\n";
    cout << "reserve and how to properly read until end-of-file." << endl;
}  // printHelp()


// Finish this function, look for individual TODO comments internally.
// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getOptions(int argc, char **argv, Options &options) {
    // These are used with getopt_long()
    opterr = static_cast<int>(false);  // Let us handle all error output for command line options
    int choice;
    int index = 0;

    // NOLINTBEGIN: getopt is old C code predating C++ style, this usage is from `man getopt`
    option longOptions[] = {
  // Insert two lines for the "mode" ('m') and the "help" ('h') options.
        {"mode", required_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
         {nullptr, 0, nullptr, '\0'}
    };  // longOptions[]
    // NOLINTEND

    //Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "m:h", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'm': {  // Need a block here to declare a variable inside a case
            string arg { optarg };
            if (arg != "resize" && arg != "reserve" && arg != "nosize") {
                // The first line of error output has to be a 'fixed' message
                // for the autograder to show it to you.
                cerr << "Error: invalid mode" << endl;
                // The second line can provide more information, but you
                // won't see it on the AG.
                cerr << "  I don't recognize: " << arg << endl;
                exit(1);
            }  // if ..arg valid

            if (arg == "reserve") {
                options.mode = Mode::kReserve;
            } else if (arg[0] == 'r') {
                options.mode = Mode::kResize;
            } else {
                options.mode = Mode::kNoSize;
            }  // if ..arg
            break;
        }  // case 'm'

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);

        }  // switch ..choice
    }  // while

    if (options.mode == Mode::kNone) {
        cerr << "Error: no mode specified" << endl;
        exit(1);
    }  // if ..mode
}  // getOptions()


//  Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.resize() to set the current size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using a subscript
void readWithResize(vector<double> &data) {
    size_t size;
    cin >> size; 
    data.resize(size); 
    for (size_t i = 0; i < size; i++){
        cin >> data[i];  
    } // for i

}  // readWithResize()


// TODO: Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.reserve() to set the maximum size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using data.push_back()
void readWithReserve(vector<double> &data) {
    size_t size; 
    cin >> size; 
    data.reserve(size); 
    for (size_t i = 0; i < size; i++){
        double a;
        cin >> a; 
        data.push_back(a); 
    }
}  // readWithReserve()


// TODO: Write this function.  It should do the following:
//   Use a while loop to read in the values, storing them
//   into the vector using data.push_back()
void readWithNoSize(vector<double> &data) {
    double temp; 
    while (cin >> temp){
        data.push_back(temp); 
    }
}  // readWithNoSize()


// This function is already done.
double getAverage(const vector<double> &data) {
    double average;
    double sum = 0;

    for (auto datum : data) {
        sum += datum;
    }  // for ..datum

    average = sum / static_cast<double>(data.size());
    return average;
}  // getAverage()


// This function is already done.  Note that we have to sort the vector
// before we can find the median.
double getMedian(vector<double> &data) {
    // Sort the data
    sort(data.begin(), data.end());

    // Figure out if vector size is even or odd
    if (data.size() % 2 == 1) {
        return data[data.size() / 2];
    }  // if ..n is odd

    // Even size, average 2 middle values
    auto mid = data.size() / 2;
    const double v1 = data[mid];
    const double v2 = data[mid - 1];
    return (v1 + v2) / 2;
}  // getMedian()


// This function is already done.
int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    // Set two digits of precision on output
    cout << fixed << showpoint << setprecision(2);

    // Get the mode from the command line and read in the data
    Options options;
    getOptions(argc, argv, options);
    vector<double> data;
    if (options.mode == Mode::kResize) {
        readWithResize(data);
    } else if (options.mode == Mode::kReserve) {
        readWithReserve(data);
    } else if (options.mode == Mode::kNoSize) {
        readWithNoSize(data);
    }  // if ..options.mode

    // Print out the average and median of the data
    if (data.empty()) {
        cout << "No data => no statistics!" << '\n';
    } else {
        cout << "Average: " << getAverage(data) << '\n';
        cout << " Median: " << getMedian(data) << '\n';
    }  // if ..data

    return 0;
}  // main()
