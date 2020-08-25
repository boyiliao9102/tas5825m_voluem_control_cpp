/* tas5825m_main_control.cpp */

#include "tas5825m_control.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>



using namespace tas5825m_control;
using namespace std;

int main(int argc, char **argv)
{
	string send_set_cmd = "i2cset -f -y 4 ";
	std::ostringstream int_string;
	std::ostringstream out_send_cmd_set;
	string final_set_cmd = "";

	stringstream tempstringstream;
	string vol_out;
	string i2c_get_ret;
	bool check_ret = false;
	int vol = 0;
	int get_vol_dec;
	float db_value=24.0;
	int i ;
	tas5825m_class tas5825m_crtl;


	/* check input parameters */
	if(argc < 2) {
		cout << "too few parameters" << endl;
		return -1;
	}

	if(argv[1][0] == tas5825m_crtl.I2C_SET ) { /* 0x73 is 's' i2cset */

		cout << "set value " << endl;
		if(argc < 3) {
			cout << "Range 0 - 255" << endl;
			return -1;
		}
		vol = atoi(argv[2]);

	} else if (argv[1][0]== tas5825m_crtl.I2C_GET ) { /* 0x67 is 'g'  i2cget */

		cout << "get value " << endl;
		if((argc >= 3)) {
			cout << "too many parameters !" << endl;
			return -1;
		}

	} else {
		cout << "parameter error !" << endl;
		return -1;
	}

#if 1
	cout << "argc=" << argc << endl;
	cout << "argv[0]" << argv[0] << endl;
	cout << "argv[1]" << argv[1] << endl;
	if((argc >= 3))
		cout << "argv[2]" << argv[2] << endl;
#endif
	/* check input parameters */


	/*
	Digital Volume

	These bits control both left and right channel digital volume. The

	digital volume is 24 dB to -103 dB in -0.5 dB step.

	00000000: +24.0 dB
	00000001: +23.5 dB
	........
	and 00101111: +0.5 dB
	00110000: 0.0 dB
	00110001: -0.5 dB
	.......
	11111110: -103 dB
	11111111: Mute

	i2cset -f -y 4 0x4C 0x4C 0xXX

	*/

	vol = tas5825m_crtl.get_volume_index(vol);
	cout << ">> input vol = " << vol << endl;

	check_ret = tas5825m_crtl.i2c_tool_check(I2C_TOOL_DETECT);
	if(check_ret == false) {
		/*i2c tools not exist  can't access i2c device*/
		return -1;
	}

	//tas5825m_crtl.debug_msg("debug message test!\n");

	/* 0x73 is 's'  i2cset */
	if(argv[1][0] == tas5825m_crtl.I2C_SET) {
		/* sprintf( send,"i2cset -f -y 4 %s 0x4C 0x%02x",TAS5825M_I2C_SLAVE_ADDRESS,vol);  //'C'version  */

		/*  input volume  Dec to Hex */
		tempstringstream << hex << vol;
		/*   trans to string */
		tempstringstream >> vol_out;
		//cout << "vol_out" << vol_out << endl;

		/* combine  i2c command */
		out_send_cmd_set  <<	send_set_cmd << TAS5825M_I2C_SLAVE_ADDRESS <<  " 0x4C " << "0x" << vol_out ;
		final_set_cmd = out_send_cmd_set.str();

		//cout << "final_set_cmd = " << final_set_cmd << endl;
		tas5825m_crtl.i2c_set(final_set_cmd);

		/* 0x67 is 'g'  i2cget */
	} else if(argv[1][0] == tas5825m_crtl.I2C_GET) {

		i2c_get_ret = tas5825m_crtl.i2c_get();
		//cout << "i2c get result = " << i2c_get_ret << endl;
		get_vol_dec = (uint32_t)strtol(i2c_get_ret.c_str(), NULL, 16);
		//cout << "get volume  = " << get_vol_dec << endl;

		if(get_vol_dec == 0xff) {
			cout << "Mute" << endl;
			return 0;
		}

		for(i = 0; i <= 255 ; i ++) {
			if(i == get_vol_dec)
				break;
			db_value = db_value - 0.5;
		}
		printf("volume = %d (0x%02x)\n",get_vol_dec,get_vol_dec);
		printf("db value = (%2.1f)db\n",db_value);


	}


	return 0;
}
