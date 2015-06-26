#ifndef PROVIDED_INCLUDED
#define PROVIDED_INCLUDED

#include <string>
#include <vector>

class BinaryConverter
{
public:
	static std::string encode(const std::vector<unsigned short>& numbers);
	static bool decode(const std::string& bitString,
					   std::vector<unsigned short>& numbers);
private:
	  // We prevent BinaryConverter objects from being created by declaring
	  // all constructors private and not implementing them.
	BinaryConverter();
	BinaryConverter(const BinaryConverter&);
};

class Compressor
{
public:
	static void compress(const std::string& s,
						 std::vector<unsigned short>& numbers);
	static bool decompress(const std::vector<unsigned short>& numbers,
						   std::string& s);
private:
	  // We prevent Compressor objects from being created by declaring
	  // all constructors private and not implementing them.
	Compressor();
	Compressor(const Compressor&);
};

class Steg
{
public:
	static bool hide(const std::string& hostIn, const std::string& msg,
					 std::string& hostOut);
	static bool reveal(const std::string& host, std::string& msg);
private:
	  // We prevent Steg objects from being created by declaring
	  // all constructors private and not implementing them.
	Steg();
	Steg(const Steg&);
};

class WebSteg
{
public:
	static bool hideMessageInPage(const std::string& url,
								  const std::string& msg,
					 			  std::string& host);
	static bool revealMessageInPage(const std::string& url, std::string& msg);
private:
	  // We prevent WebSteg objects from being created by declaring
	  // all constructors private and not implementing them.
	WebSteg();
	WebSteg(const WebSteg&);
};

#endif // PROVIDED_INCLUDED
