/*
main.cpp
CSPB 1300 Image Processing Application

PLEASE FILL OUT THIS SECTION PRIOR TO SUBMISSION

- Your name:
    Joe Hoertig-Paulus

- All project requirements fully met? (YES or NO):
    YES

- If no, please explain what you could not get to work:
    N/A

- Did you do any optional enhancements? If so, please explain:
    Yes, I added additional descriptions in the user interface stating what: scaling factor, size increase, or 90 degree rotations the user has made the new image different from the input image. These enhancements were applied to processes: 2, 5, 6, 8, and 9.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION BELOW                                    //
//***************************************************************************************************//

// Pixel structure
struct Pixel
{
    // Red, green, blue color values
    int red;
    int green;
    int blue;
};

/**
 * Gets an integer from a binary stream.
 * Helper function for read_image()
 * @param stream the stream
 * @param offset the offset at which to read the integer
 * @param bytes  the number of bytes to read
 * @return the integer starting at the given offset
 */ 
int get_int(fstream& stream, int offset, int bytes)
{
    stream.seekg(offset);
    int result = 0;
    int base = 1;
    for (int i = 0; i < bytes; i++)
    {   
        result = result + stream.get() * base;
        base = base * 256;
    }
    return result;
}

/**
 * Reads the BMP image specified and returns the resulting image as a vector
 * @param filename BMP image filename
 * @return the image as a vector of vector of Pixels
 */
vector<vector<Pixel> > read_image(string filename)
{
    // Open the binary file
    fstream stream;
    stream.open(filename, ios::in | ios::binary);

    // Get the image properties
    int file_size = get_int(stream, 2, 4);
    int start = get_int(stream, 10, 4);
    int width = get_int(stream, 18, 4);
    int height = get_int(stream, 22, 4);
    int bits_per_pixel = get_int(stream, 28, 2);

    // Scan lines must occupy multiples of four bytes
    int scanline_size = width * (bits_per_pixel / 8);
    int padding = 0;
    if (scanline_size % 4 != 0)
    {
        padding = 4 - scanline_size % 4;
    }

    // Return empty vector if this is not a valid image
    if (file_size != start + (scanline_size + padding) * height)
    {
        return {};
    }

    // Create a vector the size of the input image
    vector<vector<Pixel>> image(height, vector<Pixel> (width));

    int pos = start;
    // For each row, starting from the last row to the first
    // Note: BMP files store pixels from bottom to top
    for (int i = height - 1; i >= 0; i--)
    {
        // For each column
        for (int j = 0; j < width; j++)
        {
            // Go to the pixel position
            stream.seekg(pos);

            // Save the pixel values to the image vector
            // Note: BMP files store pixels in blue, green, red order
            image[i][j].blue = stream.get();
            image[i][j].green = stream.get();
            image[i][j].red = stream.get();

            // We are ignoring the alpha channel if there is one

            // Advance the position to the next pixel
            pos = pos + (bits_per_pixel / 8);
        }

        // Skip the padding at the end of each row
        stream.seekg(padding, ios::cur);
        pos = pos + padding;
    }

    // Close the stream and return the image vector
    stream.close();
    return image;
}

/**
 * Sets a value to the char array starting at the offset using the size
 * specified by the bytes.
 * This is a helper function for write_image()
 * @param arr    Array to set values for
 * @param offset Starting index offset
 * @param bytes  Number of bytes to set
 * @param value  Value to set
 * @return nothing
 */
void set_bytes(unsigned char arr[], int offset, int bytes, int value)
{
    for (int i = 0; i < bytes; i++)
    {
        arr[offset+i] = (unsigned char)(value>>(i*8));
    }
}

/**
 * Write the input image to a BMP file name specified
 * @param filename The BMP file name to save the image to
 * @param image    The input image to save
 * @return True if successful and false otherwise
 */
