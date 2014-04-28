#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include "time_stamp.h"

int inpt();
bool is_empty_file();
int passbook();
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
				// ?		もう少し足したい機能があるんだけど(´・ω・`)
				break;
			default :
				std::cout << "not in list\n";
				continue;
		}
	}
	return 0;
}

/* 出費の入力
	出費の入力を促す。
	入力値は整数値のみ受け付ける（int）。
	空のファイル（新規作成ファイル）の場合、先に貯金残高の入力を促す。
	EOFで操作をキャンセル。負の数は正の数に符号を替える。
	入力された出費を表す整数値を返す。												*/
int inpt()
{
	// ファイルの中身が空かチェック
	if ( is_empty_file() == true ) {
		int remain = passbook();

		if ( remain == 0 ) {
			std::cout << "not recoded" << std::endl;
		}
		else {
			write_to_disk( remain );
		}
	}
	int cost = 0;
	std::cout << "Decrease: ";
	std::cin >> cost;

	// 入力値が不正
	if ( !std::cin ) {
		if ( std::cin.eof() ) {
			std::cin.clear( std::ios::goodbit );
			return EOF;
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
/* ファイルの中身が空か調べる
	ファイル名は今月の月名を利用するため、
	新しい月の場合最初に貯金残高を入力させるのに使う。
	ファイル先頭から1文字読み取り、即座にEOF（-1）ならtrueを返す。			*/
bool is_empty_file()
{
	using time_stamp::Date;
	Date stamp;
	const char* file_name = stamp.get_month_lable();

	int count = 0;
	
	std::ifstream fs_in( file_name, std::ios::binary );
	count = fs_in.get();

	if ( count == EOF ) return true;
	return false;
}

/* 今月、月初めの貯金残高を入力させる
	空のファイル（新規作成）の場合のみ入力。
	ファイル名に今月の月名を利用するため、新規作成されるのは月を改めた場合になる。
	入力は整数値。																				*/
int passbook()
{
	int remain = 0;
	std::cout << "balance: ";
	std::cin >> remain;

	if ( !std::cin ) {
		if ( std::cin.eof() ) {
			std::cin.clear( std::ios::goodbit );
			return EOF;
		}
		if ( std::cin.fail() ) {
			std::cout << "invalid, try again." << std::endl;
			std::cin.clear( std::ios::goodbit );
			std::cin.ignore( sizeof remain );
		}
		passbook();
	}
	return remain;
}

/* ディスク・ファイルへ書き込む
	入力された出費、月初めの貯金残高などを引数にとり、
	引数の値と入力時の日時を書き込む。
	書き込む前に書き込みするか確認をする。
	書き込む値は 日時（固定長） のあと 引数の値（可変長）、適宜スペースで区切る。
	書き込み確認でNoの場合と、引数の整数値がゼロの場合書き込まない。
	このプログラムの目的では出費ゼロを書き込む意味が無い。								*/
void write_to_disk( int write_val )
{
	if ( write_val == 0 ) {
		std::cout << "not recorded" << std::endl;
		return;
	}
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

/* 書き込み確認
	ディスク・ファイルに書き込む前に本当に書き込むか確認する。							*/
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
/* 書き込みがされたファイルをコンソール上に表示する。
	読み込むのは常に、今月のファイル（ファイル名は今月の月名）。					*/
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

	int field_size = 25;

	while( !fs_in.eof() ) {
		fs_in >> date >> week >> time >> cost;
		
		if ( cost != "" ) {
			std::cout	<< date << ' '
						<< week << ' '
						<< time << ' '
						<< cost << std::endl;
		} else {
			std::cout.width( field_size );
			std::cout.fill('-');
			std::cout << '-' << std::endl;
		}
		cost = date = week = time = "";
	}
	std::cout << "type number as command > ";
}