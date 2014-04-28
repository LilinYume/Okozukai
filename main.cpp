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
				// ?		�����������������@�\������񂾂���(�L�E�ցE`)
				break;
			default :
				std::cout << "not in list\n";
				continue;
		}
	}
	return 0;
}

/* �o��̓���
	�o��̓��͂𑣂��B
	���͒l�͐����l�̂ݎ󂯕t����iint�j�B
	��̃t�@�C���i�V�K�쐬�t�@�C���j�̏ꍇ�A��ɒ����c���̓��͂𑣂��B
	EOF�ő�����L�����Z���B���̐��͐��̐��ɕ�����ւ���B
	���͂��ꂽ�o���\�������l��Ԃ��B												*/
int inpt()
{
	// �t�@�C���̒��g���󂩃`�F�b�N
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

	// ���͒l���s��
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
/* �t�@�C���̒��g���󂩒��ׂ�
	�t�@�C�����͍����̌����𗘗p���邽�߁A
	�V�������̏ꍇ�ŏ��ɒ����c������͂�����̂Ɏg���B
	�t�@�C���擪����1�����ǂݎ��A������EOF�i-1�j�Ȃ�true��Ԃ��B			*/
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

/* �����A�����߂̒����c������͂�����
	��̃t�@�C���i�V�K�쐬�j�̏ꍇ�̂ݓ��́B
	�t�@�C�����ɍ����̌����𗘗p���邽�߁A�V�K�쐬�����̂͌������߂��ꍇ�ɂȂ�B
	���͂͐����l�B																				*/
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

/* �f�B�X�N�E�t�@�C���֏�������
	���͂��ꂽ�o��A�����߂̒����c���Ȃǂ������ɂƂ�A
	�����̒l�Ɠ��͎��̓������������ށB
	�������ޑO�ɏ������݂��邩�m�F������B
	�������ޒl�� �����i�Œ蒷�j �̂��� �����̒l�i�ϒ��j�A�K�X�X�y�[�X�ŋ�؂�B
	�������݊m�F��No�̏ꍇ�ƁA�����̐����l���[���̏ꍇ�������܂Ȃ��B
	���̃v���O�����̖ړI�ł͏o��[�����������ވӖ��������B								*/
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

/* �������݊m�F
	�f�B�X�N�E�t�@�C���ɏ������ޑO�ɖ{���ɏ������ނ��m�F����B							*/
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
/* �������݂����ꂽ�t�@�C�����R���\�[����ɕ\������B
	�ǂݍ��ނ̂͏�ɁA�����̃t�@�C���i�t�@�C�����͍����̌����j�B					*/
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