bool write_image(string filename, const vector<vector<Pixel>>& image)
{
    // Get the image width and height in pixels
    int width_pixels = image[0].size();
    int height_pixels = image.size();

    // Calculate the width in bytes incorporating padding (4 byte alignment)
    int width_bytes = width_pixels * 3;
    int padding_bytes = 0;
    padding_bytes = (4 - width_bytes % 4) % 4;
    width_bytes = width_bytes + padding_bytes;

    // Pixel array size in bytes, including padding
    int array_bytes = width_bytes * height_pixels;

    // Open a file stream for writing to a binary file
    fstream stream;
    stream.open(filename, ios::out | ios::binary);

    // If there was a problem opening the file, return false
    if (!stream.is_open())
    {
        return false;
    }

    // Create the BMP and DIB Headers
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    unsigned char bmp_header[BMP_HEADER_SIZE] = {0};
    unsigned char dib_header[DIB_HEADER_SIZE] = {0};

    // BMP Header
    set_bytes(bmp_header,  0, 1, 'B');              // ID field
    set_bytes(bmp_header,  1, 1, 'M');              // ID field
    set_bytes(bmp_header,  2, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE+array_bytes); // Size of BMP file
    set_bytes(bmp_header,  6, 2, 0);                // Reserved
    set_bytes(bmp_header,  8, 2, 0);                // Reserved
    set_bytes(bmp_header, 10, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE); // Pixel array offset

    // DIB Header
    set_bytes(dib_header,  0, 4, DIB_HEADER_SIZE);  // DIB header size
    set_bytes(dib_header,  4, 4, width_pixels);     // Width of bitmap in pixels
    set_bytes(dib_header,  8, 4, height_pixels);    // Height of bitmap in pixels
    set_bytes(dib_header, 12, 2, 1);                // Number of color planes
    set_bytes(dib_header, 14, 2, 24);               // Number of bits per pixel
    set_bytes(dib_header, 16, 4, 0);                // Compression method (0=BI_RGB)
    set_bytes(dib_header, 20, 4, array_bytes);      // Size of raw bitmap data (including padding)                     
    set_bytes(dib_header, 24, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 28, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 32, 4, 0);                // Number of colors in palette
    set_bytes(dib_header, 36, 4, 0);                // Number of important colors

    // Write the BMP and DIB Headers to the file
    stream.write((char*)bmp_header, sizeof(bmp_header));
    stream.write((char*)dib_header, sizeof(dib_header));

    // Initialize pixel and padding
    unsigned char pixel[3] = {0};
    unsigned char padding[3] = {0};

    // Pixel Array (Left to right, bottom to top, with padding)
    for (int h = height_pixels - 1; h >= 0; h--)
    {
        for (int w = 0; w < width_pixels; w++)
        {
            // Write the pixel (Blue, Green, Red)
            pixel[0] = image[h][w].blue;
            pixel[1] = image[h][w].green;
            pixel[2] = image[h][w].red;
            stream.write((char*)pixel, 3);
        }
        // Write the padding bytes
        stream.write((char *)padding, padding_bytes);
    }

    // Close the stream and return true
    stream.close();
    return true;
}

//***************************************************************************************************//
//                                DO NOT MODIFY THE SECTION ABOVE                                    //
//***************************************************************************************************//


//
// YOUR FUNCTION DEFINITIONS HERE
//

vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
            
            double dist_cols = (col - (num_columns / 2)) * (col - (num_columns / 2));
            double dist_rows = (row - (num_rows / 2)) * (row - (num_rows / 2));
                                                         
            double distance = sqrt(dist_cols + dist_rows);
                                                                                                       
            double scaling_factor = (num_rows - distance) / num_rows;

            // Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            new_image[row][col].red = red_color * scaling_factor;
            new_image[row][col].green = green_color * scaling_factor;
            new_image[row][col].blue = blue_color * scaling_factor;
        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;
}

vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            double average_value = (red_color + green_color + blue_color) / 3;
            
            if (average_value >= 170)
            {
                new_image[row][col].red = (255 - (255 - red_color) * scaling_factor);
                new_image[row][col].green = (255 - (255 - green_color) * scaling_factor);
                new_image[row][col].blue = (255 - (255 - blue_color) * scaling_factor);
            }
            else if (average_value < 90)
            {
                new_image[row][col].red = red_color * scaling_factor;
                new_image[row][col].green = green_color * scaling_factor;
                new_image[row][col].blue = blue_color * scaling_factor;
            }
            else
            {
                new_image[row][col].red = red_color;
                new_image[row][col].green = green_color;
                new_image[row][col].blue = blue_color;
            }

        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;
}

vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            double gray_value = (red_color + green_color + blue_color) / 3;
            
            new_image[row][col].red = gray_value;
            new_image[row][col].green = gray_value;
            new_image[row][col].blue = gray_value;

        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;
}

vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    // Height and Width are switched 
    vector<vector<Pixel>> new_image(num_columns, vector<Pixel> (num_rows));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[col][row].red = image[num_rows-1-row][col].red;
            new_image[col][row].green = image[num_rows-1-row][col].green;
            new_image[col][row].blue = image[num_rows-1-row][col].blue;
            
            //newimg.setPixel( (row-1) - num_rows, num_columns ,p) <--- ^ this is my understanding of the translation

        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;
}

vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    int choice = number % 4;
    
    // Define a new 2D vector the same size as the input 2D vector
    // Height and Width are switched depending on user choice (number of 90 degree rotations)
    
if (0 == choice)
{
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[row][col].red = image[row][col].red;
            new_image[row][col].green = image[row][col].green;
            new_image[row][col].blue = image[row][col].blue;
            
        }
    }
    return new_image;    

}

if (1 == choice)
{
    vector<vector<Pixel>> new_image(num_columns, vector<Pixel> (num_rows));

    
    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[col][row].red = image[num_rows-1-row][col].red;
            new_image[col][row].green = image[num_rows-1-row][col].green;
            new_image[col][row].blue = image[num_rows-1-row][col].blue;
            
        }
    }
    return new_image;    

}

if (2 == choice)
{
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[row][col].red = image[num_rows-1-row][num_columns - 1 - col].red;
            new_image[row][col].green = image[num_rows-1-row][num_columns - 1 - col].green;
            new_image[row][col].blue = image[num_rows-1-row][num_columns - 1 - col].blue;
            
        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;    

}
if (3 == choice)
{
    vector<vector<Pixel>> new_image(num_columns, vector<Pixel> (num_rows));
    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[col][row].red = image[row][num_columns - 1 - col].red;
            new_image[col][row].green = image[row][num_columns - 1 - col].green;
            new_image[col][row].blue = image[row][num_columns - 1 - col].blue;
            
        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;    
}
    return image;
}



vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector larger than the input 2D vector by user input (*x and *y)
    vector<vector<Pixel>> new_image(y_scale * num_rows, vector<Pixel> (x_scale * num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < y_scale * num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < x_scale * num_columns; col++)
        {
            
        // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[row][col].red = image[row / y_scale][col / x_scale].red;
            new_image[row][col].green = image[row / y_scale][col / x_scale].green;
            new_image[row][col].blue = image[row / y_scale][col / x_scale].blue;

        }
        // Return the new 2D vector after the nested for loop is complete//
        
    }
    return new_image;
}


vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            double gray_value = (red_color + green_color + blue_color) / 3;
            
            if (gray_value >= 255/2)
            {
                new_image[row][col].red = 255;
                new_image[row][col].green = 255;
                new_image[row][col].blue = 255;
            }
            else
            {
                new_image[row][col].red = 0;
                new_image[row][col].green = 0;
                new_image[row][col].blue = 0;
            }

        }
        // Return the new 2D vector after the nested for loop is complete
    }
    return new_image;
}

vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[row][col].red = (255 - (255 - red_color) * scaling_factor);
            new_image[row][col].green = (255 - (255 - green_color) * scaling_factor);
            new_image[row][col].blue = (255 - (255 - blue_color) * scaling_factor);   
        }
        // Return the new 2D vector after the nested for loop is complete   
    }
    return new_image;
}

vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            new_image[row][col].red = (red_color * scaling_factor);
            new_image[row][col].green = (green_color * scaling_factor);
            new_image[row][col].blue = (blue_color * scaling_factor);   
        }
        // Return the new 2D vector after the nested for loop is complete   
    }
    return new_image;
}

//Max function for process 10 max value:
double max(int x, int y, int z)
{
    double result = x;
    if (x > y && x > z) 
    {
        result = x;
    }
    if (y > x && y > z) 
    {
        result = y;
    }
    if (z > x && z > y) 
    {
        result = z;
    }
    return result;
}
vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)
{
    // Get the number of rows/columns from the input 2D vector (remember: num_rows is height, num_columns is width)
    int num_rows = image.size();
    int num_columns = image[0].size();
    
    // Define a new 2D vector the same size as the input 2D vector
    vector<vector<Pixel>> new_image(num_rows, vector<Pixel> (num_columns));
    

    // For each of the rows in the input 2D vector
    for (int row = 0; row < num_rows; row++)
    {
        // For each of the columns in the input 2D vector
        for (int col = 0; col < num_columns; col++)
        {
            // Get the color values for the pixel located at this row and column in the input 2D vector
            int red_color = image[row][col].red;
            int green_color = image[row][col].green;
            int blue_color = image[row][col].blue;
            
            double max_color = max(red_color, green_color, blue_color);

            // Perform the operation on the color values (refer to Runestone for this)
        // & Save the new color values to the corresponding pixel located at this row and column in the new 2D vector
            
            double total_values = (red_color + green_color + blue_color);
            
            if (total_values >= 550)
            {
                new_image[row][col].red = 255;
                new_image[row][col].green = 255;
                new_image[row][col].blue = 255;
            }
            else if (total_values <= 150)
            {
                new_image[row][col].red = 0;
                new_image[row][col].green = 0;
                new_image[row][col].blue = 0;
            }
            else if (max_color == red_color)
            {
                new_image[row][col].red = 255;
                new_image[row][col].green = 0;
                new_image[row][col].blue = 0;
            }
            else if (max_color == green_color)
            {
                new_image[row][col].red = 0;
                new_image[row][col].green = 255;
                new_image[row][col].blue = 0;
            }
            else
            {
                new_image[row][col].red = 0;
                new_image[row][col].green = 0;
                new_image[row][col].blue = 255;
            }

        }
        // Return the new 2D vector after the nested for loop is complete
    }
    return new_image;
}

