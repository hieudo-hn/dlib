# GUI Information:
## DLIB Info
DLIB classes, headers, and functions relevant to basic widgets like dialog boxes and buttons along with their handler functions can be found in include/dlib/gui\_widgets. More advanced information can be found in the sections below. 

### Notable Files 
The ones relevant to most use cases are: widgets.h, widgets.cpp, and widgets\_abstract.h. In order to add a new GUI widget or handler based upon an old one, all three must be updated accordingly.

### Implementation Notes
Search the files listed in the __notable files__ section for the keyword "my" in order to find examples of adding a new message\_box, handler thread, and initializer.

### Execution Handling
Handling functions, like the ones that execute after pressing a button, are optionally assigned to the gui element by passing in &<yourfunction> to eligible constructors. In order for this function to run, a certain condition must be met.

For a basic message\_box the function triggers when the button is pressed. It dispatches a new thread to that function, which then deletes the window and executes the handler function passed to the constructor.

For mymessage\_box this execution is flipped. First, the handler function executes, then the dialog is deleted. Mymessage\_box also lacks a button, and by putting the call to the handler under the call to the initializer, rather than using the initializer to bind the handler to a button, it executes immediately upon completing the window initialization process.

### Important Notes on Thread Handling
The thread dispatched to execute the bound handler function func() should be the only thread involved in any process which may affect global execution. For example:

guielement("title", "message", &handlerfunction)
system("somesystemcall")

will result in guielement likely being improperly initialized until the system call completes, since the system call pauses all other execution. 

Instead, you can do the following:
void handlerfunction(){
//some function body here
.
.
.
system("somesystemcall")
}

guielement("title", "message", &handlerfunction)

to execute them sequencially. mymessage\_box's autoexecution of the handler was written to address such functionality.
