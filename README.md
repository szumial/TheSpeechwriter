# The Speechwriter

## Description
This application stores text in a table. It allows to add, edit and remove sentences. It allows to move the sentences to reorder them. It can identify duplicate sentences and has built-in duplicate navigation. Additionally it enables the user to mark a sentence as the first sentence, which indicates the start of a new paragraph. The program also has import and export capabilities and works with plain text files.

## How to build
Open the _TheSpeechwriterApp.pro_ file in Qt Creator and compile it. The following build settings have been tested and verified to be working:

- Qt version used: 6.5.2
- Build system: qmake
- Verified to work when compiling using MinGW x64
- Tested on Windows 11 x64

Since Qt Framework is a multi-platform development too, it should be possible to compile the application
with different compilers and run it on other operating systems.

## Try it out
After starting the application, you can either edit the contents of the presented table yourself, or you can import the _sample.txt_ file, to check how it works with a predefined text file containing sentences structured in paragraphs.