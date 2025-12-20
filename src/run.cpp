#include "inc.h"
#include "utils.h"
#include "macros.h"

ccodes color_codes_rt;
var_struct var_struct_ref_runtime;
controllers controllers_ref_run;

bool silence_arg_control = false;
bool silence_urslt_arg_control = false;

namespace run {
	void execute_directive(const std::string& directive, bool silence_control) {
     	int system_res = system(directive.c_str());
     	
 		// silence control
 		if(silence_control != true && silence_arg_control != true) 
 			std::cout << color_codes_rt.color_execute_s << str_utils::ltrim(directive) << color_codes_rt.color_execute_e << "\n";
 		
 		// directive result control
 		if(system_res != 0 && silence_urslt_arg_control == false) 
 			std::cout << color_codes_rt.color_warning_s << RES_ERROR_0 << system_res << RES_ERROR_1 << color_codes_rt.color_warning_e << "\n";
	}

	std::string execute_command_and_return(const char* command) {
		char buffer[256];
		FILE* storage;
		std::string result;
		
		#ifdef _WIN32
			storage = _popen(command, "r");
		#else
			storage = popen(command, "r");
		#endif
		
		while(fgets(buffer, 256, storage) != NULL) 
			result += buffer;

		#ifdef _WIN32
			_pclose(storage);
		#else
			pclose(storage);
		#endif
		
		int pos = 0;
		
		while(pos != std::string::npos) {
			pos = result.find('\n', pos);
			
			if(pos == std::string::npos) 
				break;
				
    		result.replace(pos, 1, " ");
    		pos += 1;
		} 

		return result;
	}
	
	bool error_control(const std::string& control_value, const int& error_line, std::string parameter = "") {
		const std::string error_msg_str = color_codes_rt.color_error_s + ERROR_0 + std::to_string(error_line) + ERROR_1;
		const std::string error_msg_end = color_codes_rt.color_error_f + "\n";	

		if(control_value == ERROR_FILE_NOT_EXISTS) {
			std::cout << error_msg_str << EC000 << error_msg_end;			
			return true;
		} else if(control_value == ERROR_TARGET_NOT_EXISTS) {
			std::cout << error_msg_str << EC001 << error_msg_end;
			std::cout << parameter;
			
			return true;
		} else if(control_value == ERROR_NULL_VAR_NAME) {
			std::cout << error_msg_str << EC00 << error_msg_end;
			return true;
		} else if(control_value == ERROR_NULL_VALUE) {
			std::cout << error_msg_str << EC01 << error_msg_end;
	    	return true;
		} else if(control_value == ERROR_VAR_NOT_EXISTS) {
			std::cout << error_msg_str << EC02 << "->" << parameter << error_msg_end;
			return true;
		} else if(control_value == ERROR_DEPS_NOT_EXISTS) {
			std::cout << error_msg_str << EC03 << "->" << parameter << error_msg_end;
			return true;
		} else if(control_value == ERROR_PATH_O_NOT_EXISTS) {
			std::cout << error_msg_str << EC04 << "->" << parameter << error_msg_end;
			return true;
		} else {
			return false;
		}	
	}
}
