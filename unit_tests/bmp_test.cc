#include <gtest/gtest.h>
#include <bmp/bmp.hh>
#include <vector>
#include <fstream>
using namespace zinhart::bmp;
TEST(bmp_test, write_read_ascii)
{

  zinhart::bmp::bmp_parser<zinhart::bmp::ascii> parser;
  std::vector<char> write_ascii({'a','b','c','d'});
  std::vector<std::string> read_ascii;
  std::uint32_t i{0};  
  bmp_parser<ascii> bp;
  std::fstream strm;
  const std::string file("ascii");
  const char delimeter{','};
  bp.write_line(strm, delimeter, write_ascii,file);
  bp.read_line(strm, read_ascii, delimeter, file);
  for(i = 0; i < write_ascii.size(); ++i)
	ASSERT_EQ(read_ascii.at(i)[0], write_ascii[i]);
}
TEST(bmp_test, read_ascii_callback)
{
  // essentially moves iris class labels from end to beginning of each line through a callback
  bmp_parser<ascii> bp;
  std::uint32_t i{0};
  std::fstream strm;
  std::vector<std::string> read_iris;
  const std::string iris_data_set("iris-flower-dataset");
  bp.read(strm, read_iris, iris_data_set,'\n', [](std::string & init){init.insert(0,",");std::rotate(init.begin(),init.begin()+17,init.end());init.pop_back();});
  for(i = 0; i < read_iris.size(); ++i)
  {
	std::string & point{read_iris.at(i)};
	if(i < 50)
	{
	  std::string cmp(point.begin(), point.end() - 16);
	  ASSERT_EQ(cmp, std::string("Iris-setosa"));

	}
	else if(i > 50 && i < 100)
	{
	  std::string cmp(point.begin(), point.end() - 16);
	  ASSERT_EQ(cmp, std::string("Iris-versicolor"));
	}
	else if(i > 100 && i < 150)
	{
	  std::string cmp(point.begin(), point.end() - 16);
	  ASSERT_EQ(cmp, std::string("Iris-virginica"));
	}
  }
  const std::string encoded_iris("encoded-iris-data-set-ascii");
  bp.write(strm, read_iris,",", encoded_iris);
}

TEST(bmp_test,binary)
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
TEST(bmp_test,mnist_test)
{
  /*
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
  for(std::uint32_t i = 0,start = 0, size = 784, stride = 1; i < 2; ++i, start+=784)// replace 2 with count
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
	*/
}
TEST(bmp_test,mnist_train)
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
TEST(bmp_test,mnist_test_label)
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
TEST(bmp_test,mnist_train_label)
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
