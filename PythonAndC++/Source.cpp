#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
	PyImport_ImportModule("SomeNameHere") is supposed to be the name of the .py file you create
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("SamplePythonCode");//anytime you see this, add the .py file name
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"SamplePythonCode");//anytime you see this, add the .py file name
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
void callIntFunc(string proc, int param) //fixme
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"SamplePythonCode"); //anytime you see this, add the .py file name
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	//commented out following line, did not want return vaule.
	//if you want return value, uncomment following line and add return value to
	//function definition
	//return _PyLong_AsInt(presult);
}

//print user options function
void PrintMenu() {
	cout << string(100, '\n'); //clear screen refuses to work.
	cout << setw(28) << setfill('*') << "Program Menu" << setw(16) << "*" << endl;
	cout << "  1. Display a Multiplication Table" << endl;
	cout << "  2. Double a Value" << endl;
	cout << "  9. Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3." << endl;
	cout << setw(44) << "*" << endl << endl;
}


void main()
{

	int choice = 0;
	while (choice != 3) { //while loop to get more options next run through

		int userVal = 0; //always reinitialize userVal, just in case, inside loop

		PrintMenu(); //print user options each time loop is rerun

		try {
			if (cin >> choice) {}
			else {
				cin.clear();
				cin.ignore();
				choice = 0;
				throw 504; //catch block that shows the options necessary
			}
			switch (choice) {
			case 1: //display multiplication table
				cout << endl << endl << endl << "Enter your number:" << endl << endl;
				if (cin >> userVal) {}
				else {
					throw 505; //catch block shows value needs to be an int
				}
				cout << "Your number multiplication tables:" << endl;
				callIntFunc("MultiplicationTable", userVal); //call python code
				cout << endl << endl << endl << endl; //give some visual space, formatting
				system("pause"); //asks user to press any key to continue, pausing the output
				break; //break out of current loop, go again

			case 2://Double a Value
				cout << endl << endl << endl << "Enter your number:" << endl << endl;
				if (cin >> userVal) {}
				else {
					throw 505; //catch block shows value needs to be an int
				}
				cout << "Your number " << userVal << " doubled is ";
				callIntFunc("DoubleValue", userVal); //call python code
				cout << endl << endl << endl << endl;
				system("pause"); //asks user to press any key to continue, pausing the output
				break; //break out of current loop, go again

			case 3://Exit
				break;

			}
		}
		catch (int e) {
			if (e == 505) { //from input error in user investment parameter input stage
				string tempString;
				cout << endl << endl << "The input you entered was not an integer" << endl << endl;

				//It just positions the cin pointer at the end of the stdin stream and cin.clear() clears all error flags such as the EOF flags
				//https://stackoverflow.com/questions/257091/how-do-i-flush-the-cin-buffer
				cin.seekg(0, ios::end);
				cin.clear();
				system("pause"); //asks user to press any key to continue, pausing the output
			}
			else if (e == 504) { //from input error in user menu
				string tempString;
				cout << "Please enter (1) or (2) or (3)" << endl;

				//It just positions the cin pointer at the end of the stdin stream and cin.clear() clears all error flags such as the EOF flags
				//https://stackoverflow.com/questions/257091/how-do-i-flush-the-cin-buffer
				//because if the user pressed 10 incorrect characters, then enter, it would go through each incorrect character until it flushed itself
				cin.seekg(0, ios::end);
				cin.clear();
				system("pause"); //asks user to press any key to continue, pausing the output
				
			}
		}
		catch (...) {
			cout << endl << endl << "Error: Exception thrown" << endl << endl;
			cin.clear();
			cin.ignore(5);
		}
	}

}