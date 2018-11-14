#ifndef BMP_PARSER_HH
#define BMP_PARSER_HH
//#include <experimental/filesystem> perhaps when its not experimental
#include <fstream>
#include <regex>
namespace zinhart
{
  namespace bmp
  {
  	enum file_type : char {ascii = 0, BIN};
	enum binary_file_format : char {GENERIC = 0, IDX = 1};
	class csv_format
	{
	  public:
		using uint = unsigned int;
		csv_format() = default;
		csv_format(const csv_format&) = default;
		csv_format(csv_format&&) = default;
		csv_format & operator =(const csv_format&) = default;
		csv_format& operator =(csv_format&&) = default;
		~csv_format() = default;
		void set_delimeters(char start = '#', char middle = ',', char end = '|')
		{
		  delimiters.reserve(3);
		  delimiters.push_back(start);
		  delimiters.push_back(middle);
		  delimiters.push_back(end);
		}
		std::string get_delimeters()const
		{
		  return delimiters;
		}
	  private:
		std::string delimiters;
	};

	template<file_type file>
	  class bmp_parser;

	template<>
	  class bmp_parser<ascii>
	  {
		public:
		  using uchar = std::uint8_t;
		  using uint = std::uint32_t;
		  bmp_parser() = default;
		  bmp_parser(const bmp_parser&) = default;
		  bmp_parser(bmp_parser&&) = default;
		  bmp_parser & operator = (const bmp_parser&) = default;
		  bmp_parser& operator = (bmp_parser&&) = default;
		  ~bmp_parser() = default;
		  template<class Container>
			void write(std::fstream & file_handle, Container & c, std::string delim, const std::string & file, const char eol = '\n')
			{
			  using iter =  typename Container::iterator;
			  using uint = unsigned int;
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::out | std::ios::app );
			  }
			  std::string line,temp;
			  line.reserve(c.size() * 2 + 1);
			  line.resize(0);
			  std::regex delim_regex("\\" +  delim + "+");
			  std::sregex_token_iterator srti_end;
			  for(uint i = 0; i < c.size(); ++i)
			  {
				std::sregex_token_iterator srti(c[i].begin(), c[i].end(), delim_regex, -1);
				for(;srti != srti_end; ++srti)
				{
				  line +=  srti->str();
				  line += delim;
				}
				line.pop_back();
				file_handle<<line<<eol;
				line.clear();
			}
			file_handle.close();
		  }
		  
