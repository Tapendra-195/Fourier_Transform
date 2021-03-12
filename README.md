# Fourier_Transform
There are two file the first is Fourier_data which you can use to draw any shape in image or you can draw over image, which will store the data in text file. Then the program Fourier does discrete Fourier transform in the data to get the function in frequency domain and show it visually.</br>

# Compiling process
1)cmake .</br>
2)make</br>
3)./Fourier_data [image_path] -- to get coordinates</br>
4)./Fourier </br>
# Function to get coordinate from the image
Fourier_data.cpp</br>
left click and drag to get data.</br>
right click for undo.</br>
double click left mouse button to save and exit.</br>

# Function to perform Fourier transform </br>
Fourier.cpp</br>

# Sample data
coordinate.txt is the sample data. The code Fourier can directly be used after using </br>
1)cmake . </br>
2)make </br>
3)./Fourier =>to run on the sample data</br>

# Requirements
1)OpenCV</br>
2)C++</br>
3)cmake (to compile)
