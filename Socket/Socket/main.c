/*
	Joshua Miner and Ethan Welsh
	CS1652 Fall 2014
	Project 1: Part 1
	HTTP Client
*/

int main(int argc, char **argv)
{
    if(argc!=5)	// If there are not 5 arguments on the command line
    {
        printf("Improperly formatted command line. Try again \n");		// Print what the error is
        printf("Format is http_client k|u server_name server_port server_path\n");		// Format
        return EXIT_FAILURE;
    }
    if(argv[1]=='k')	// Operating in the Kernel Stack
    {
        return 0;	// All operations were successful
    }
    else if(argv[1]=='u')
    {
        // Operating in the User (Minet) Stack
        // We don't have to build this one for the project?
        return 0;	// Operations were successful
    }
    else
    {
        return -1;		// There was an issue encountered
    }
}