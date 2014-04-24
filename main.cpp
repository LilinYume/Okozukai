#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <string>
#include <cctype>
#include "time_stamp.h"

int inpt();
bool input_again( int& v );
void write_to_disk( int );
bool write_confirm();
void read_month_file();

int main()
{
	std::cout << "[0]:new input "
				<< "[1]:view History "
				<< "[2]:exit\n"
				<< "chose command > ";
				
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
				read_month_file();
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
/* ������������
	���l�����͂����܂œ��͂𑣂��iint:�����_�؂�̂āj
	EOF�ő���̎��������\															*/
int inpt()
{
	int pull = 0;
	std::cin >> pull;

	if ( !std::cin && !std::cin.eof() ) {
		while( input_again( pull ) );
	}
	return pull;
}
/* cin��fail��ԂȂ�ē��͂𑣂�
	
	�܂�fail�t���O���N���A���邽�߁A	�Ăяo������fail�t���O���m�F���ČĂԁB
	1�Dfail�r�b�g���N���A���A�ēx���͂𑣂��B
	2. cin�̕ێ����Ă���o�b�t�@������(�ǂݔ�΂�)�B
	3�D�ēx���͂𑣂��B
	4. �ē��͂����s�ifail��ԁj�Ȃ�true(������x���͂��K�v)��Ԃ��A
	   good��ԂȂ�false(���͂̂�蒼���s�p)��Ԃ��B									*/
bool input_again( int& re_val )
{
	std::cin.clear();
	std::cin.ignore( sizeof re_val, '\n' );
	std::cout << "inputed value is not a number, try again." << std::endl;
	std::cin >> re_val;

	if ( std::cin.fail() ) return true;
	else return false;
}

/* �f�B�X�N�E�t�@�C���ɐ����l���������ށB
	ofstream �̃��[�J���I�u�W�F�N�g�̃t�@�C���E�X�g���[���̐�����
	���̊֐��̃��C�t�^�C���Ɉς˂� �ԧ�E�E�E										*/
void write_to_disk( int w_val )
{
	using time_stamp::Date;
	Date stamp;

	const char* file_name = stamp.get_month_lable();

	// �����̌������t�@�C�����ɂ���	
	std::ofstream fs_out( file_name, std::ios::binary | std::ios::app );

	if ( !fs_out ) {
		std::cerr << "Err: file cannot open\n";
		exit( EXIT_FAILURE );
	}
	if ( write_confirm() == true ) { 
		fs_out << w_val << ' ' << stamp.get_day() <<  ' ' << stamp.get_time() <<  std::endl;
		std::cout << "success" << std::endl;
		std::cout << "[number] as command > " << std::flush;
	}
	// NO�̏ꍇ�̓��b�Z�[�W�����\�����A�t�@�C���ɏ������܂Ȃ��B
	else {
		std::cout << "your input value does not recorded\n"
			<< "[number] as command > " << std::flush;
		return;
	}
}
/* ���O�̓��͂����������m�F����B�i�m�F�̂݁A����������͂��Ȃ��B�j
	y,Y,n,N�̂����ꂩ1�����̂ݓ��͂��󂯕t����B
	�߂�l��Yes�̏ꍇtrue, No�̏ꍇfalse�B
	�֐��𔲂��钼�O��"return false"�͊֐������̂������������Ƃ�\���B				*/
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
void read_month_file()
{
	std::string costs, day, time;
	using time_stamp::Date;
	Date stamp;

	const char* file_name = stamp.get_month_lable();
	costs = day = time = "";

	std::ifstream fs_in( file_name, std::ios::binary );
	
	if ( !fs_in ) {
		std::cerr << "File cannot open\n";
		exit( EXIT_FAILURE );
	}
	while( !fs_in.eof() ) {
		fs_in >> costs >> day >> time;
		std::cout << costs << ' ' << day << std::endl;
	}
	std::cout << "[number] as command > " << std::flush;
}