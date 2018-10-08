#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#define ROBOT_NUM 10   // The id number (see below)
robot_link rlink;      // datatype for the robot link

void TestIO(){
    rlink.command(WRITE_PORT_3, 255);
    stopwatch watch;
    watch.start();
    while(true){
			int v=rlink.request (READ_PORT_3);
			cout << "time:" << watch.read() << "\tValue="  <<v << endl;
			watch.stop();
			watch.start();
	}
}

int main ()
{
    int val;                              // data from microprocessor
    if (!rlink.initialise (ROBOT_NUM)) { // setup the link
        cout << "Cannot initialise link" << endl;
        rlink.print_errs("  ");
        return -1;
    }
    val = rlink.request (TEST_INSTRUCTION); // send test instruction
    if (val == TEST_INSTRUCTION_RESULT) {   // check result
        cout << "Test passed" << endl;
        TestIO();
        return 0;                            // all OK, finish
    }
    else if (val == REQUEST_ERROR) {
        cout << "Fatal errors on link:" << endl;
        rlink.print_errs();
    }
    else
        cout << "Test failed (bad value returned)" << endl;
    return -1;                          // error, finish
}
