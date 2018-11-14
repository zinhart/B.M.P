#ifndef CSV_PARSER_TEST_H
#define CSV_PARSER_TEST_H
#include <gtest/gtest.h>
//#include "csv_parser.h"
#include "../binary_parser"
#include <zinhart/vector_space>
class binary_parser_test : public ::testing::Test
{
  protected:
  	static void SetUpTestCase(){};
	static void TearDownTestCase(){};

};
TEST_F(binary_parser_test,ascii)
{
  using namespace zinhart;
  csv_format fmt;
  fmt.set_delimeters('#',',','\n');
  binary_parser<ASCII> p;
  vector_space<char,1> a({'a','b','c','d'});
  fstream strm;
  string file("ascii");
  p.write(strm,fmt,a,file);
  vector<std::string> vec;
  p.read(strm,fmt,vec,file);
  std::for_each(vec.begin(),vec.end(),[&vec](std::string & init){std::cout<<init<<"\n";});
  string iris_data_set("training-data/iris-flower-data-set");
  p.read(strm,vec,iris_data_set,'\n', [](std::string & init){init.insert(0,",");std::rotate(init.begin(),init.begin()+17,init.end());init.pop_back();});
  std::for_each(vec.begin(),vec.end(),[&vec](std::string & init){std::cout<<init<<"\n";});
  string iris("encoded-iris-data-set-ascii");
 // p.write(strm,fmt,vec,",",iris);
}

