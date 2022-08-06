# WorldSeed
My Final Year Project at Staffordshire University. World seed is a UE4 plugin designed to assist level designers with quick level blockouts and itterations, focused around top-down maps.

# How the project works
This project is a UE4 Level Design Plugin, the respository here is not only the plugin but also an example of how it can be used.

The core elements of this system are the Style-Graph, GeneratorCore and the Landmarks, which inform the Generator of how the designer wishes the shape the map.

## Core Classes

The classes listed bellow are the main features of the plugin.
It should be also noted that Landmarks and Style-Graphs are fully extendable and have been designed in a way that enables programmers to implement their own additions.


### [GeneratorCore](https://github.com/RinKail/WorldSeed/blob/master/Plugins/WorldSeed/Source/WorldSeed/Public/WT_GeneratorCore.h)
The generator core handles all the heavy lifting in regards to managing the grid's data. This reads in the provided dimensions, and then has the landmarks apply their unique characteristics. This data is then shunted off to the Chunks to render the grid.

### [Chunks](https://github.com/RinKail/WorldSeed/blob/master/Plugins/WorldSeed/Source/WorldSeed/Public/WT_WorldChunk.h)
The Chunks handle the rendering of the enviroment. This involves utilising instanced-static meshes and loading them in from a data-table.

### [Landmarks](https://github.com/RinKail/WorldSeed/blob/master/Plugins/WorldSeed/Source/WorldSeed/Public/WT_LandmarkAnchor_Base.h)
Landmarks as previously mentioned are the main tool for designers to manipulate the enviroment. Based on the type being used and the parameters provided, a Landmark will apply it's characteristics to the grid, whether it be cutting away or adding to the enviroment.

### [Style-Graph](https://github.com/RinKail/WorldSeed/blob/master/Plugins/WorldSeed/Source/WorldSeed/Public/MyWT_GenerationStyle_Dungeon.h)
The style graph is one of the more unique elements of the plugin, as it allows designers to create their own generation system utilising a UObject with a number of useful utility functions created that will automatically generate landmarks in various sizes and positions.