int main()
{
    // Read in BMP image file into a 2D vector (using read_image function)
    
    // Call process_1 function using the input 2D vector and save the result returned to a new 2D vector

    // Write the resulting 2D vector to a new BMP image file (using write_image function)
    //
    // YOUR CODE HERE
    //
    
	cout << "CSPB1300 Image Processing Application"<< endl;
    string filename;
    cout << "Enter input BMP filename: ";
    cin >> filename;
    vector<vector<Pixel>> input_image = read_image(filename);

      
    cout << "IMAGE PROCESSING MENU"<< endl;
    cout << "0) Change image (current: " << filename << ")" << endl;
    cout << "1) Vignette" << endl;
    cout << "2) Clarendon" << endl;
    cout << "3) Grayscale" << endl;
    cout << "4) Rotate 90 degrees" << endl;
    cout << "5) Rotate multiple 90 degrees" << endl;
    cout << "6) Enlarge" << endl;
    cout << "7) High contrast" << endl;
    cout << "8) Lighten" << endl;
    cout << "9) Darken" << endl;
    cout << "10) Black, white, red, green, blue" << endl;
    cout << endl;
    cout << "Enter menu selection (Q to quit): " << endl;
    
    string process;
    while (cin >> process)
    {
        if (process == "0")
        {
            cout << endl;
            cout << "Change Image Selected" << endl;
            cout << "Enter new input BMP filename: ";
            cin >> filename;
            cout << "Successfully changed input image to " << filename << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
            
        else if (process == "1")
        {
            cout << endl;
            cout << "Vignette selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_1(input_image);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied vignette!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "2")
        {
            cout << endl;
            cout << "Clarendon selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            double scaling_factor;
            cout << "Enter scaling factor: ";
            cin >> scaling_factor;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_2(input_image, scaling_factor);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied clarendon by scaling factor of "<< scaling_factor << "!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "3")
        {
            cout << endl;
            cout << "Grayscale selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_3(input_image);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied grayscale!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
    
        else if (process == "4")
        {
            cout << endl;
            cout << "Rotate 90 degrees selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_4(input_image);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied 90 degree rotation!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "5")
        {
            cout << endl;
            cout << "Rotate multiple 90 degrees selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            int number;
            cout << "Enter number of 90 degree rotations: ";
            cin >> number;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_5(input_image, number);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied " << number <<" 90 degree rotations!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "6")
        {
            cout << endl;
            cout << "Enlarge selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            int x_scale;
            int y_scale;
            cout << "Enter X scale: ";
            cin >> x_scale;
            cout << "Enter Y scale: ";
            cin >> y_scale;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_6(input_image, x_scale, y_scale);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully enlarged X scale by * " << x_scale << " and Y scale by * " << y_scale << "!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        
        else if (process == "7")
        {
            cout << endl;
            cout << "High contrast selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_7(input_image);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied high contrast!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "8")
        {
            cout << endl;
            cout << "Lighten selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            double scaling_factor;
            cout << "Enter scaling factor: ";
            cin >> scaling_factor;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_8(input_image, scaling_factor);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully lightened by scaling factor of "<< scaling_factor << "!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "9")
        {
            cout << endl;
            cout << "Darken selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            double scaling_factor;
            cout << "Enter scaling factor: ";
            cin >> scaling_factor;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_9(input_image, scaling_factor);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully darkened by scaling factor of "<< scaling_factor << "!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "10")
        {
            cout << endl;
            cout << "Black, white, red, green, blue selected" << endl;
            string new_filename;
            cout << "Enter output BMP filename: ";
            cin >> new_filename;
            
            vector<vector<Pixel>> input_image = read_image(filename);
            vector<vector<Pixel>> filter_image = process_10(input_image);
            bool success = write_image(new_filename, filter_image);
            
            cout << "Successfully applied black, white, red, green, blue filter!" << endl;
            cout << endl;
            cout << "IMAGE PROCESSING MENU"<< endl;
            cout << "0) Change image (current: " << filename << ")" << endl;
            cout << "1) Vignette" << endl;
            cout << "2) Clarendon" << endl;
            cout << "3) Grayscale" << endl;
            cout << "4) Rotate 90 degrees" << endl;
            cout << "5) Rotate multiple 90 degrees" << endl;
            cout << "6) Enlarge" << endl;
            cout << "7) High contrast" << endl;
            cout << "8) Lighten" << endl;
            cout << "9) Darken" << endl;
            cout << "10) Black, white, red, green, blue" << endl;
            cout << endl;
            cout << "Enter menu selection (Q to quit): " << endl;
        }
        
        else if (process == "Q" || process == "q")
        {
            cout << endl;
            cout << "Thank you for using my program!" << endl;
            cout << "Quitting..." << endl;
            cout << endl;
            break;
            
        }
        else if (process < "0" || process > "10")
        {
            cout << endl;
            cout << "Please enter a valid integer (0-10) or Q to quit:" << endl;
            cout << endl;
        }
        
    }
            
        
            
    

    return 0;
}
