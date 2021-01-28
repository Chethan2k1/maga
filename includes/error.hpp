#pragma once

#include <iostream>

#include <boost/system/error_code.hpp>

namespace error {
    void print(boost::system::error_code &ec){
        std::cerr << "Error Code : " << ec.value() << ", Message : " << ec.message() << std::endl;
    }
}