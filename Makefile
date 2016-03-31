all: HuffmanEncoder HuffmanDecoder

%: %.cpp
	g++ -std=c++11 $< -o $@

%: %.c
	gcc $< -o $@

