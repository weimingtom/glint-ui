## Environment ##
You'd need Leopard (or Tiger) machine with XCode installed. We run build/test on a regular basis on Leopard/XCode 3.0 setup.

## Getting the code ##
If you don't have the code yet, here are the steps, from Terminal window:
```
cd ~/
mkdir glint-ui
svn checkout http://glint-ui.googlecode.com/svn/trunk glint-ui
```

## Building ##

Now you can just build it:
```
cd ~/glint-ui/glint
make
```

That's it. You now should have a simple test and a sample app(s) built in bin-dbg/osx-i386/
Note that to run the sample applications like hello\_world you need to double-click on its bundle in Finder. Just running its executable from command line won't work nicely because of the way OSX treats bundles.

We use GNU make to build.

To build non-debug bits, use this:
```
make MODE=opt
```
The result is going to be in bin-opt/osx-i386

For now, only Intel code is built.