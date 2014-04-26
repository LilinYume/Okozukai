#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include "time_stamp.h"

int inpt();
void write_to_disk( int );
bool write_confirm( int& );
void view_history();

int main()
{
	char chose = -1;
	int inpt_val = 0;
	std::cout << "0: exit  1: new input  2: view history" << std::endl;
	std::cout << "type number as command > ";
	while( std::cin >> chose ) {
		switch( chose ) {
			case '0' :
				exit( EXIT_SUCCESS );
				break;
			case '1' :
				inpt_val = inpt();
				write_to_disk( inpt_val );
				std::cout << "type number as command > ";
				break;
			case '2' :
				view_history();
				break;
			case '3' :
				// ?
				break;
			default :
				std::cout << "not in list\n";
				continue;
		}
	}
	return 0;
}
int inpt()
{
	int cost = 0;
	std::cout << "Decrease: ";
	std::cin >> cost;

	if ( !std::cin ) {
		if ( std::cin.eof() ) {
			std::cin.clear( std::ios::goodbit );
			return -1;
		}
		if ( std::cin.fail() ) {
			std::cout << "not a number, try again" << std::endl;
			std::cin.clear( std::ios::goodbit );
			std::cin.ignore( sizeof cost, '\n' );
		}
		inpt();
	}
	if ( cost < 0 ) cost = -cost;
	return cost;
}
void write_to_disk( int write_val )
{
	using time_stamp::Date;

	if ( write_confirm( write_val ) == true ) {
		Date stamp;
		const char* file_name = stamp.get_month_lable();

		std::ofstream fs_out( file_name, std::ios::binary | std::ios::app );

		if ( !fs_out ) {
			std::cerr << "file cannot open\n";
			exit( EXIT_FAILURE );
		}
		fs_out  << stamp.get_day() << ' '
				<< stamp.get_week_lable() << ' ' 
				<< stamp.get_time() << ' '
				<< write_val << std::endl;

		std::cout << "success!" << std::endl;
	}
	else {
		std::cout << "not recorded" << std::endl;
	}
}
bool write_confirm( int& inpted )
{
	char ans = '\0';

	if ( inpted < 0 ) return false;
	std::cout << "write to disk file, are you ok? (y/n) ";

	while( std::cin >> ans ) {
		if ( ans == 'n' || ans == 'N' ) return false;
		else if ( ans == 'y' || ans == 'Y' ) return true;
		else  {
			std::cout << "write to disk file, are you ok? (y/n) ";
			std::cin.ignore( sizeof ans );
		}
	}
	return false;
}
void view_history()
{
	using time_stamp::Date;

	Date stamp;
	const char* file_name = stamp.get_month_lable();

	std::ifstream fs_in( file_name, std::ios::binary );

	if ( !fs_in ) {
		std::cerr << "file cannot open\n";
		exit( EXIT_FAILURE );
	}

	std::string cost, date, week ,time;
	cost = date = week = time = " ";

	int field_size = 0;
	while( !fs_in.eof() ) {
		fs_in >> date >> week >> time >> cost;
		
		if ( cost != "" ) {
			std::cout	<< date << ' '
						<< week << ' '
						<< time << ' '
						<< cost << std::endl;
			field_size = date.size() + week.size() + time.size() + cost.size();
		} else {
			std::cout.width( field_size );
			std::cout.fill('-');
			std::cout << '-' << std::endl;
		}
		cost = date = week = time = "";
	}
	std::cout << "type number as command > ";
}