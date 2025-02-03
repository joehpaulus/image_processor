10 Image Processing Functions Described Below: 

### **PROCESS 1**

*   Description:

*   Adds vignette effect to image (dark corners)

*   Recommended function signature:

        vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image)

*   Sample output:

![](doc_images/process1.jpg)

### **PROCESS 2**

*   Description:

*   Adds Clarendon effect to image (darks darker and lights lighter) by a scaling factor

*   Recommended function signature:

        vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)

*   Sample output (with scaling_factor=0.3):

![](doc_images/process2.jpg)

###   **PROCESS 3**

*   Description:

*   Grayscale image

*   Recommended function signature:

        vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)

*   Sample output:

![](doc_images/process3.jpg)

###   **PROCESS 4**

*   Description:

*   Rotates image by 90 degrees clockwise (not counter-clockwise)

*   Recommended function signature:

        vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)

*   Sample output:

![](doc_images/process4.jpg)

###   **PROCESS 5**

*   Description:

*   Rotates image by a specified number of multiples of 90 degrees clockwise

*   Recommended function signature:

        vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)

*   Sample output (with number=2):

![](doc_images/process5.jpg)

###   **PROCESS 6**

*   Description:

*   Enlarges the image in the x and y direction

*   Recommended function signature:

        vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)

*   Sample output (with x_scale=2 and y_scale=3):

![](doc_images/process6.jpg)

###  **PROCESS 7**

*   Description:

*   Convert image to high contrast (black and white only)

*   Recommended function signature:

        vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image) 

*   Sample output:

![](doc_images/process7.jpg)

###   **PROCESS 8**

*   Description:

*   Lightens image by a scaling factor

*   Recommended function signature:

        vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor) 

*   Sample output (with scaling_factor=0.5):

![](doc_images/process8.jpg)

###   **PROCESS 9**

*   Description:

*   Darkens image by a scaling factor

*   Recommended function signature:

        vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)  

*   Sample output (with scaling_factor=0.5):

![](doc_images/process9.jpg)

###   **PROCESS 10**

*   Description:

*   Converts image to only black, white, red, blue, and green

*   Recommended function signature:

        vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)

*   Sample output:

![](doc_images/process10.jpg)

* * *

### USER INTERFACE 

*   User can enter an input BMP filename

    *   This is the image file that the operations will be performed on

*   Displays a menu of available selections to the user

    *   One selection to change the input image if desired
    *   One for each of the image processing functions

*   User can enter their desired operation / menu selection
*   User can enter the desired output BMP filename

    *   This is the file where the modified image will be saved to
    *   Remember to never specify the original input BMP filename or else it will be overwritten!

*   User can enter the necessary parameters for the selected operation
    *   For example: scaling factor, number of rotations, etc.
    *   This applies to processes 2, 5, 6, 8, 9

*   Displays confirmation that the operation was successful
    *   Or an error message if the operation failed

*   User is brought back to the main menu after each operation so they can continue performing operations until they chose to exit out of the program

    *   It is very important that your program continues to accept user input until they explicitly signal they want to exit
    *   _Hint: You should use a while loop for your menu system_

*   User can gracefully exit out of the program

Below is an example input/output session showing how your user interface might look and operate. The text within the back quotes (`) represents the user input and the remainder of the text is the output from your program. Please note that your user interface does not have to match exactly with the one shown below. That is, your wording, order, look and feel, and layout can differ as long as you still meet the requirements outlined above. You may even want to improve upon what's shown below by making your user interface look nicer, giving better feedback to the user, performing user input validation, etc. However, you are not required to do so.  

<pre>
$ `./main`

Enter input BMP filename: `sample.bmp`

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `1`

Vignette selected
Enter output BMP filename: `process1.bmp`
Successfully applied vignette!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `2`

Clarendon selected
Enter output BMP filename: `process2.bmp`
Enter scaling factor: `0.3`
Successfully applied clarendon!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `3`

Grayscale selected
Enter output BMP filename: `process3.bmp`
Successfully applied grayscale!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `4`

Rotate 90 degrees selected
Enter output BMP filename: `process4.bmp`
Successfully applied 90 degree rotation!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `5`

Rotate multiple 90 degrees selected
Enter output BMP filename: `process5.bmp`
Enter number of 90 degree rotations: `2`
Successfully applied multiple 90 degree rotations!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `6`

Enlarge selected
Enter output BMP filename: `process6.bmp`
Enter X scale: `2`
Enter Y scale: `3`
Successfully enlarged!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `7`

High contrast selected
Enter output BMP filename: `process7.bmp`
Successfully applied high contrast!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `8`

Lighten selected
Enter output BMP filename: `process8.bmp`
Enter scaling factor: `0.5`
Successfully lightened!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `9`

Darken selected
Enter output BMP filename: `process9.bmp`
Enter scaling factor: `0.5`
Successfully darkened!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `10`

Black, white, red, green, blue selected
Enter output BMP filename: `process10.bmp`
Successfully applied black, white, red, green, blue filter!

IMAGE PROCESSING MENU
0) Change image (current: sample.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `0`

Change image selected
Enter new input BMP filename: : `sample2.bmp`
Successfully changed input image!

IMAGE PROCESSING MENU
0) Change image (current: sample2.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `1`

Vignette selected
Enter output BMP filename: `process1_2.bmp`
Successfully applied vignette!

IMAGE PROCESSING MENU
0) Change image (current: sample2.bmp)
1) Vignette
2) Clarendon
3) Grayscale
4) Rotate 90 degrees
5) Rotate multiple 90 degrees
6) Enlarge
7) High contrast
8) Lighten
9) Darken
10) Black, white, red, green, blue

Enter menu selection (Q to quit): `Q`

Thank you for using my program!
Quitting...

$
</pre>
* * *
