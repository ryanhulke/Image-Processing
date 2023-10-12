#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <math.h>
using namespace std;

struct Picture
{
	struct Header
	{
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short height;
		short width;
		char bitsPerPixel;
		char imageDescriptor;
	};

	struct Pixel
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
		Pixel(unsigned char b, unsigned char g, unsigned char r)
		{
			this->b = b;
			this->g = g;
			this->r = r;
		}
	};
	Header header;
	vector<Pixel> pixels;
	Picture(){}
	Picture(Header header, vector<Pixel>& pixels)
	{
		this->header = header;
		this->pixels = pixels;
	}
};
bool ReadFile(string path, Picture& picture)
{
	ifstream file(path, ios_base::binary);
	if (file.is_open())
	{
		char idLength;
		file.read((char*)&idLength, sizeof(idLength));
		char colorMapType;
		file.read((char*)&colorMapType, sizeof(colorMapType));
		char dataTypeCode;
		file.read((char*)&dataTypeCode, sizeof(dataTypeCode));
		short colorMapOrigin;
		file.read((char*)&colorMapOrigin, sizeof(colorMapOrigin));
		short colorMapLength;
		file.read((char*)&colorMapLength, sizeof(colorMapLength));
		char colorMapDepth;
		file.read((char*)&colorMapDepth, sizeof(colorMapDepth));
		short xOrigin;
		file.read((char*)&xOrigin, sizeof(xOrigin));
		short yOrigin;
		file.read((char*)&yOrigin, sizeof(yOrigin));
		short width;
		file.read((char*)&width, sizeof(width));
		short height;
		file.read((char*)&height, 2);
		char bitsPerPixel;
		file.read((char*)&bitsPerPixel, 1);
		char imageDescriptor;
		file.read((char*)&imageDescriptor, 1);
		Picture::Header header = { idLength, colorMapType, dataTypeCode, colorMapOrigin,colorMapLength, colorMapDepth, xOrigin, yOrigin, width,height, bitsPerPixel, imageDescriptor };
		vector<Picture::Pixel> pixels;
		int numPixels = width * height;
		for (int j = 0; j < numPixels; j++)
		{
			unsigned char b;
			file.read((char*) &b, 1);
			unsigned char g;
			file.read((char*) &g, 1);
			unsigned char r;
			file.read((char*) &r, 1);
			Picture::Pixel newPixel(b, g, r);
			pixels.push_back(newPixel);
		}
		picture = Picture(header, pixels);
		file.close();
		return true;
	}
	else return false;
}
void WriteFile(string path, Picture& picture)
{
	ofstream file(path, ios_base::binary);
	if (file.is_open())
	{
		char idLength = picture.header.idLength;
		file.write((char*)&idLength, sizeof(idLength));
		char colorMapType = picture.header.colorMapType;
		file.write((char*)&colorMapType, sizeof(colorMapType));
		char dataTypeCode = picture.header.dataTypeCode;
		file.write((char*)&dataTypeCode, sizeof(dataTypeCode));
		short colorMapOrigin = picture.header.colorMapOrigin;
		file.write((char*)&colorMapOrigin, sizeof(colorMapOrigin));
		short colorMapLength = picture.header.colorMapLength;
		file.write((char*)&colorMapLength, sizeof(colorMapLength));
		char colorMapDepth = picture.header.colorMapDepth;
		file.write((char*)&colorMapDepth, sizeof(colorMapDepth));
		short xOrigin = picture.header.xOrigin;
		file.write((char*)&xOrigin, sizeof(xOrigin));
		short yOrigin = picture.header.yOrigin;
		file.write((char*)&yOrigin, sizeof(yOrigin));
		short width = picture.header.width;
		file.write((char*)&width, sizeof(width));
		short height = picture.header.height;
		file.write((char*)&height, sizeof(height));
		char bitsPerPixel = picture.header.bitsPerPixel;
		file.write((char*)&bitsPerPixel, sizeof(bitsPerPixel));
		char imageDescriptor = picture.header.imageDescriptor;
		file.write((char*)&imageDescriptor, sizeof(imageDescriptor));
		vector<Picture::Pixel> pixels;
		for (int j = 0; j < picture.header.width * picture.header.height; j++)
		{
			unsigned char b = picture.pixels[j].b;
			file.write((char*)&b, sizeof(b));
			unsigned char g = picture.pixels[j].g;
			file.write((char*)&g, sizeof(g));
			unsigned char r = picture.pixels[j].r;
			file.write((char*)&r, sizeof(r));
		}
		file.close();
	}
}
Picture Multiply(Picture& pic1, Picture& pic2)
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < pic1.header.height * pic1.header.width; i++)
	{
		unsigned char b = round((((float)pic1.pixels[i].b) / 255) * (((float)pic2.pixels[i].b) / 255) * 255);
		unsigned char g = round((((float)pic1.pixels[i].g) / 255) * (((float)pic2.pixels[i].g) / 255) * 255);
		unsigned char r = round((((float)pic1.pixels[i].r) / 255) * (((float)pic2.pixels[i].r) / 255) * 255);
		Picture::Pixel newPixel(b, g, r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic1.header, newPixels);
	return picture;
}
Picture Subtract(Picture& pic1, Picture& pic2)
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < pic1.header.height * pic1.header.width; i++)
	{
		int b = pic1.pixels[i].b - pic2.pixels[i].b;
		if (b > 255)
			b = 255;
		else if (b < 0)
			b = 0;
		int g = pic1.pixels[i].g - pic2.pixels[i].g;
		if (g > 255)
			g = 255;
		else if (g < 0)
			g = 0;
		int r = pic1.pixels[i].r - pic2.pixels[i].r;
		if (r > 255)
			r = 255;
		else if (r < 0)
			r = 0;
		Picture::Pixel newPixel((unsigned char) b, (unsigned char) g, (unsigned char) r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic1.header, newPixels);
	return picture;
}
Picture Add(Picture& pic1, Picture& pic2)
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < pic1.header.height * pic1.header.width; i++)
	{
		int b = (int) pic2.pixels[i].b + pic1.pixels[i].b;
		if (b > 255)
			b = 255;
		else if (b < 0)
			b = 0;
		int g = (int) pic2.pixels[i].g + pic1.pixels[i].g;
		if (g > 255)
			g = 255;
		else if (g < 0)
			g = 0;
		int r = (int) pic2.pixels[i].r + pic1.pixels[i].r;
		if (r > 255)
			r = 255;
		else if (r < 0)
			r = 0;
		Picture::Pixel newPixel((unsigned char)b, (unsigned char)g, (unsigned char)r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic1.header, newPixels);
	return picture;
}
Picture Screen(Picture& pic1, Picture& pic2)
{
	vector<Picture::Pixel> newPixels;

	for (int i = 0; i < pic1.header.height * pic1.header.width; i++)
	{
		unsigned char b = round((1 - (1 - (((float)pic1.pixels[i].b) / 255)) * (1 - (((float)pic2.pixels[i].b) / 255))) * 255);
		unsigned char g = round((1 - (1 - (((float)pic1.pixels[i].g) / 255)) * (1 - (((float)pic2.pixels[i].g) / 255))) * 255);
		unsigned char r = round((1 - (1 - (((float)pic1.pixels[i].r) / 255)) * (1 - (((float)pic2.pixels[i].r) / 255))) * 255);
		Picture::Pixel newPixel(b, g, r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic1.header, newPixels);
	return picture;
}
Picture Overlay(Picture& pic1, Picture& pic2)
{
	vector<Picture::Pixel> newPixels;

	for (int i = 0; i < pic1.header.height * pic1.header.width; i++)
	{
		unsigned char b, g, r;
		if ((double)pic2.pixels[i].b / 255 <= 0.5)
			b = round(2 * ((double)pic1.pixels[i].b * pic2.pixels[i].b) / 255);
		else
			b = round((1 - 2 * ((1 - (double)pic1.pixels[i].b / 255) * (1 - (double)pic2.pixels[i].b / 255))) * 255);
		if ((double)pic2.pixels[i].g / 255 <= 0.5)
			g = round(2 * ((double)pic1.pixels[i].g * pic2.pixels[i].g) / 255);
		else
			g = round((1 - 2 * ((1 - (double)pic1.pixels[i].g / 255) * (1 - (double)pic2.pixels[i].g / 255))) * 255);
		if ((double)pic2.pixels[i].r / 255 <= 0.5)
			r = round(2 * ((double)pic1.pixels[i].r * pic2.pixels[i].r) / 255);
		else
			r = round((1 - 2 * ((1 - (double)pic1.pixels[i].r / 255) * (1 - (double)pic2.pixels[i].r / 255))) * 255);
		Picture::Pixel newPixel(b, g, r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic1.header, newPixels);
	return picture;
}
Picture AddColor(Picture& pic, int color, int amount) // 0 = b, 1 = g, 2 = r
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < pic.header.height * pic.header.width; i++)
	{
		if (color == 0) {
			pic.pixels[i].b += amount;
			if (pic.pixels[i].b > 255)
				pic.pixels[i].b = 255;
		}
		else if (color == 1) {
			pic.pixels[i].g += amount;
			if (pic.pixels[i].g > 255)
				pic.pixels[i].g = 255;
		}
		else if (color == 2) {
			pic.pixels[i].r += amount;
			if (pic.pixels[i].r > 255)
				pic.pixels[i].r = 255;
		}
		Picture::Pixel newPixel(pic.pixels[i].b, pic.pixels[i].g, pic.pixels[i].r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic.header, newPixels);
	return picture;
}
Picture Scale(Picture& pic, int color, float factor) 
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < pic.header.height * pic.header.width; i++)
	{
		if (color == 0) {
			pic.pixels[i].b = (unsigned char)(pic.pixels[i].b * factor);
			if (pic.pixels[i].b > 255)
				pic.pixels[i].b = 255;
		}
		else if (color == 1) {
			pic.pixels[i].g = (unsigned char)(pic.pixels[i].g * factor);
			if (pic.pixels[i].g > 255)
				pic.pixels[i].g = 255;
		}
		else if (color == 2) {
			pic.pixels[i].r = (unsigned char)(pic.pixels[i].r * factor);
			if (pic.pixels[i].r > 255)
				pic.pixels[i].r = 255;
		}
		Picture::Pixel newPixel(pic.pixels[i].b, pic.pixels[i].g, pic.pixels[i].r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic.header, newPixels);
	return picture;
}
void SplitChannels(Picture& pic, vector<Picture>& picChannels) {
	vector<Picture::Pixel> newPixelsR;
	vector<Picture::Pixel> newPixelsG;
	vector<Picture::Pixel> newPixelsB;
	for (int i = 0; i < pic.header.height * pic.header.width; i++)
	{
		unsigned char b = pic.pixels[i].b;
		unsigned char g = pic.pixels[i].g;
		unsigned char r = pic.pixels[i].r;
		Picture::Pixel newPixelB( b, b, b);
		Picture::Pixel newPixelG(g, g, g);
		Picture::Pixel newPixelR(r, r, r);
		newPixelsB.push_back(newPixelB);
		newPixelsG.push_back(newPixelG);
		newPixelsR.push_back(newPixelR);
	}
	Picture B = Picture(pic.header, newPixelsB);
	Picture G = Picture(pic.header, newPixelsG);
	Picture R = Picture(pic.header, newPixelsR);
	picChannels.push_back(B);
	picChannels.push_back(G);
	picChannels.push_back(R);
}
Picture Combine(Picture& picR, Picture& picB, Picture& picG)
{
	vector<Picture::Pixel> newPixels;
	for (int i = 0; i < picB.header.height * picB.header.width; i++)
	{
		unsigned char b = picB.pixels[i].b;
		unsigned char g = picG.pixels[i].g;
		unsigned char r = picR.pixels[i].r;
		Picture::Pixel newPixel(b, g, r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(picB.header, newPixels);
	return picture;
}
Picture Flip(Picture& pic)
{
	vector<Picture::Pixel> newPixels;
	for (int i = pic.header.height * pic.header.width - 1; i > -1; i--)
	{
		unsigned char b = pic.pixels[i].b;
		unsigned char g = pic.pixels[i].g;
		unsigned char r = pic.pixels[i].r;
		Picture::Pixel newPixel(b, g, r);
		newPixels.push_back(newPixel);
	}
	Picture picture = Picture(pic.header, newPixels);
	return picture;
}
bool ComparePictures(Picture& pic1, Picture& pic2)
{
	if (pic1.header.idLength != pic2.header.idLength)
		return false;
	if (pic1.header.bitsPerPixel != pic2.header.bitsPerPixel)
		return false;
	if (pic1.header.colorMapDepth != pic2.header.colorMapDepth)
		return false;
	if (pic1.header.colorMapLength != pic2.header.colorMapLength)
		return false;
	if (pic1.header.colorMapOrigin != pic2.header.colorMapOrigin)
		return false;
	if (pic1.header.colorMapType != pic2.header.colorMapType)
		return false;
	if (pic1.header.dataTypeCode != pic2.header.dataTypeCode)
		return false;
	if (pic1.header.height != pic2.header.height)
		return false;
	if (pic1.header.width != pic2.header.width)
		return false;
	if (pic1.header.imageDescriptor != pic2.header.imageDescriptor)
		return false;
	if (pic1.header.xOrigin != pic2.header.xOrigin)
		return false;
	if (pic1.header.yOrigin != pic2.header.yOrigin)
		return false;
	for (int i = 0; i < pic1.header.width * pic1.header.height; i++)
	{
		if (pic1.pixels[i].b != pic2.pixels[i].b)
			return false;
		if (pic1.pixels[i].g != pic2.pixels[i].g)
			return false;
		if (pic1.pixels[i].r != pic2.pixels[i].r)
			return false;
	}
	return true;
}
int main(int argc, char* argv[])
{
	if (argc < 2 || strcmp(argv[1], "--help") == 0) {
		cout << "Project 2: Image Processing, Spring 2023\n\nUsage:\n    ./project2.out [output] [firstImage] [method] [...]\n";
		return 0;
	}
	string outputPath = argv[1];
	Picture trackingImage;
	if (outputPath.substr(outputPath.length() - 4, 4) != ".tga") {
		cout << "Invalid file name." << endl;
		return 0;
	}
	// iterating through all remaining arguments
	string inputPath = argv[2];
	for (int i = 3; i < argc;) {
		if (inputPath.substr(inputPath.length() - 4, 4) != ".tga") {
			cout << "Invalid file name." << endl;
			return 0;
		}
		if (!ReadFile(inputPath, trackingImage)) {
			cout << "File does not exist." << endl;
			return 0;
		}
		string operation = argv[i++];
		if (operation == "multiply") {
			cout << "mult.." << i << " " << argc << endl;
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			string path = argv[i++];	// argument index i (for which argument we're on gets shifted over 1 to account for operation
			cout << "i's" << endl;
			Picture pic;
			if (!ReadFile(path, pic)) {
				cout << "File does not exist." << endl;
			}
			trackingImage = Multiply(trackingImage, pic);
		}
		else if (operation == "subtract") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			string path = argv[i++];
			Picture pic;
			if (!ReadFile(path, pic)) {
				cout << "File does not exist." << endl;
			}
			trackingImage = Subtract(trackingImage, pic);
		}
		else if (operation == "overlay") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			string path = argv[i++];
			Picture pic;
			if (!ReadFile(path, pic)) {
				cout << "File does not exist." << endl;
			}
			trackingImage = Overlay(trackingImage, pic);
		}
		else if (operation == "screen") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			string path = argv[i++];
			Picture pic;
			if (!ReadFile(path, pic)) {
				cout << "File does not exist." << endl;
			}
			trackingImage = Screen(pic, trackingImage);
		}
		else if (operation == "combine") {
			if (i+1 >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			string path = argv[i++];
			Picture pic;
			string path2 = argv[i++];
			Picture pic2;
			if (!ReadFile(path, pic) || !ReadFile(path2, pic2)) {
				cout << "File does not exist." << endl;
			}
			trackingImage = Combine(trackingImage, pic, pic2);
		}
		else if (operation == "flip") {
			i++;
			trackingImage = Flip(trackingImage);
		}
		else if (operation == "onlyred") {
			vector<Picture> picChannels;
			SplitChannels(trackingImage, picChannels);
			trackingImage = picChannels[2];
		}
		else if (operation == "onlygreen") {
			vector<Picture> picChannels;
			SplitChannels(trackingImage, picChannels);
			trackingImage = picChannels[1];
		}
		else if (operation == "onlyblue") {
			vector<Picture> picChannels;
			SplitChannels(trackingImage, picChannels);
			trackingImage = picChannels[0];
		}
		else if (operation == "addred") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			try {
				int num = stoi(argv[i++]);
				trackingImage = AddColor(trackingImage, 2, num);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else if (operation == "addgreen") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			try {
				int num = stoi(argv[i++]);
				trackingImage = AddColor(trackingImage, 1, num);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else if (operation == "addblue") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
				try {
						int num = stoi(argv[i++]);
				trackingImage = AddColor(trackingImage, 0, num);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else if (operation == "scalered") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			try {
				float factor = stof(argv[i++]);
				trackingImage = Scale(trackingImage, 2, factor);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else if (operation == "scalegreen") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			try {
				float factor = stof(argv[i++]);
				trackingImage = Scale(trackingImage, 1, factor);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else if (operation == "scaleblue") {
			if (i >= argc) {
				cout << "Missing argument." << endl;
				return 0;
			}
			try {
				float factor = stof(argv[i++]);
				trackingImage = Scale(trackingImage, 0, factor);
			}
			catch (exception e) {
				cout << "Invalid argument." << endl;
				return 0;
			}
		}
		else {
			cout << "Invalid method name." << endl;
		}
	}
	WriteFile(outputPath, trackingImage);
	/*
	tests
	// task 1
	Picture layer1;
    ReadFile("input/layer1.tga", layer1);
	Picture pattern1; 
	ReadFile("input/pattern1.tga", pattern1);
	Picture out = Multiply(layer1, pattern1);
	WriteFile("output/part1.tga", out);

	// task 2
	Picture layer2;
	ReadFile("input/layer2.tga", layer2);
	Picture car;
	ReadFile("input/car.tga", car);
	Picture out2 = Subtract(car, layer2);
	WriteFile("output/part2.tga", out2);

	// task 3
	Picture pattern2;
	ReadFile("input/pattern2.tga", pattern2);
	Picture out3 = Multiply(layer1, pattern2);
	Picture text;
	ReadFile("input/text.tga", text);
	out3 = Screen(out3, text);
	WriteFile("output/part3.tga", out3);

	// task 4
	Picture circles;
	ReadFile("input/circles.tga", circles);
	Picture out4 = Multiply(circles, layer2);
	out4 = Subtract(out4, pattern2);
	WriteFile("output/part4.tga", out4);

	// task 5
	Picture out5 = Overlay(layer1, pattern1);
	WriteFile("output/part5.tga", out5);

	// task 6
	Picture out6 = AddGreen(car);
	WriteFile("output/part6.tga", out6);\

	// task 7
	Picture out7 = Scale(car);
	WriteFile("output/part7.tga", out7);

	// task 8
	vector<Picture> picChannels;
	SplitChannels(car, picChannels);
	Picture realOut8b;
	ReadFile("examples/EXAMPLE_part8_b.tga", realOut8b);
	WriteFile("output/part8_b.tga", picChannels[0]);
	WriteFile("output/part8_g.tga", picChannels[1]);
	WriteFile("output/part8_r.tga", picChannels[2]);

	// task 9
	Picture layerRed;
	ReadFile("input/layer_red.tga", layerRed);
	Picture layerGreen;
	ReadFile("input/layer_green.tga", layerGreen);
	Picture layerBlue;
	ReadFile("input/layer_blue.tga", layerBlue);
	Picture result = Combine(layerBlue, layerGreen, layerRed);
	WriteFile("output/part9.tga", result);
	
	// task 10
	Picture text2;
	ReadFile("input/text2.tga", text2);
	Picture flippedPic = Flip(text2);
	Picture realOut10;
	WriteFile("output/part10.tga", flippedPic);
	*/
    return 0;
}