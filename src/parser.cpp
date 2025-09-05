#include "inc.h"
#include "parser.h"
#include "utils.h"
#include "run.h"
#include "macros.h"

var_struct var_struct_ref_parser;
ccodes color_codes_parser;
controllers controllers_ref_parser;

void controllers_set_true() {
	controllers_ref_parser.sys_controller    = true;
	controllers_ref_parser.target_controller = true;
}

namespace parsers {
	std::string line_parser(const std::string& line, std::string pars_symbol_start, std::string pars_sybol_end, int to_parse) {
	    size_t pos = line.find(pars_symbol_start);
	    if(pos == std::string::npos) 
	    	return line;
	
	    std::string temp = line.substr(0, pos);
	    std::string temp_parameter;
	
	    while(pos != std::string::npos) {
	        size_t pose = line.find(pars_sybol_end, pos + pars_symbol_start.size());
	        if(pose == std::string::npos) 
	        	break;
	
	        temp_parameter = line.substr(pos + pars_symbol_start.size(), pose - pos - pars_symbol_start.size());
	
	        if(to_parse == 0) {
	            // to_parse = 0 -> var parsing
	            int tfi = str_utils::find_index(var_struct_ref_parser.var_name, temp_parameter);
	            if (tfi == -1) 
	            	return ERROR_VAR_NOT_EXISTS + temp_parameter;
	            else 
	            	temp += var_struct_ref_parser.var_value[tfi];
	        } else {
	        	// to_parse != 0 -> system command parsing
	        	temp += run::execute_command_and_return(temp_parameter.c_str());
	        }
	        	
	        pos = line.find(pars_symbol_start, pose + pars_sybol_end.size());
	        if(pos == std::string::npos) {
	            temp += line.substr(pose + pars_sybol_end.size()); // add the rest
	            break;
	        }
	
	        temp += line.substr(pose + pars_sybol_end.size(), pos - (pose + pars_sybol_end.size()));
	    } return temp;
	}
	
	bool var_def_parser(std::string read_line, int line_counter) {
		if(read_line.substr(0,1) == VAR_KEY) {
			if(run::error_control(enma_utils::find_var_name(read_line.substr(1,read_line.length()), ASSIGNMENT_KEY), line_counter)) {
				return false;
			} else if(run::error_control(enma_utils::find_var_value(read_line, ASSIGNMENT_KEY), line_counter)) {
				return false;
			} else {
				std::string temp_var_name = enma_utils::find_var_name(read_line.substr(1,read_line.length()), ASSIGNMENT_KEY);
				std::string temp_var_value = enma_utils::find_var_value(read_line, ASSIGNMENT_KEY);
				
				//change var value
				int tfi = str_utils::find_index(var_struct_ref_parser.var_name, temp_var_name);
				if(tfi != -1)  {
					var_struct_ref_parser.var_value[tfi] = temp_var_value;
				} else {
					//create new var
					var_struct_ref_parser.var_name.push_back(temp_var_name);
					var_struct_ref_parser.var_value.push_back(temp_var_value);
				}
			}
		} return true;
	}
	
	int detect_condition_parser(std::string& condition_data, std::string& read_line, int key_control, int key_length, bool eq_state) {
		///* check condition (if) or (elif) */
		if(key_control != -1) {
			// get condition data
			condition_data = read_line.substr(key_control + key_length, read_line.length() - key_control - 1);
	
			// condition check for (def->) - var control *trigger
			std::string def_key_ref = DEF_KEY;
			std::string condition_data_ltrimmed = str_utils::ltrim(condition_data);
			bool ref_ccparser = condition_control_parser(str_utils::ltrim(condition_data_ltrimmed.substr(def_key_ref.length(), condition_data_ltrimmed.length() - def_key_ref.length())), eq_state, 2);
					
			if(condition_data_ltrimmed.substr(0, def_key_ref.length()) == DEF_KEY && ref_ccparser) 
				return 1;
			else if(condition_data_ltrimmed.substr(0, def_key_ref.length()) == DEF_KEY && !ref_ccparser) 
				return 0;
		
			// condition check for match *trigger
			if (condition_control_parser(condition_data, eq_state, 1)) 
				return 1;
			else 
				return 0;
		} return -1;
	}
	
	bool condition_control_parser(const std::string& data, bool eq, int toControl) {
		if(toControl == 1) {
			// condition check for match
			std::istringstream isdata(data);
			bool first_run = false;
			std::string temp;
			std::string is_temp;
						
			while(isdata >> temp) {
				if(first_run && temp != is_temp) {
					if(eq) 
						return true;
					else 
						return false;
				}
				
				first_run = true;
				is_temp = temp;
			} 
			
			if(eq) 
				return false;
			else 
				return true;
		} else {
			// condition check for (def->) - var control   
			int find_index_ref = str_utils::find_index(var_struct_ref_parser.var_name, data);
			if(find_index_ref != -1 && !eq) 
				return true;
			else if(find_index_ref == -1 && eq) 
				return true;
			else 
				return false;
		} return false;
	}
	
	bool handle_condition_parser(bool condition_controller, std::string& read_line, int line_counter) {
		if(condition_controller == true) {
			if(read_line.substr(0,1) != AC_CON_KEY) 
				return false;
			else 
				var_def_parser(str_utils::ltrim(read_line), line_counter);
				
	    		return true;
	    } return false;
	}
	
	int deps_parser(const std::string& target, const std::string& deps) {
		if(!std::filesystem::exists(target)) {
			return -1;
		} else {
			// get target modify data
			auto target_time = std::filesystem::last_write_time(target);
			std::istringstream isdata(deps);
			std::string deps_temp;
	
			// deps date and exsists control
			while(isdata >> deps_temp) {
				if(!std::filesystem::exists(deps_temp)) 
					return -2;
				
				// if deps newer then target 
				if(target_time < std::filesystem::last_write_time(deps_temp)) 
					return 0;
			} return -3;
		}
	}
	
