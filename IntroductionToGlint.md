# Introduction #

Glint is a UI framework developed primarily for use in C++ applications. To illustrate the difference from other UI frameworks, here are main design principles of Glint:
  * Small and fast, compiled from source and statically linked with the app (whose size depends on the features being used)
  * No separate distribution, no DLLs, reg entries, dirs etc. A small static lib.
  * Does not use ATL, MFC, COM, extremely moderate use of C++ features like templates to make it readable and portable as much as possible.
  * It is not all things for all the people - but it is so simple that all people can add all things they want. Basically, it can be thought as a good place to start and then customize.
  * Most of the code is in a lib that does not make any outward calls - only via platform-encapsulating interface. So to port the whole thing to another platform it's enough to re-implement this interface and recompile. No hidden dependencies whatsoever - everything from memory allocation to taking input to drawing into screen locations is encapsulated into that interface.

# Usage #

To create Glint UI, one performs following tasks:
  1. Create XML 'pages' describing overall look of the UI. That includes also preparation of some images.
  1. Write code (C++) which loads the XML above from resource or file.
  1. Write some more code to find the interactive elements in that XML by id and add message handlers to them.
  1. Iterate on XML (looks) and code (behaviors) more or less independently until satisfied.

## Xml Structure ##

You can use simple "see it as you type" application to copy-paste examples from this document or experiment yourself. Just run sample application **glint\_pad** (glint\_pad.exe on Windows) from build directory. No installs necessary. It parses the XML you edit on the fly and renders what would be rendered on screen.

**Note**: glint\_pad is currently only built for Windows, but that's temporarily.

XML for Glint is treated in a very transparent way - when parser sees an open tag, it finds a C++ type registered in Glint TypeSystem (see type\_system.{h,cc}) with similar name and creates an instance of this type. Then it reads all attributes and their values and looks into TypeSystem again to find if the object specified by the opening tag actually has those properties. If there is a match, the callback registered for this type/property combination is called to set the value of the property. Simple. For nested objects, they are created the same way and then set as "default property" on a parent object.

The glint namespace is http://www.google.com/glint, so the simplest possible XML looks like this:
```
<Node xmlns="http://www.google.com/glint"/>
```

'Node' is the simplest type of the visual element in Glint - it may have background and it may contain other nodes, but that's all. To see your node on the screen, you should give it a background color (default is transparent) and some size (by default, there is nothing to define the size of the node so it's 0,0):
```
    <Node xmlns="http://www.google.com/glint"

          background="#FFFF00"

          min_width="200"

          min_height="200"

    /> 
```

If you type this into your glint\_pad, now you have yellow rectangle on the screen!

Note the '**min\_width**' and not '**width**' - since in Glint, we use min\_width and max\_width that are essentially constraints rather then exact amount. The size/position of things in Glint is computed automatically, so the localization, data fetched in runtime or such actions as user resizing a window do not ruin the carefully designed layout of dialogs and panels - the UI in Glint automatically adjusts according to 'hints' given by designer.


Here is an example of nested elements. Note how min\_width, min\_height, horizontal\_alignment and vertical\_alignment cause the child nodes to be positioned inside of their container:
```
<Node xmlns="http://www.google.com/glint" background="#FFFF00" min_width="200" min_height="200">

 <Node background="#CCCCCC" min_width="50" min_height="50"/>

 <Node background="#FF0000" min_width="50" min_height="50" vertical_alignment="top" horizontal_alignment="center"/>

 <Node background="#00FF00" min_width="50" min_height="50" vertical_alignment="center" horizontal_alignment="left"/>

 <Node background="#0000FF" min_width="50" min_height="50" vertical_alignment="bottom" horizontal_alignment="right"/>

</Node>
```

Note that the grey Node takes the whole space inside the parent - this is because default value for the horizontal and vertical\_alignment is "fill" which asks the child to take as much space as it can inside it's parent container.


Now, it's time to play with some text:
```
<Node xmlns="http://www.google.com/glint" background="#FFFF00" min_width="200" min_height="200">

   <Column>

    <SimpleText text="Hello, World!" font_size="24" font_family="Arial" margin="10 10 10 0"/>

    <SimpleText text="Hello, World!" font_size="24" font_family="Times" margin="10 10 10 0"/>

    <SimpleText text="Hello, World!" font_size="24" font_family="Georgia" margin="10 10 10 10"/>

  </Column>

</Node>
```


See those 3 lines of text? Note we put them into Column layout container - that makes them to be stacked on top of each other. There is also Row.

## Objects in Glint (for now) and their properties ##