TEST_F(binary_parser_test,binary)
{
  /*
  using namespace zinhart;
  csv_format<BIN> fmt;
  fmt.set_delimeters('#',',','|');
  binary_parser p;
  vector_space<char,1> data({'a','b','c','d'});
  fstream f;
  std::string file = "bin";
  fstream strm;
  p.encode(strm,fmt,data,file);
  //vector_space<std::string,1> v;
  vector<std::string> v;
  p.decode(f,fmt,v,file);
 // std::for_each(v.begin(),v.end(),[&v](std::string & init){std::cout<<init<<"\n";});*/
}
TEST_F(binary_parser_test,mnist_test)
{
  using namespace zinhart;
  fstream strm;
  std::string file = "training-data/t10k-images-idx3-ubyte";

  binary_parser<BIN> prep;
  byte_packer<std::uint8_t, std::uint64_t> bp;
  vector_space<std::uint64_t,1> out_header_binaryp1;
  out_header_binaryp1.push_back(bp());
  vector_space<std::uint32_t,2>	original_image_set(10000, 784), copied_image_set(10000, 784) ;
  vector_space<std::uint32_t,1> header_original(4),out_headerp2({10000u,2u,28u,28u});
  vector_space<std::uint8_t,1> zinhart(8), img_binary(10000 * 784);
  vector_space<std::uint32_t,1> info_original(3), info_copy(4);
  
  
  const std::uint16_t count = 10;
  prep.read(strm,file,header_original);// read in original new header/magic number
  prep.read(strm,file,original_image_set,std::uint8_t(),16);// read in original images themselves
  std::cout<<"Header/Magic Num Original "<<header_original<<"\n";
  //loop is for visualZing
  for(std::uint32_t i = 0,start = 0, size = 784, stride = 1; i < count; ++i, start+=784)
  {
	auto img_slice = original_image_set.slice(start,size,stride); 
	vector_space<std::uint32_t,2> image(28,28);
	image.assign(img_slice.begin(),img_slice.end());
	std::cout<<image;
  }
  prep.read(strm,file,img_binary,16);// read in all images, 16 is 16 bytes
  file = "zinhart-mnist-10ktest-idx4"; 
  prep.write(strm, file, out_header_binaryp1);
  prep.write(strm, file, out_headerp2);
  prep.write(strm, file, img_binary);

  prep.read(strm,file,zinhart);// read in new header/magic number in new binary 
  prep.read(strm, file, info_copy,8);// read in number of images dimensions etc in new binary
  prep.read(strm, file, copied_image_set,std::uint8_t(), 24);// read in images themselves in new binary
  std::cout<<"Header/Magic Num Copy: "<<zinhart<< " info: "<<info_copy<<"\n"; 
  //loop is for visualizing
  for(std::uint32_t i = 0,start = 0, size = 784, stride = 1; i < 2/*count*/; ++i, start+=784)
  {
	auto img_slice = copied_image_set.slice(start,size,stride); 
	vector_space<std::uint32_t,2> image(28,28);
	image.assign(img_slice.begin(),img_slice.end());
	std::cout<<image;
  }
  //validate copy
  for (std::uint32_t i = 0; i < 10000; ++i)
	for(std::uint32_t j = 0; j < 784; ++j)
	  assert(original_image_set[i][j] == copied_image_set[i][j]);
  for(auto beg1 = original_image_set.begin(), beg2 = copied_image_set.begin(); beg1 != original_image_set.end(); ++beg1,++beg2 ) 
	assert((*beg1) == (*beg2));
/*	*/
}
TEST_F(binary_parser_test,mnist_train)
{
/*  using namespace zinhart;
  fstream strm;
  binary_parser<BIN> prep;
  byte_packer<std::uint8_t, std::uint64_t> bp;
  vector_space<std::uint64_t,1> out_header_binaryp1;
  out_header_binaryp1.push_back(bp());
  vector_space<std::uint32_t,2>	original_image_set(60000, 784), copied_image_set(60000, 784) ;
  vector_space<std::uint32_t,1> header_original(4),out_headerp2({60000u,2u,28u,28u});
  vector_space<std::uint8_t,1> zinhart(8), img_binary(60000 * 784);
  vector_space<std::uint32_t,1> info_original(3), info_copy(4);

  const std::uint16_t count = 3; 
  std::string  file = "training-data/train-images-idx3-ubyte";
  prep.read(strm,file,header_original);// read in original new header/magic number
  prep.read(strm,file,original_image_set,std::uint8_t(),16);// read in original images themselves
  std::cout<<"Header/Magic Num Original "<<header_original<<"\n";

  //loop is for visualzing
  for(std::uint32_t i = 0,start = 0, size = 784, stride = 1; i < count; ++i, start+=784)
  {
	auto img_slice = original_image_set.slice(start,size,stride); 
	vector_space<std::uint32_t,2> image(28,28);
	image.assign(img_slice.begin(),img_slice.end());
	std::cout<<image;
  }
  prep.read(strm,file,img_binary,16);// read in all images, 16 is 16 bytes

  file = "zinhart-mnist-60ktrain-idx4"; 
  prep.write(strm, file, out_header_binaryp1);//write header to new file
  prep.write(strm, file, out_headerp2);// write number of images dimensions etc in new binary
  prep.write(strm, file, img_binary);
 
  prep.read(strm,file,zinhart);// read in new header/magic number in new binary 
  prep.read(strm, file, info_copy,8);// read in number of images dimensions etc in new binary
  prep.read(strm, file, copied_image_set,std::uint8_t(), 24);// read in images themselves in new binary

  std::cout<<"Header/Magic Num Copy: "<<zinhart<< " info: "<<info_copy<<"\n"; 

  //loop is for visualizing
  for(std::uint32_t i = 0,start = 0, size = 784, stride = 1; i < count; ++i, start+=784)
  {
	auto img_slice = copied_image_set.slice(start,size,stride); 
	vector_space<std::uint32_t,2> image(28,28);
	image.assign(img_slice.begin(),img_slice.end());
	std::cout<<image;
  }
//validate copy
  for (std::uint32_t i = 0; i < 60000; ++i)
	for(std::uint32_t j = 0; j < 784; ++j)
	  assert(original_image_set[i][j] == copied_image_set[i][j]);
  for(auto beg1 = original_image_set.begin(), beg2 = copied_image_set.begin(); beg1 != original_image_set.end(); ++beg1,++beg2 ) 
	assert((*beg1) == (*beg2));*/
}
TEST_F(binary_parser_test,mnist_test_label)
{
/*  using namespace zinhart;
  fstream strm;
  std::string file = "training-data/t10k-labels-idx1-ubyte";
  binary_parser<BIN> prep;
  byte_packer<std::uint8_t, std::uint64_t> bp;
  vector_space<std::uint64_t,1> out_header_binaryp1;
  out_header_binaryp1.push_back(bp());
  vector_space<std::uint32_t,2>	original_image_set(10000, 1), copied_image_set(10000, 1) ;
  vector_space<std::uint32_t,1> header_original(2),out_headerp2({10000u, 1});
  vector_space<std::uint8_t,1> zinhart(8), label_binary(10000 * 1);
  vector_space<std::uint32_t,1> info_copy(2);
  
  prep.read(strm,file,header_original);// read in original new header/magic number
  prep.read(strm,file,original_image_set,std::uint8_t(),8);// read in original labels themselves
  std::cout<<"Header/Magic Num Original "<<header_original<<"\n";

  const std::uint16_t count = 10; 
  //loop is for visualzing
  for(std::uint32_t i = 0; i < count; ++i)
  {
	std::cout<<original_image_set[i][0]<<" "; 
  }
  prep.read(strm,file,label_binary,8);// read in all images, 16 is 16 bytes
  file = "zinhart-mnist-test-labels-idx1"; 
  prep.write(strm, file, out_header_binaryp1);//write header to new file
  prep.write(strm, file, out_headerp2);// write number of images dimensions etc in new binary
  prep.write(strm, file, label_binary);
 
  prep.read(strm,file,zinhart);// read in new header/magic number in new binary 
  prep.read(strm, file, info_copy,8);// read in number of images dimensions etc in new binary
  prep.read(strm, file, copied_image_set,std::uint8_t(), 16);// read in images themselves in new binary

  std::cout<<"\n Header/Magic Num Copy: "<<zinhart<< " info: "<<info_copy<<"\n"; 
  for (std::uint32_t i = 0; i < 10000; ++i)
	for(std::uint32_t j = 0; j < 1; ++j)
	  assert(original_image_set[i][j] == copied_image_set[i][j]);
  for(auto beg1 = original_image_set.begin(), beg2 = copied_image_set.begin(); beg1 != original_image_set.end(); ++beg1,++beg2 ) 
	assert((*beg1) == (*beg2));
	*/
}
TEST_F(binary_parser_test,mnist_train_label)
{
/*  using namespace zinhart;
  fstream strm;
  std::string file = "training-data/train-labels-idx1-ubyte";
  binary_parser<BIN> prep;
  byte_packer<std::uint8_t, std::uint64_t> bp;
  vector_space<std::uint64_t,1> out_header_binaryp1;
  out_header_binaryp1.push_back(bp());
  vector_space<std::uint32_t,2>	original_image_set(60000, 1), copied_image_set(60000, 1) ;
  vector_space<std::uint32_t,1> header_original(2),out_headerp2({60000u, 1});
  vector_space<std::uint8_t,1> zinhart(8), label_binary(60000 * 1);
  vector_space<std::uint32_t,1> info_copy(2);
  prep.read(strm,file,header_original);// read in original new header/magic number
  prep.read(strm,file,original_image_set,std::uint8_t(),8);// read in original labels themselves
  std::cout<<"Header/Magic Num Original "<<header_original<<"\n";

  const std::uint16_t count = 10; 
  //loop is for visualzing
  for(std::uint32_t i = 0; i < count; ++i)
  {
	std::cout<<original_image_set[i][0]<<" "; 
  }
  prep.read(strm,file,label_binary,8);// read in all images, 16 is 16 bytes
  file = "zinhart-mnist-train-labels-idx1"; 
  prep.write(strm, file, out_header_binaryp1);//write header to new file
  prep.write(strm, file, out_headerp2);// write number of images dimensions etc in new binary
  prep.write(strm, file, label_binary);
 
  prep.read(strm,file,zinhart);// read in new header/magic number in new binary 
  prep.read(strm, file, info_copy,8);// read in number of images dimensions etc in new binary
  prep.read(strm, file, copied_image_set,std::uint8_t(), 16);// read in images themselves in new binary

  std::cout<<"\n Header/Magic Num Copy: "<<zinhart<< " info: "<<info_copy<<"\n"; 
  for (std::uint32_t i = 0; i < 60000; ++i)
	for(std::uint32_t j = 0; j < 1; ++j)
	  assert(original_image_set[i][j] == copied_image_set[i][j]);
  for(auto beg1 = original_image_set.begin(), beg2 = copied_image_set.begin(); beg1 != original_image_set.end(); ++beg1,++beg2 ) 
	assert((*beg1) == (*beg2));
*/
}
#endif