	void main_parser(const std::string& file_path, const std::string& argument) {
		bool directive_controller = false;
		bool condition_controller = false;
		bool condition_eq_controller = false;
		bool condition_false_control = false;
		bool condition_else_detect = false;
		
		int line_counter = 0;
		
		std::ifstream enmfilef(file_path);
		std::string read_line;
	
		// condition vars
		int key_control;
		std::string condition_key_for_length;
	
		bool handle_condition_parser_result;
		int detect_condition_parser_result;
		
		bool silence_control = false;
	
		std::string condition_data;
		std::string deps;
	
		while(std::getline(enmfilef, read_line)) {
			line_counter++;
	
			read_line = line_parser(read_line, USE_VAR_START, USE_VAR_END, 0);
			std::string var_not_exists_error = ERROR_VAR_NOT_EXISTS;
			// === var exists control ===
			if(read_line.substr(0, var_not_exists_error.length()) == ERROR_VAR_NOT_EXISTS) {
				run::error_control(ERROR_VAR_NOT_EXISTS, line_counter, read_line.substr(var_not_exists_error.length(), read_line.length() - var_not_exists_error.length()));
				controllers_set_true();
				break;
			} else {
				read_line = line_parser(read_line, "s>", "<s", 1);
			}
				
			// === comment line control ===
			if(read_line.substr(0,1) == COMMENT_KEY) 
				continue;
			
			// === set dep and parsing read_line ===
	 		key_control = str_utils::find_key_index(read_line, DEP_KEY);
			if(key_control != -1) {
				// get deps
				deps = read_line.substr(key_control + 6, read_line.length() - key_control + 1);
				// depless line
				read_line = str_utils::trim(read_line.substr(0, key_control));
			}
			
			// === check deps ===
			int deps_parser_result = deps_parser(read_line, deps);
			if(str_utils::trim(read_line) == argument) { 
				if(deps_parser_result == -2) {
					// deps not found
					run::error_control(ERROR_DEPS_NOT_EXISTS, line_counter, deps);
					controllers_set_true();
					break;
				} 

				if(deps_parser_result == -3) {
					// target newer then deps
					controllers_set_true();
					std::cout << color_codes_parser.color_warning_s << UP_TO_DATE_0 << read_line << UP_TO_DATE_1 << color_codes_parser.color_warning_e << "\n";
					continue;
				}
			}	
			
			// === control for "if" condition ===
			key_control = str_utils::find_key_index(read_line, IF_KEY);
			condition_key_for_length = IF_KEY;                                                                                                  //eq
			detect_condition_parser_result = detect_condition_parser(condition_data, read_line, key_control, condition_key_for_length.length(), false);
			
			if(detect_condition_parser_result == 1) {
				// condition is true
				condition_false_control = false;
				condition_controller = detect_condition_parser_result;
				continue;
			}
			
			if(detect_condition_parser_result == 0) {
				// condition is false
				condition_false_control = true;
				continue;
			}
						
			// run in "if"
			handle_condition_parser_result = handle_condition_parser(condition_controller, read_line, line_counter);
			
		    if(!handle_condition_parser_result) 
		    		condition_controller = handle_condition_parser_result;
			else 
				continue;
			
			// === control for "elif" condition ===
			key_control = str_utils::find_key_index(read_line, ELIF_KEY);
			condition_key_for_length = ELIF_KEY;                                                                                                //eq
			detect_condition_parser_result = detect_condition_parser(condition_data, read_line, key_control, condition_key_for_length.length(), true);
			
			if(detect_condition_parser_result == 1) {
				//condition is true
				condition_false_control = false;
				condition_controller = detect_condition_parser_result;
				continue;
			}

			if(detect_condition_parser_result == 0) {
				// condition is false
				condition_false_control = true;
				continue;
			}
	
			// run in "ELIF" 
			handle_condition_parser_result = handle_condition_parser(condition_controller, read_line, line_counter);
			
			if(!handle_condition_parser_result) 
				condition_controller = handle_condition_parser_result;
			else 
				continue;
	
			// === detect and run in "else" ===
			if(str_utils::rtrim(read_line) == ELSE_KEY) {
				condition_else_detect = true;
				continue;
			} 
			
			if(condition_else_detect) {
				handle_condition_parser_result = handle_condition_parser(condition_false_control, read_line, line_counter);
				if(!handle_condition_parser_result) {
					condition_false_control = handle_condition_parser_result;
					condition_else_detect = false;
				} else {
					continue;
				} condition_else_detect = false;
			}
			
			// === process and execute on directive ===
			if(directive_controller == true) {
				// silence control
				if(str_utils::trim(read_line.substr(0,2)) == SILENCE_KEY) {
					read_line = DIRECTIVE_KEY + read_line.substr(2, read_line.length() - 1);
					silence_control = true;
				}
									
				controllers_ref_parser.target_controller = true;
				if(read_line.substr(0,1) != DIRECTIVE_KEY) 
					directive_controller = false;
				else 
					run::execute_directive(read_line, silence_control);
			} 

			if(str_utils::trim(read_line) == argument) 
				directive_controller = true;
			
			// === register variables on memory(vectors) ===
			if(!var_def_parser(read_line,line_counter)) 
				break;
			
		} 
		if(controllers_ref_parser.target_controller == false) 
			run::error_control(ERROR_TARGET_NOT_EXISTS, 0);
	}
}