		  template<class Container>
			void write_line(std::fstream & file_handle, const char delimiter, const Container & c, const std::string & file, const char eol = '\n')
			{
			  static_assert(std::is_class<Container>::value && std::is_fundamental< typename Container::value_type>::value,"only containers of primitive types may be recieved as arguments to encode");  
			  using iter =  typename Container::const_iterator;
			  using uint = unsigned int;
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::out );
			  }
			  std::string line;
			  line.reserve(c.size() * 2 + 1);// perform 1 allocation
			  line.resize(0);
			  for(uint i = 0; i < c.size(); ++i)
			  {
				line.push_back(static_cast<char>(c[i]));
				line.push_back(delimiter);
			  }
			  line.erase(line.begin() + line.size() -1); // git rid of delimeter behind the last value
			  file_handle<<line<<eol;
			  file_handle.close();
			}
		  template<class Container>
			void write_line_app(std::fstream & file_handle, const char delimiter, const Container & c, const std::string & file, const char eol = '\n')
			{
			  static_assert(std::is_class<Container>::value && std::is_fundamental< typename Container::value_type>::value,"only containers of primitive types may be recieved as arguments to encode");  
			  using iter =  typename Container::const_iterator;
			  using uint = unsigned int;
			  if(!file_handle.is_open())
			  {
				file_handle.open( file,  std::ios::app | std::ios::out );
			  }
			  std::string line;
			  line.reserve(c.size() * 2 + 1);// perform 1 allocation
			  line.resize(0);
			  for(uint i = 0; i < c.size(); ++i)
			  {
				line.push_back(static_cast<char>(c[i]));
				line.push_back(delimiter);
			  }
			  line.erase(line.begin() + line.size() -1); // git rid of delimeter behind the last value
			  file_handle<<line<<eol;
			  file_handle.close();
			}

		  template <class Container>
			void read_line(std::fstream & file_handle, Container & output, char delim, const std::string & file)
			{
			  static_assert(std::is_same<typename Container::value_type, std::string>::value,"Only containers of strings may be recieved as an argument to strip");

			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in );
			  }
			  std::string line;
			  output.clear();
			  while(std::getline(file_handle,line,delim))
			  {
				output.push_back(line);
			  }
			  file_handle.close();
			}
		  template <class Container>
			void read(std::fstream & file_handle, Container & output, char delim, const std::string & file)
			{
			  static_assert(std::is_same<typename Container::value_type, std::string>::value,"Only containers of strings may be recieved as an argument to strip");
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in );
			  }
			  std::string line;
			  output.clear();
			  while(std::getline(file_handle,line,delim))
			  {
				output.push_back(line);
			  }
			  file_handle.close();
			}
		  template <class Container>
			void read(std::fstream & file_handle, csv_format & fmt, Container & output, const std::string & file)
			{
			  static_assert(std::is_same<typename Container::value_type, std::string>::value,"Only containers of strings may be recieved as an argument to strip");
			  using uint = unsigned int;
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in);
			  }
			  std::string line;
			  std::string delimiters = fmt.get_delimeters();
			  while(std::getline(file_handle,line,delimiters[2])){output.push_back(line);}
			  file_handle.close();
			}
		  template<class Container, class Callback>
			void read(std::fstream & file_handle, Container & output, const std::string & file, char delim, Callback callback)
			{
			  static_assert(std::is_same<typename Container::value_type, std::string>::value,"Only containers of strings may be recieved as an argument to read");  
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in );
			  }
			  std::string line;
			  output.clear();
			  while(std::getline(file_handle,line,delim))
			  {
				output.push_back(line);
			  }
			  std::for_each(output.begin(),output.end(),[&callback](std::string & init){ callback(init);});
			  file_handle.close();
			}
	  };

	template<>
	  class bmp_parser<BIN>
	  {
		public:
		  using uchar = std::uint8_t;
		  using uint = std::uint32_t;
		  bmp_parser() = default;
		  bmp_parser(const bmp_parser&) = default;
		  bmp_parser(bmp_parser&&) = default;
		  bmp_parser & operator = (const bmp_parser&) = default;
		  bmp_parser& operator = (bmp_parser&&) = default;
		  ~bmp_parser() = default;

		  template<class Container>
			void write(std::fstream & file_handle,  std::string & file, Container & header)
			{
			  static_assert(std::is_class<Container>::value,"Container needs to be a class");
			  if(!file_handle.is_open())
			  {
				file_handle.open(file, std::ios::out | std::ios::binary | std::ios::app);
			  }
			  else
			  {
				file_handle.close();
				file_handle.open( file, std::ios::out | std::ios::binary | std::ios::app);
			  }
			  for(auto begin = header.begin(); begin != header.end(); ++begin)
			  {
				endian_switch(*begin);
				file_handle.write((char *)&(*begin), sizeof(*begin));
			  }
			  file_handle.close();
			}
		  template<class Container>
			void read(std::fstream & file_handle,  std::string & file,  Container & header, std::streampos pos = 0)
			{
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in | std::ios::binary);
			  }
			  else
			  {
				file_handle.close();
				file_handle.open( file, std::ios::in | std::ios::binary);
			  }
			  file_handle.seekg(pos,std::ios::beg);
			  for(auto begin = header.begin(); begin != header.end(); ++begin)
			  {
				file_handle.read((char *)&(*begin), sizeof(*begin));
				endian_switch(*begin);
			  }
			  file_handle.close();
			}
		  template<class Container,class Block_Size>
			void read(std::fstream & file_handle,  std::string & file, Container & header, Block_Size size, std::streampos pos)
			{
			  static_assert(std::is_integral<Block_Size>::value, "Block_Size must be a integral type");
			  static_assert(std::is_class<Container>::value,"Container needs to be a class");
			  static_assert(std::is_integral<typename Container::value_type>::value, "Block_Size must be a integral type");
			  if(!file_handle.is_open())
			  {
				file_handle.open( file, std::ios::in | std::ios::binary);
			  }
			  else
			  {
				file_handle.close();
				file_handle.open( file, std::ios::in | std::ios::binary);
			  }
			  file_handle.seekg(pos,std::ios::beg);
			  for(auto begin = header.begin(); begin != header.end(); ++begin)
			  {
				file_handle.read((char *)&size, sizeof(size));
				endian_switch(size);
				(*begin) = size;
			  }
			  file_handle.close();
			}
	  };

	template<file_type file>
	  class csv_writer;

	template<>
	  class csv_writer<ascii> : public bmp_parser<ascii> 
	  {
		public:
		  csv_writer() = default;
		  csv_writer(const csv_writer&) = default;
		  csv_writer(csv_writer&&) = default;
		  csv_writer & operator =(const csv_writer&) = default;
		  csv_writer& operator =(csv_writer&&) = default;
		  ~csv_writer() = default;
		  using bmp_parser::write;
		private:
		  using bmp_parser::read;
	  };

	template<>
	  class csv_writer<BIN> : public bmp_parser<BIN> 
	  {
		public:
		  csv_writer() = default;
		  csv_writer(const csv_writer&) = default;
		  csv_writer(csv_writer&&) = default;
		  csv_writer & operator =(const csv_writer&) = default;
		  csv_writer& operator =(csv_writer&&) = default;
		  ~csv_writer() = default;
		  using bmp_parser::write;
		private:
		  using bmp_parser::read;
	  };

	template<file_type file>
	  class csv_reader;

	template<>
	  class csv_reader<ascii> : public bmp_parser<ascii> 
	  {
		public:
		  csv_reader() = default;
		  csv_reader(const csv_reader&) = default;
		  csv_reader(csv_reader&&) = default;
		  csv_reader & operator =(const csv_reader&) = default;
		  csv_reader& operator =(csv_reader&&) = default;
		  ~csv_reader() = default;
		  using bmp_parser::read;
		private:
		  using bmp_parser::write;
	  };

	template<>
	  class csv_reader<BIN> : public bmp_parser<BIN> 
	  {
		public:
		  csv_reader() = default;
		  csv_reader(const csv_reader&) = default;
		  csv_reader(csv_reader&&) = default;
		  csv_reader & operator =(const csv_reader&) = default;
		  csv_reader& operator =(csv_reader&&) = default;
		  ~csv_reader() = default;
		  using bmp_parser::read;
		private:
		  using bmp_parser::write;
	  };
  }// END NAMESPACE BMP
}// END NAMESPACE ZINHART
#endif //BITMAP_PARSER_HH
