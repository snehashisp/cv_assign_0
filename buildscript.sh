echo "This script assumes opencv is installed correctly in the linux system libraries"

g++ -o splitvideo Q1/code1.cpp $(pkg-config --cflags --libs opencv)
g++ -o combinevideo Q1/code2.cpp $(pkg-config --cflags --libs opencv)
g++ -o webcamframes Q2/code.cpp $(pkg-config --cflags --libs opencv)
g++ -o chromakey Q3/code.cpp $(pkg-config --cflags --libs opencv)





