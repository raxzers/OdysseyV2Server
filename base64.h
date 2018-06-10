//
// Created by raxzers on 6/4/18.
//

#ifndef ODYSSEYSERVER2_0_BASE64_H
#define ODYSSEYSERVER2_0_BASE64_H


#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);


#endif //ODYSSEYSERVER2_0_BASE64_H
