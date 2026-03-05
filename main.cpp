#include "inc.h"
#include "run.h"
#include "utils.h"
#include "macros.h"
#include "parser.h"

ccodes color_codes;
var_struct var_struct_ref;
controllers controllers_ref;

int main(int argc, char* argv[]) 
{
	const std::string current_path = std::filesystem::current_path().string();
	std::string argument;
	std::string file_path;

	if (argc < 2)
		argument = DEFAULT_ARG;
	else
		argument = argv[1];

	// argument control ---
	int arg_control_counter = 0;

	while (1) 
	{
		//* -h help argument control
		if (argument == HELP_ARG_KEY) 
		{
			std::cout << HELP_OUT << "\n";
			return 0;
		}

		//* -v version argument control
		if (argument == VERSION_ARG_KEY) 
		{
		    std::cout << VERSION_OUT << VERSION << "\n";
		    return 0;
		}

		file_path = current_path + "/" + FILE_READ_KEY;

		//* -s silence argument control
		if (argument == SILENCE_ARG_KEY) 
		{
			silence_arg_control = true;

			if (argc < 3)
				argument = DEFAULT_ARG;
			else 
				argument = argv[arg_control_counter + 1];
		}

		//* -sur silence unexpected results argument control
		if (argument == SILENCE_URSLT_ARG_KEY)
		{
			silence_urslt_arg_control = true;

			if (argc < 3)
				argument = DEFAULT_ARG;
			else
				argument = argv[arg_control_counter + 1];
		}

		//* -p change directory argument control
		if (argument == PATH_OPTION_ARG_KEY) 
		{
			file_path = argv[arg_control_counter + 2];

			if (!std::filesystem::exists(file_path))
			{
				run::error_control(ERROR_PATH_O_NOT_EXISTS, 0, file_path);
				return -1;
			}

			std::filesystem::current_path(std::filesystem::current_path() / file_path);

			if (argv[argc - 1] != file_path)
				argument = argv[argc - 1];
			else
				argument = DEFAULT_ARG;

			file_path = FILE_READ_KEY;
			break;
		}

		if (arg_control_counter > argc) 
			break; 

		arg_control_counter++;
	}

	/* ======= SET SYSTEM AND RUN PARSING ======= */
	if (std::filesystem::exists(file_path) && !(std::filesystem::is_directory(file_path))) {
		#ifdef _WIN32
			set_ter_color(color_codes);
		#endif

		// RUN PARSING
		parsers::main_parser(file_path, argument);

		if (controllers_ref.sys_controller == false) 
			return 1;

		return 0;
	} 
	else 
	{
		run::error_control(ERROR_FILE_NOT_EXISTS, 0);
		return 1;
	}
}
