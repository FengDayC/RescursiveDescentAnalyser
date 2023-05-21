#pragma once
#include<string>
#include<iostream>
#include<map>
#include<memory>
#include<fstream>
#include<sstream>
#include<vector>
#include<filesystem>
#include<stack>

/*###############################Utils########################################*/

#define S_PTR(_Type,_Name) std::shared_ptr<_Type> _Name
#define MK_SPTR(_Type,_ARGS_) std::make_shared<_Type>(_ARGS_)

/*###############################Utils########################################*/

/*###############################SYMBOL########################################*/

#define DIGIT(x) (x>='0'&&x<='9')
#define LETTER(x) ((x>='a'&&x<='z')||(x>='A'&&x<='Z'))
#define SPACE(x) (x==' ')

/*###############################SYMBOL########################################*/