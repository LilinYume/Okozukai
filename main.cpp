#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "time_stamp.h"

void create_new_file( const char* );
bool is_empty_file( const char* );
int inpt( const char* );
int inpt_status( std::istream& );
void write_to_disk( const char* , int );
bool write_confirm();
void view_history( const char* );

const int OK = 1;
const int NG = 0;

int main() 
{
	using time_stamp::Date;
	Date stamp;
	const char* file_name = stamp.get_month_lable();

	create_new_file( file_name );

	char command = -1;
	int cost = 0;

	std::cout << "0: exit    1: new input    2: view history" << std::endl;
	std::cout << "type command: ";
	while ( std::cin >> command ) {
		switch ( command ) {
			case '0':
				exit( EXIT_SUCCESS );
				break;
			case '1':
				cost = inpt( "Decrease: " );
				write_to_disk( file_name, cost );
				break;
			case '2':
				view_history( file_name );
				break;
			default:
				std::cout	<< "not listed. tyr again\n"
							<< "type command: ";
				continue;
		}
	}
	return 0;
}

void create_new_file( const char* file )
{
	int balance = 0;
	std::ofstream fs_out( file, std::ios::binary | std::ios::app );

	if ( !fs_out ) {
		std::cerr << "file cannot open\n";
		exit( EXIT_FAILURE );
	}

	if ( is_empty_file( file ) == false ) return;

	std::cout << "balance: ";
	std::cin >> balance;
	
	if ( !std::cin ) {
		if ( std::cin.eof() ) {
			std::cout << "terminating..." << std::endl;
			exit( EXIT_FAILURE );
		}
		if ( std::cin.fail() ) {
			std::cin.clear();
			std::cin.ignore( sizeof balance );
			create_new_file( file );
		}
	}
	if ( balance == 0 ) {
		std::cout << "not recored" << std::endl;
		exit( EXIT_SUCCESS );
	} 
	else {
		fs_out << "balance: " << balance << std::endl;
	}
}
bool is_empty_file( const char* file )
{
	std::ifstream fs_in( file, std::ios::binary );

	if ( !fs_in ) {
		std::cerr << "file cannot open\n";
		exit( EXIT_FAILURE );
	}
	int status = fs_in.get();

	if ( status == EOF ) return true;
	return false;
}
int inpt( const char* prompt_messeage ) 
{
	int inpt_val = 0;

	do {
		std::cout << prompt_messeage;
		std::cin >> inpt_val;
	} while( inpt_status( std::cin ) == NG );

	return inpt_val;
}
int inpt_status( std::istream& stream_in ) 
{
	if ( stream_in.eof() ) {
		stream_in.clear();
		return EOF;
	}
	if ( stream_in.fail() ) {
		std::streambuf *cin_buf = stream_in.rdbuf();
		stream_in.clear();
		stream_in.ignore( sizeof *cin_buf, '\n' );
		std::cout << "invlid type, try again" << std::endl;
		return NG;
	}
	return OK;
}
void write_to_disk( const char* file, int write_val )
{
	if ( write_val == 0 ) { 
		std::cout	<< "not reacorded\n"
					<< "type command: ";
		return;
	}
	using time_stamp::Date;
	Date stamp;

	std::ofstream fs_out ( file, std::ios::binary | std::ios::app );
	if ( !fs_out ) {
		std::cerr << "file cannot open\n";
		exit( EXIT_FAILURE );
	}
	if ( write_confirm() == OK ) {
		fs_out	<< stamp.get_day() << ' '
				<< stamp.get_week_lable() << ' '
				<< write_val << std::endl;
		std::cout	<< "success!\n" 
					<< "type command: ";
	}
	else {
		std::cout	<< "not reacorded\n"
					<< "type command: ";
	}
}
bool write_confirm() 
{
	char chose = '\0';
	
	std::cout << "write to disk file, are you ok? (y/n) ";
	std::cin >> chose;

	if ( inpt_status( std::cin ) == EOF ) return false;

	else if ( chose == 'y' || chose == 'Y' ) {
		return true;
	}
	else if ( chose == 'n' || chose == 'N' ) {
		return false;
	}
	else {
		std::cin.ignore( sizeof chose );
		write_confirm();
	}
	return false;
}
void view_history( const char* file )
{
	std::string balance, rows;
	balance = rows = "";

	std::ifstream fs_in ( file, std::ios::binary );
	if ( !fs_in ) {
		std::cerr << "file cannot open\n";
		exit ( EXIT_FAILURE );
	}
	getline( fs_in, balance );
	std::cout << balance << std::endl;
	
	while( getline( fs_in, rows ) ) {
		std::cout << rows << std::endl;
	}
	std::cout << "----------------------------------" << std::endl;
	std::cout << "type command: ";
}