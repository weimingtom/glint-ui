Glint is an open-source UI framework developed primarily for use in C++ applications. It is bulit around the concept of "UI Node" which can form a "UI tree". On that tree, Glint performs layout (computing required sizes and setting final bounds of nodes), drawing (recursive compositing with transform and clip support), input handling (converting input in events and traversing the tree to deliver the events), focus handling (for keyboard input and to provide visual cues) and animations support (for transitional animations).

To illustrate the difference from other UI frameworks, here are main design principles of Glint:

1. Small and fast, compiled from source and statically linked with the app. No separate distribution, no DLLs, reg entries, dirs etc. A small static lib.

2. Does not use ATL, MFC, COM, Boost and uses very little C++ features like templates to make it readable as much as possible and easy to compile into application's project.

3. It is not all things for all the people - but it is so simple that all people can add things they want. Basically, it can be thought as a good place to start and then customize.

4. No pre-packaged hi-level controls like listboxes or tabbed panels. Glint has Node, ImageNode, NineGrid, SimpleText, Button, Row, Column and some other types of nodes. Not even editable text control although this can change in the future.