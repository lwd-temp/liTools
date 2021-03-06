liTools
=======
Tools for compression and decompression of Little Inferno .pak files, and routines for managing mod creation and merging

Created by Daxar (aka MOM4Evr) from the infernofans forum -- http://infernofans.com/user/13

RUN ALL THESE PROGRAMS OFF A BACKUP COPY OF YOUR GAME, NOT THE ORIGINAL VERSION! You have been warned. It's also a good idea to have a backup copy of your embed.pak, frontend.pak, and resource.pak just in case something goes terribly horribly wrong, since errors tend to multiply rather than vanish as you compress/decompress pakfiles.

Also please note that it's best to have the the input files (Such as the Little Inferno executable, or the pak files you want to extract) in the same directory as these tools when running them. It'll save you a lot of hassle if you're using the command line, and it'll alleviate potential working directory issues if you're clicking and dragging files into executables.


Usage
=====
strip
-----
First, you'll need the three resource files "resource.pak", "embed.pak", and "frontend.pak" extracted from your game's executable. You can do this with 7zip or ResourceHacker (the files you want are in the subfolder 20480, resources 1, 2, and 3 respectively) if you so choose. liTools provides a tool to help out with this, however. Simply run:

    strip.exe "Little Inferno.exe"

Or click and drag "Little Inferno.exe" into "strip.exe". This will pull all the .pak files out of your game's executable and strip the .pak files from your executable so it will read the .pak files from disk on the next run. Note that since this does modify your executable, BACK IT UP FIRST! See the "Utilities" section of this readme for programs to pull out the .pak files without modifying the game executable.

liDecompress
------------
Once you have all three .pak files extracted, you can now decompress them into a useable form with liDecompress. You can decompress more than one file at a time if you wish. Simply drag the three .pak files into the executable, or run it like:

    liDecompress.exe resource.pak embed.pak frontend.pak

The present version of the program attempts to reconstruct some of the resources from the extracted binary data, though something may go terribly horribly wrong. In this case, it should just spit out errors into the console and keep going. It should be fairly robust, but don't blame me if it crashes unexpectedly or does something bizarre.

Please note that can take a good amount of time to decompress these files. resource.pak takes about 2 minutes for me. The console window should tell you the overall progress, and force-killing the program shouldn't have (m)any detrimental consequences, but be prepared to be patient. It does take a while. This program is fairly unoptimized, and it's a large file to process with a lot of compression/decompression steps for each resource.

After decompression, the files you're after are in data/ and vdata/. The filenames are correct, but not all data formats are known yet, so not everything is openable. (For version 0.3, only png and ogg files are in the correct format, and some .dat files are converted to xml)

Accepted commandline switches:
	--overwrite-progress	Writes over the previous progress indicator, to mimimize commandline output. Doesn't display filenames.
	--threads=n				Decompress with n threads. If n=0, will spawn as many threads as your computer has processor cores. Default is n=0.

liCompress
----------
To compress files again, type:

    liCompress.exe resource.pak embed.pak frontend.pak

