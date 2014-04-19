﻿#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <string>
#include <cctype>

int inpt();
bool input_again( int& v );
void write_to_disk( int );
bool write_confirm();

const char END = '0';
const char NOT_ENDING = '1';

int main()
{
	std::cout << "chose command\t"
				<< "[0]:new input "
				<< "[1]:view History "
				<< "[2]:exit "
				<< std::endl;
				
	char chose = '\0';
	int v = 0;

	while( std::cin >> chose ) {
		switch ( chose ) {
			case '0' : 
				std::cout << "Decrease: ";
				v = inpt();
				write_to_disk( v );
				break;
			case '1' :
				return 0;
				break;
			case '2' :
				exit( EXIT_SUCCESS );
			default :
				std::cout << "not in listed" << std::endl;
				continue;
		}
	}
	return 0;
}
/* メモリ内入力
	数値が入力されるまで入力を促す（int:小数点切り捨て）
	EOFで操作の取り消しが可能															*/
int inpt()
{
	int pull = 0;
	std::cin >> pull;
	
	
	if ( !std::cin && !std::cin.eof() ) {
		while( input_again( pull ) );
	}
	return pull;
}

/* cinがfail状態なら再入力を促す

	まずfailフラグをクリアするため、	呼び出し元でfailフラグを確認して呼ぶ。

	1．failビットをクリアし、再度入力を促す。
	2. cinの保持しているバッファを消す(読み飛ばす)。
	3．再度入力を促す。
	4. 再入力も失敗（fail状態）ならtrue(もう一度入力が必要)を返し、
	   good状態ならfalse(入力のやり直し不用)を返す。							*/
bool input_again( int& re_val )
{
	std::cin.clear();
	std::cin.ignore( sizeof re_val, '\n' );
	std::cout << "inputed value is not a number, try again." << std::endl;
	std::cin >> re_val;

	if ( std::cin.fail() ) return true;
	else return false;
}

/* ディスク・ファイルに整数値を書き込む。
	ofstream のローカルオブジェクトのファイル・ストリームの生死は
	この関数のライフタイムに委ねる ｽﾔｧ・・・									*/
void write_to_disk( int w_val )
{
	std::ofstream fs_out( "tmp.txt", std::ios::binary | std::ios::app );

	if ( !fs_out ) {
		std::cerr << "Err: file cannot open\n";
		exit( EXIT_FAILURE );
	}

	if ( write_confirm() == true ) { 
		fs_out << w_val << std::endl;
		std::cout << "success" << std::endl;
		std::cout << "type [number] as command " << std::flush;
	}
	// NOの場合はメッセージだけ表示し、ファイルに書き込まない。
	else {
		std::cout << "your input value does not recorded\n"
			<< "type [number] as command " << std::flush;
		return; 
	}
}
/* 直前の入力を取り消すか確認する。（確認のみ、取り消し操作はしない。）
	y,Y,n,Nのいずれか1文字のみ入力を受け付ける。
	戻り値はYesの場合true, Noの場合false。
	関数を抜ける直前の"return false"は関数式自体がおかしいことを表す。			*/
bool write_confirm()
{
	bool ok = true;
	char ans = '\0';
	std::cout << "write to disk file. are you ok? (y/n) " << std::flush;

	while( std::cin.get( ans ) ) {

		if ( ans == 'y' || ans == 'Y') return ok;
		else if ( ans == 'n' || ans == 'N' ) return !ok;
		else {
			std::cin.unget();
			std::cin.get( ans );
		}
	}
	return false;
}