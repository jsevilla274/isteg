# Simple Image Steganography using LSB encoding

## To Compile

~~~~
mkdir build
cd build
cmake ..
cmake --build .
~~~~

## Usage

~~~~
*** Image Steganography by LSB substitution ***

Usage:  
./isteg [-e] <text file to encode> <source bitmap> <destination bitmap>
./isteg [-d] <encoded bitmap> <decoded file>

-e : Encode text in image
-d : Decode text from image
~~~~
