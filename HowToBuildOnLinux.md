## Environment ##
You need build tools and gtk.

TODO: Fill in the packages that must be installed in order to build here (gtk-2.0-dev, etc.)

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

That's it. You now should have a simple test and a sample app(s) built in bin-dbg/linux-i386/

For now, you need to run this application from the hello\_world directory, so that it can find the .xml and .png files.

```
cd ~/glint-ui/glint/sample_apps/hello_world
../../bin-dbg/linux-i386/hello_world
```


We use GNU make to build.

To build non-debug bits, use this:
```
make MODE=opt
```
The result is going to be in bin-opt/linux-i386

For now, only Intel code is built.