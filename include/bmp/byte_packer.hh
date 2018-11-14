#ifndef BYTE_PACKER_HH
#define BYTE_PACKER_HH
#include <algorithm>
#include <cstdint>
#include <bitset>
namespace zinhart
{
  namespace bmp
  {
	template<class T>
  	  void endian_switch(T & obj)
	  {
		using uchar = unsigned char;
		T * tptr =  & obj;
		uchar * mem = (uchar*)(tptr);//reinterpret_cast<uchar*>(tptr);
		std::reverse(mem, mem + sizeof(T));
	  }
	template<class T>
	  void endian_switch(T * obj)
	  {
		using uchar = unsigned char;
		uchar * mem = (uchar*)(obj);//reinterpret_cast<uchar*>(obj);
		std::reverse(mem, mem + sizeof(T));
	  }

	template<class FundamentalType1, class FundamentalType2>
	  class byte_packer;

	template<>
	  class byte_packer<std::uint8_t, std::uint32_t>
	  {
		public:
		  using uchar = std::uint8_t;
		  using uint = std::uint32_t;
		  byte_packer() = default;
		  byte_packer(const byte_packer &) = default;
		  byte_packer(byte_packer &&) = default;
		  byte_packer & operator = (const byte_packer &) = default;
		  byte_packer & operator = (byte_packer&&) = default;
		  uint operator()(uchar & byte1, uchar & byte2, uchar & byte3, uchar & byte4)
		  {
			uint ret = 0;
			ret |= byte4;
			ret |= (byte3 << 8);
			ret |= (byte2 << 16);
			ret |= (byte1 << 24);
			return ret;
		  }	  
		  std::bitset<32> operator()(std::bitset<32> ret, uchar & byte1, uchar & byte2, uchar & byte3, uchar & byte4)
		  {
			ret.reset();
			ret |= byte4;
			ret |= (byte3 << 8);
			ret |= (byte2 << 16);
			ret |= (byte1 << 24);
			return ret;
		  }
	  };
	template<>
	  class byte_packer<std::uint8_t, std::uint64_t>
	  {
		public:
		  using uchar = std::uint8_t;
		  using uint = std::uint64_t;
		  byte_packer() = default;
		  byte_packer(const byte_packer &) = default;
		  byte_packer(byte_packer &&) = default;
		  byte_packer & operator = (const byte_packer &) = default;
		  byte_packer & operator = (byte_packer&&) = default;
		  uint operator()(uchar byte1 = 'z', uchar byte2 = 'i', uchar byte3 = 'n', uchar byte4 = 'h', uchar byte5 = 'a', uchar byte6 = 'r', uchar byte7 ='t', uchar byte8 = 0)
		  {
			uint ret = 0;
			byte_packer<uchar,std::uint32_t> bp;
			ret |= bp(byte1,byte2,byte3,byte4);
			ret<<=32;
			ret |= bp(byte5,byte6,byte7,byte8);
			return ret;
		  }
		  std::bitset<64> operator()(std::bitset<64> ret, uchar byte1 = 'z', uchar byte2 = 'i', uchar byte3 = 'n', uchar byte4 = 'h', uchar byte5 = 'a', uchar byte6 = 'r', uchar byte7 ='t', uchar byte8 = 0)
		  {
			byte_packer<uchar,std::uint32_t> bp;
			ret |= bp(byte1,byte2,byte3,byte4);
			ret<<=32;
			ret |= bp(byte5,byte6,byte7,byte8);
			return ret;
		  }
	  };
  }// END NAMESPACE BMP
}// END NAMESPACE ZINHART
#endif