**Node**
|Property|Format|Example|Description|
|:-------|:-----|:------|:----------|
|background|#AARRGGBB in hex form or #RRGGBB|background="#FFFF00"|sets background color, may be semi-transparent|
|min\_width|int   |min\_width="200"|sets the minimum width|
|min\_height|int   |min\_height="200"|sets the minimum height|
|max\_width|int   |max\_width="200"|element's layout is notified that this is the limit, if it still tries to grow more then that, it gets clipped|
|max\_height|int   |max\_height="200"|element's layout is notified that this is the limit, if it still tries to grow more then that, it gets clipped|
|margin  |"left top right bottom"|margin="10 10 10 0"|margin is a transparent "space" on 4 sides of the object, often used for spacing purposes|
|horizontal\_alignment|"fill" (default), "left", "center", "right"|horizontal\_alignment="left"|Specifies how to locate the child within its parent|
|vertical\_alignment|"fill" (default), "top", "center", "bottom"|vertical\_alignment="top"|Specifies how to locate the child within its parent|
|alpha   |int, from 0 (transparent) to 255 (opaque)|alpha="150"|sets transparency of the node|
|transform|translate(float\_dx float\_dy) scale(float\_scale\_x float\_scale\_y) rotate(float\_radians) matrix(m00 m01 m10 m11 m02 m12)|transform="rotate(0.3)"|sets transform applied to the element Note that transform is applied after the layout and does not affect layout results|
|id      |"string"|id="close\_button"|Sets ID that is used by code to bind to the node|

**SimpleText** has all the properties of **Node**, plus:
|text|"string"|text="Hello, World!"|sets the text to render|
|:---|:-------|:-------------------|:----------------------|
|font\_family|"string"|font\_family="Tahoma"|sets font to use       |
|font\_size|"number"|font\_size="18"     |sets font size, in points|
|foreground|"color\_ref"|foreground="#FF0000"|sets text color        |
|bold|"bool"  |bold="true"         |makes it bold          |
|italic|"bool"  |italic="true"       |makes it italic        |

**ImageNode** has all the properties of **Node**, plus:
|source|"file path"|source="c:\images\foo.png"|sets the image to display. Note that only local file names are supported. In case XML Parser was given a "base dir" (see **XmlParser::Parse(...)** method), relative filenames work too|
|:-----|:----------|:-------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

**NineGrid** has all the properties of **Node**, plus:
|source|"file path"|source="c:\images\foo.png"|sets the image to use as a 9-grid. Note that only local file names are supported. In case XML Parser was given a "base dir" (see **XmlParser::Parse(...)** method), relative filenames work too. The 9-grid works like this: it takes center N-pixel column and M-pixel row from the 9-grid image and stretches the image to a given size so that edges and center are filled as needed. Corners simply go to their respective destination corners unmodified, and sides and center are scaled using bilinear interpolation to correctly preserve gradients and non-trivial images. The transparency is, as everywhere else, processed correctly (use png images for transparency)|
|:-----|:----------|:-------------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|center\_width|int        |center\_width="50"        |sets the width of the center 'column' of a nine-grid partition. This 'column' is centerd in the nine-grid image                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
|center\_height|int        |center\_height="50"       |sets the height of the center 'row' of a nine-grid partition. This 'row' is centerd in the nine-grid image                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|shadow|"bool", default is 'false'|shadow="true"             |enables shadow around nine-grid image. Since nine-grid typically forms the outline of many UI controls like Buttons, using shadow on them makes then more '3D'                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |

**Row** has all the properties of **Node**, plus:
|direction|"left\_to\_right", "right\_to\_left", top\_to\_bottom" or "bottom\_to\_top"|direction="top\_to\_bottom"|Direction in which the row grows. Child nodes of the Row are arranged in specified direction. Basically, "top\_to\_bottom" makes Row to be a column...|
|:--------|:--------------------------------------------------------------------------|:--------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------|
|distribution|"string"                                                                   |distribution="153,`*`,2`*`"|Specifies the way available space is distributed among children of the Row. Numbers mean fixed pixel size, "natural" means "measure the child and give it its natural size", "`*`" means weighted portions of the remaining space. |

**Button** has all the properties of **NineGrid**, plus:
|enabled|"bool"|enabled="false"|Enables or disables the button . By default, the button is enabled. Disabled button can not be clicked|
|:------|:-----|:--------------|:-----------------------------------------------------------------------------------------------------|
|source |"file path"|source="button\_strip.png"|Specifies an "image strip" - the image that has 4 horizontally adjacent equal-size images for all the states of the Button - Normal, MouseOver, Pressed and Disabled, in this order|
|**Note**: the text or images or whatever you want to be inside the Button should be simply nested into it, since Button, as any other node in Glint, is also a container.|

**Column** is a very simple variation of Node - it arranges all its children vertically in a 'column'. Equivalent to:
```
<Row direction="top_to_bottom">...</Row>
```