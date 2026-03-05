#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cctype>
#include <algorithm>

struct ccodes 
{
    std::string color_execute_s = "\033[32m";
    std::string color_execute_e = "\033[0m";
    std::string color_warning_s = "\033[33m";
    std::string color_warning_e = "\033[0m";
    std::string color_error_s   = "\033[31m";
    std::string color_error_f   = "\033[0m";
};

struct var_struct 
{
	std::vector<std::string> var_name;
	std::vector<std::string> var_value;
};

struct controllers 
{
	bool sys_controller    = false;
	bool target_controller = false;
};

	static void set_ter_color(ccodes& color_ref) 
	{
		color_ref.color_execute_s   = "";
		color_ref.color_execute_e   = "";
		std::string color_warning_s = "";
		std::string color_warning_e = "";
		color_ref.color_error_s     = "";
		color_ref.color_error_f     = "";
	}
