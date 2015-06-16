## Environment ##

Install the following to create the build environment:

  * Subversion (svn client) - [from here](http://subversion.tigris.org/downloads/subversion-1.4.4.zip)
  * Visual Studio 2005 (Perhaps other versions will work too)

## Getting the code ##
If you don't have the code yet, here are the steps, from cmd.exe window (assuming c:\projects\glint is the place where you want to keep the code):
```
cd /d C:\projects
mkdir glint-ui
svn checkout http://glint-ui.googlecode.com/svn/trunk glint-ui
```

## Building ##

Now you can just build it (assuming the directory structure above):
```
cd /d C:\projects\glint-ui\glint
make
```
The result is going to be in bin-dbg/win-i386

To build non-debug bits, use this:
```
make MODE=opt
```
The result is going to be in bin-opt/win-i386