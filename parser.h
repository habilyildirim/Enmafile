#ifndef PARSER_H
#define PARSER_H

namespace parsers 
{
	std::string line_parser(const std::string& line, std::string pars_symbol_start, 
										std::string pars_symbol_end, int to_parse);
	bool var_def_parser(std::string read_line, int line_counter);

	int deps_parser(const std::string& target, const std::string& deps);
	
	int detect_condition_parser(std::string& condition_data, std::string& read_line, 
									int key_control, int key_length, bool eq_state);
	bool condition_control_parser(const std::string& data, bool eq, int toControl);
	bool handle_condition_parser(bool condition_controller, std::string& read_line, 
																 int line_counter);
	
	void main_parser(const std::string& file_path, const std::string& argument);	
}

#endif