Please note that these .pak files might not exist beforehand (they don't have to). If they do exist, you can click-drag them into the executable as usual. For each pakfile, liCompress looks for a text file in the form [pakfile].pak.filelist.txt to know what files go inside that particular .pak file. You can create your own .pak files this way. liDecompress will autogenerate these .filelist.txt files when decompressing, so to save yourself a lot of typing, decompress first before compressing so all the .filelist.txt files are there. For now, this is how it works; this probably will change in a later version to become more user-friendly.

Accepted commandline switches:
	--overwrite-progress	Writes over the previous progress indicator, to mimimize commandline output. Doesn't display filenames.
	--threads=n				Compress with n threads. If n=0, will spawn as many threads as your computer has processor cores. Default is n=0.
	--level=n				Compress with the zlib compression level n, which should be between 0 (no compression) and 9 (full compression). Default is n=-1.


Sound modification
==================
The way the game is set up, memory is allocated for sounds before the sounds are loaded. If you change the .ogg sounds in the game folders, the file vdata/sndmanifest.dat will also have to be updated to reflect these changes, otherwise the game may crash. Before compressing, run the program:

  recalcSoundManifest.exe 

No commandline arguments are needed. This will parse through all the .ogg files in the game folders and update the sndmanifest.dat.xml generated by liDecompress. In order for this to work, you'll have to have run liDecompress on embed.pak first, as embed.pak contains the sndmanifest.dat file that will be updated. Once you've run recalcSoundManifest, run liCompress as normal, but be sure to compress embed.pak as well, to save your changes (embed.pak contains the vdata/sndmanifest.dat file).
Also note that from the sounds I've tried changing, it seems that the game is finnicky about the .ogg files it accepts. The only way I've gotten any new files to work is by setting the "Quality" setting for the OGG export to 3 (In Audacity; I havent tried with other tools). Other values seem to cause the game to crash.

Note that you can edit the vdata/sndmanifest.dat.xml freely. Each take for a sound is one sound effect that plays on that particular event. So, for example, if you want to cause the Little Inferno Beta jingle sound to happen 50% of the time when the cardboard sword burns, simply find the Little Inferno Beta take in the xml (line 308 for me), and copy it beside the take for the cardboard sword jingle (Line 104). So, here's what it will look like before:

    <sound id="4292572444">
        <take filename="data/items/CardboardSword/endOfAdventure.flac.ogg" channels="2" samplespersec="44100" samplecountperchannel="286651" vorbisworkingsetsizebytes="106848" vorbismarkerssizebytes="288" vorbispacketssizebytes="97656"/>
    </sound>

And after:
	
    <sound id="4292572444">
        <take filename="data/items/CardboardSword/endOfAdventure.flac.ogg" channels="2" samplespersec="44100" samplecountperchannel="286651" vorbisworkingsetsizebytes="106848" vorbismarkerssizebytes="288" vorbispacketssizebytes="97656"/>
        <take filename="data/items/LittleInfernoBeta/BetaMusic.flac.ogg" channels="2" samplespersec="44100" samplecountperchannel="705600" vorbisworkingsetsizebytes="106848" vorbismarkerssizebytes="648" vorbispacketssizebytes="224898"/>
    </sound>

Just save this change and compress embed.pak and you're good to go.

Note that you don't have to worry about all the Vorbis data; you could also just have:

    <sound id="4292572444">
        <take filename="data/items/CardboardSword/endOfAdventure.flac.ogg" />
        <take filename="data/items/LittleInfernoBeta/BetaMusic.flac.ogg" />
    </sound>

And run recalcSoundManifest.exe (before recompressing), which will repopulate the rest of the XML.

Please note that all this DOES NOT mean that you can add your own sounds to the game. New sound resources are not supported yet. If you attempt to add a new .ogg file as a take somewhere, you'll end up with an undefined sound ID, which will likely cause a crash.

Mod Creation
============
Mods should be standard .pak files. To create your own mod, change the files you wish to change, and create a [modname].filelist.txt, which should be a list of all the filenames (relative to the Little Inferno executable) of all the files you have changed. For example, if you change the images data/items/Battery/Battery.png, data/animations/Intro/pchim1.png, data/animations/Intro/pchim2.png, and data/animations/Intro/pchim3.png, and you wish to name your mod "myMod", you'll have to create a file "myMod.filelist.txt" in the Little Inferno executable directory, and add the following lines to it:

    data/items/Battery/Battery.png
    data/animations/Intro/pchim1.png
    data/animations/Intro/pchim2.png
    data/animations/Intro/pchim3.png

Then, simply run 

    liCompress myMod.pak

And it'll spit out the myMod.pak file. 

PLEASE NOTE that the files in the .filelist.txt file must have the same extension as the filenames from residmap.dat. For example, if you modify the file "data/music/Credits.flac.ogg", then your corresponding line in the .filelist.txt should be "data/music/Credits.flac". Otherwise, the program won't work correctly. This sucks and will likely be changed in a later version. In any case, liCompress works via file extension anyway, so don't go changing any file extensions.

Mod Installation
================
To install mods, run

    modManage [pakfile1] [pakfile2] ... [pakfileN]

Where [pakfileX] is the mod you wish to install (You can also click-and-drag files into the executable as normal). modManage will pull resource headers out of every .pak file to figure out what files should go in what .pak file, and will only modify the original .pak files as needed. Changes made by pafiles later in the commandline will overwrite earlier changes; for example, if you run "modManage battery.pak battery2.pak", and both of these .pak files change data/items/Battery/Battery.png, the battery2.pak's version of the image will end up in the final game.
modManage will spit out a file in the working directory titled "mergeresults.txt", which will have details as to how the merging went. In case two mods conflict and overwrite the same file, it'll say so in this file. 


Changelog
=========
Version 0.1:
* Initial release
* Most .png and .png.normal images are correctly formatted

Version 0.2:
* Added pullpakfiles and WinResource programs for pulling resources directly from the game's executable
* Correct handling of greyscale images

Version 0.2.1:
* Made strip program for extracting then stripping the .pak files
* Made util/removeresc.exe and util/test.exe programs
* Refactoring, minor changes to some tools
	
Version 0.3:
* Added liCompress for compressing everything back into .pak files
* For now, all files are compressed by default
* Added util/repack.exe program for repacking .pak files into the game's exe

Version 0.3.1:
* Multithreaded decompression/compression for speed
* OGG streams are now left uncompressed

Version 0.3.2:
* modManage program for merging in mods
	
Version 0.3.3:
* vdata/wordPackDict.dat is parsed and written to vdata/wordPackDict.dat.xml
* wordPackDict.dat.xml is compressed back into .dat form

Version 0.3.4:
* vdata/sndmanifest.dat is parsed and written to vdata/sndmanifest.dat.xml
* sndmanifest.dat.xml is compressed back into .dat form
* added recalcSoundManifest.exe program to update sndmanifest.dat.xml if .ogg files are changed
* removed residmap.dat; its contents are now compiled into the compress/decompress executables

Version 0.3.5:
* vdata/itemmanifest.dat is partially parsed to vdata/itemmanifest.dat.xml
* itemmanifest.dat.xml is NOT parsed back; it's just there to look pretty for now
* Optimized liCompress and liDecompress; they should run ~40 seconds faster now, just with higher RAM usage
* Commandline arguments for liDecompress and liCompress; see readme for details
* Optimized modManage program to merge mods within seconds
* residmap.dat now parsed to/from XML
* Images now properly converted to/from premultiplied alpha
* Initial Unicode support

Version 0.3.6: (9/10/13)
* Now (only) supports Little Inferno 1.2. Be sure to upgrade to the latest version before using!
* vdata/itemmanifest.dat is now parsed to/from XML correctly. Hooray!
* vdata/loctexmanifest.dat now parsed to/from XML
* vdata/combodb.dat now parsed to/from XML
* Matrix transformations thanks to MyGod
* Some Unicode tweaks and such
* Added util/hash.exe for easily getting hashes of filenames or other such text strings

Version 0.3.7: (9/13/13)
* vdata/catalogdb.dat is now parsed to/from XML
* vdata/letterdb.dat is now parsed to/from XML


Building
========
Build with:

    make

These utilities depend on the following third-party libraries, which you'll need to link in:
libogg - http://xiph.org/downloads/
libpng - http://www.libpng.org/pub/png/libpng.html
tinyxml2 - http://www.grinninglizard.com/tinyxml2/index.html
ttvfs - https://github.com/fgenesis/ttvfs
libvorbis - http://xiph.org/downloads/
zlib - http://www.zlib.net/

Apologies for hastily-thrown-together code that isn't commented much at all. It should be fairly straightforward and readable, but no guarantees.


Utilities ("util" subdirectory)
===============================
pullpakfiles.exe
----------------
If you'd rather just read the .pak files from your executable without stripping them out and modifying your game's executable, run:

    util/pullpakfiles.exe "Little Inferno.exe"

Or copy "pullpakfiles.exe" from the "util" subdirectory into the same folder as "Little Inferno.exe" and click and drag "Little Inferno.exe" into it. Please note that this leaves the .pak files inside your executable, so the executable will still read the internal pakfiles when run.


removeresc.exe
----------------
This program strips the .pak files out of your game's executable without saving them anywhere. DO NOT USE this program unless you have already run pullpakfiles to get the .pak files out, or unless you have the three .pak files in the directory already, since it just deletes the resources from the game without saving them anywhere. If you wish to run this program, use it in the same way as pullpakfiles:

    util/removeresc.exe "Little Inferno.exe"

Or copy "removeresc.exe" from the "util" subdirectory into the same folder as "Little Inferno.exe" and click and drag "Little Inferno.exe" into it.


WinResource.exe
----------------
This program reads all the resources from inside an executable and spits out some data about them. In moments of dire need, it probably won't help you at all. So ignore it. Or run it with:

    util/WinResource.exe "Little Inferno.exe"

Or copy "WinResource.exe" from the "util" subdirectory into the same folder as "Little Inferno.exe" and click and drag "Little Inferno.exe" into it. It'll spit out a file "resinfo.txt" containing basic information on the resources inside the Little Inferno executable. It may be useful, however, if you're unsure if your executable has been stripped or not (as if the file size alone wouldn't tell you). If resources with "Type: 20480" are all 4 bytes, you'll know it's stripped. Otherwise, probably not. At the very least, it's a fun program to play with.


hash.exe
----------------
Usage:
	hash.exe [string1] [string2] ... [stringn]

Spits out the hashed unsigned integer values of the input strings. Useful for debugging resource values and such.

