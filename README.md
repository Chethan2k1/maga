# maga
This is very [WIP] Asynchronous Web Server written in C++17
# dependencies
- boost::asio
- [magahttpparser](https://github.com/Chethan2k1/magahttpparser)
# build
If boost is installed in the system, there is a good chance the headers will be detected automatically, if not to specify path to boost header files use `-DBOOST_INCLUDE_DIR=<PATH_TO_HEADER_FILES>`

[magahttpparser](https://github.com/Chethan2k1/magahttpparser) if already downloaded it can be pointed towards by including `-DMAGAHTTPPARSER_ROOT=<PATH_TO_HEADER_FILES>` flag or else just by not including this flag will download and build along with maga

To not build the `test` target in [magahttpparser](https://github.com/Chethan2k1/magahttpparser) add `-DENABLE_TESTS=OFF` flag

```
git clone https://github.com/Chethan2k1/maga.git
cd maga
cmake -H. -Bbuild -DBOOST_INCLUDE_DIR=/usr/local/include -DENABLE_TESTS=OFF
cmake --build build
```
