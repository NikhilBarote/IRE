
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring> // memset()

#include "myparser.h"

using namespace std;
string final_folder;
int main(int argc, char* argv[])
{
	// Set the global C and C++ locale to the user-configured locale,
	// so we can use cout with UTF-8, via Glib::ustring, without exceptions.
	locale::global(locale(""));

	string filepath;
	if(argc > 1 ){
		filepath = argv[1]; //Allow the user to specify a different XML file to parse.
		if(argc>2){
			final_folder=argv[2];
			if(final_folder[final_folder.size()-1]!='/'){
				final_folder+="/";
			}
		}
	}
	else{
		filepath = "sample.xml";
		final_folder="./";
	}

	// Parse the entire document in one go:
	int return_code = EXIT_SUCCESS;
	/*	try
		{
		MySaxParser parser;
		parser.set_substitute_entities(true);
		parser.parse_file(filepath);
		}
		catch(const xmlpp::exception& ex)
		{
		cerr << "libxml++ exception: " << ex.what() << endl;
		return_code = EXIT_FAILURE;
		}
	 */
	// Incremental parsing, sometimes useful for network connections:
	try
	{
		// cout << endl << "Incremental SAX Parser:" << endl;

		ifstream is(filepath.c_str());
		if (!is)
			throw xmlpp::exception("Could not open file " + filepath);

		char buffer[1000000];
		const size_t buffer_size = sizeof(buffer) / sizeof(char);

		//Parse the file:
		MySaxParser parser;
		parser.set_substitute_entities(true);
		do
		{
			memset(buffer, 0, buffer_size);
			is.read(buffer, buffer_size-1);
			if(is.gcount())
			{
				// We use Glib::ustring::ustring(InputIterator begin, InputIterator end)
				// instead of Glib::ustring::ustring( const char*, size_type ) because it
				// expects the length of the string in characters, not in bytes.
				Glib::ustring input(buffer, buffer+is.gcount());
				parser.parse_chunk(input);
			}
		}
		while(is);

		parser.finish_chunk_parsing();
	}
	catch(const xmlpp::exception& ex)
	{
		cerr << "Incremental parsing, libxml++ exception: " << ex.what() << endl;
		return_code = EXIT_FAILURE;
	}

	return return_code;
}

