#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "time_stamp.h"

void create_new_file( const char* file_name );
bool is_empty_file( const char* file_name );
int inpt( const char* prompt_messeage );
int inpt( const char* prompt_messeage, int& input_status );
int inpt_status( std::istream& );
void write_to_disk( const char* file_name, int value );
bool write_confirm();
void view_history( const char* file_name );

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
/* ファイル名を今月の月名で作成・残高の入力を促す。
	既に今月のファイルが作成済みならmain関数に制御を戻す。
	ただし、ファイル自体が作成されていてもファイルサイズがゼロの場合残高の入力を促す。
	入力値は整数値のみを許容。
	入力値がゼロまたはEOFがの場合はプログラムを終了する								　*/
void create_new_file( const char* file )
{
	int balance = -1;
	std::ofstream fs_out( file, std::ios::binary | std::ios::app );

	if ( !fs_out ) {
		std::cerr << "file cannot open\n";
		exit( EXIT_FAILURE );
	}

	if ( is_empty_file( file ) == false ) return;

	int status = NG;
	
	while( status == NG ) {
		balance = inpt( "balance: ", status );

		if ( status == EOF || balance == 0 ) {
			std::cout << "not recored" << std::endl;
			exit( EXIT_SUCCESS );
		}
	}
	fs_out << "balance: " << balance << std::endl;
}
/* ファイルが空かどうかを調べる
	ファイルの先頭から1文字取得し、即座にEOFが帰ってくればファイルが空（サイズ・ゼロ）とみなす。
	ファイルが空の場合 True を返し、ファイルが空でなければ False を返す。					*/
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
/* 整数値の入力を促す
	入力ストリームのステータスが Good状態になるまで再入力を促し、
	入力された整数値を返す													 */
int inpt( const char* prompt_messeage ) 
{
	int inpt_val = 0;

	do {
		std::cout << prompt_messeage;
		std::cin >> inpt_val;
	} while( inpt_status( std::cin ) == NG );

	return inpt_val;
}
/* 整数値の入力を促す
	入力ストリームのステータスを引数で参照渡しする。
	入力された整数値を返す													 */
int inpt( const char* prompt_messeage, int& input_status )
{
	int inpt_val = -1;
	
	std::cout << prompt_messeage;
	std::cin >> inpt_val;

	if ( !std::cin ) {
		if ( std::cin.eof() ) {
			input_status = EOF;
			std::cin.clear();
		}
		else if ( std::cin.fail() ) {
			input_status = NG;
			std::cin.clear();
			std::cin.ignore( sizeof inpt_val );
		}
	}
	else input_status = OK;
	return inpt_val;
}
/* 入力ストリームのストリーム状態を返す
	EOF なら シンボル定数 EOF: -1
	Fail なら シンボル定数 NG: 0
	Good なら シンボル定数 OK: 1
	をそれぞれ返す。
*/
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
/* ディスク・ファイルに入力された整数値を書き込む。
	ファイルへ書き込む前に書き込み確認をする。
	ファイル名は今月の月名
	フォーマット：日付 [space] 曜日 [space] 入力値 [LF]			*/
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
/* ファイルへの書き込み確認
	YesかNoを入力させる。
	入力値（char）は y Y n N のいずれかに収まるようにする。
	Yes('y' または 'Y' の入力)なら Trueを返す。
	No( 'n' または 'N' の入力)なら Falseを返す。				*/
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
/*	支出の履歴を表示する
	 今月の月名ファイルを開きファイル内の値をコンソール上に出力。
	 また、出費の合計を計算し、出費合計も同様に出力する。				*/
void view_history( const char* file )
{
	int amount = 0;
	std::vector<std::string> sum_of_cost;
	std::string balance, date, week, costs;
	balance = date = week = "";
	

	std::ifstream fs_in ( file, std::ios::binary );
	if ( !fs_in ) {
		std::cerr << "file cannot open\n";
		exit ( EXIT_FAILURE );
	}
	getline( fs_in, balance );
	std::cout << balance << std::endl;
	
	while(  fs_in >> date >> week >> costs  ) {
		std:: cout << date << ' ' << week << ' ' << costs << std::endl;
		sum_of_cost.push_back( costs );
	}

	for ( int i = 0; i < sum_of_cost.size(); ++i ) {
		amount += atoi( sum_of_cost[i].c_str() );
	}
	std::cout << "----------------------------------" << std::endl;
	std::cout << "total costs: " << amount << std::endl;
	std::cout << "type command: ";
}
