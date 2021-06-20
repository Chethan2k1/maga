# maga
This is very [WIP] Web Server
# dependencies
- boost asio
- magahttpparser
# build
If boost is installed in the system, there is a good chance the headers will be detected automatically, if not to specify path to boost header files use ```-DBOOST_INCLUDE_DIR=<PATH_TO_HEADER_FILES>```

magahttpparser if already downloaded it can be pointed towards by using ```-DMAGAHTTPPARSER_ROOT=<PATH_TO_HEADER_FILES>``` or else just by not including this flag will download and build along with maga

```git clone https://github.com/Chethan2k1/maga.git```

```cd maga```

```cmake -H. -Bbuild -DBOOST_INCLUDE_DIR=/usr/local/include```

```cmake --build build```