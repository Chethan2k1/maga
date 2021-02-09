#pragma once

#include <iostream>

#include <boost/system/error_code.hpp>

namespace error {
    inline void print(const boost::system::error_code &ec){
        std::cerr << "Error Code : " << ec.value() << ", Message : " << ec.message() << std::endl;
    }
}