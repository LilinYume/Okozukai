#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <iostream>
#include <string>
#include <ctime>

namespace time_stamp
{
	const int day_buf = 9;
	const int time_buf = 7;

	class Date
	{
	private:
		std::string month_lable;
		std::string week_lable;
		char day[ day_buf ];
		char time[ time_buf ];
	public:
		Date()
		{
			month_lable = "";
			week_lable = "";

			for( int i = 0; i < sizeof day; ++i ) {
				day[ i ] = '\0';
			}
			for( int i = 0; i < sizeof time; ++i ) {
				time[ i ] = '\0';
			}
		}
		Date( const Date& l_ob )
		{
			this-> month_lable = l_ob.month_lable;
			this-> week_lable = l_ob.week_lable;

			for( int i = 0; i < sizeof day; ++i ) {
				this-> day[ i ] = l_ob.day[ i ];
			}
			for( int i = 0; i < sizeof time; ++i ) {
				this-> time[ i ] = l_ob.time[ i ];
			}

		}
	private: 
		void set_month_lable()
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			int current_month = today.tm_mon;

			std::string tmp[][12] = 
								{
									{ "Jan" },
									{ "Feb" },
									{ "Mar" },
									{ "Apr" },
									{ "May" },
									{ "Jun" },
									{ "Jul" },
									{ "Aug" },
									{ "Sep" },
									{ "Oct" },
									{ "Nov" },
									{ "Dec" },
								};
			month_lable = *tmp[ current_month ];
		}
		void set_week_lable() 
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			int current_week = today.tm_wday;
			
			std::string tmp[][7] = 
									{
										{ "Sun" },
										{ "Mon" },
										{ "Tue" },
										{ "Wed" },
										{ "Thu" },
										{ "Fri" },
										{ "Sat" },
									};

			week_lable = *tmp[ current_week ];
		}
		void set_day_value() 
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );
			std::strftime( day, sizeof day, "%Y%m%d", &today );
		}
		void set_time_value() 
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );
			std::strftime( time, sizeof time, "%H%M%S", &today );
		}
	public:
		const char* get_month_lable() 
		{
			set_month_lable();
			return month_lable.c_str();
		}
		const char* get_week_lable()
		{
			set_week_lable();
			return week_lable.c_str(); 
		}
		const char* get_day() 
		{
			set_day_value();
			return day;
		}
		const char* get_time()
		{
			set_time_value();
			return time;
		}
	};
}
#endif

