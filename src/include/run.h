#ifndef RUN_H
#define RUN_H

extern bool silence_arg_control;
extern bool silence_urslt_arg_control;

namespace run {
	static bool silence_control_arg = false;
	
	void execute_directive(const std::string& directive, bool silence_control);
	std::string execute_command_and_return(const char* command);
	bool error_control(const std::string& control_value, const int& error_line, std::string parameter = "");
}

#endif
