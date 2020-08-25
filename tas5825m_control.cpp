
/*tas5825m_control.cpp */

#include <iostream> // header in standard library
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "tas5825m_control.h" // header in local directory




using namespace tas5825m_control;
using namespace std;

void tas5825m_class::debug_msg(string debug_msg)
{
	cout << "tas5825m> "<< debug_msg << endl;
}

int tas5825m_class::i2c_set( string send_shell_cmd)
{

#if TAS5825M_DEBUG
	cout << "i2c_set send_cmd = " << send_shell_cmd << endl;
#endif

	tas5825m_class::i2c_send_set_cmd(send_shell_cmd);

	return 0;
}

string tas5825m_class::i2c_get()
{
	string ret = "return_error";
	int string_length;
	string send_get_cmd = "i2cget -f -y 4 ";
	string final_get_cmd = "";
	std::ostringstream out_send_cmd_get;

#if TAS5825M_DEBUG
	cout << "i2c_get " << endl;
#endif

	out_send_cmd_get << send_get_cmd <<  TAS5825M_I2C_SLAVE_ADDRESS <<  " 0x4C " ;
	final_get_cmd = out_send_cmd_get.str();

	ret = tas5825m_class::i2c_send_get_cmd(final_get_cmd);

	return ret;
}


int tas5825m_class::i2c_send_set_cmd( string shell_cmd)
{
	char output_shell[64];

#if 0
	cout << "send_cmd = " << shell_cmd << endl;
#endif
	strcpy(output_shell,shell_cmd.c_str());
	system(output_shell);

	return 0;
}

string  tas5825m_class::i2c_send_get_cmd( string shell_cmd)
{
	string err_string  = "get_i2c_error";
	string ret_string = "";
	char output_shell[64];
	FILE *fp;
	char ret_buf[8];
	std::stringstream temp_ss;

#if 1
	cout << "i2c_send_get_cmd  = " << shell_cmd << endl;
#endif


	strcpy(output_shell,shell_cmd.c_str());

	if ((fp = popen(output_shell,"r")) == NULL) {
		cout << "popen() error!" << endl;
		return err_string;
	}

	while (fgets(ret_buf, sizeof(ret_buf), fp)) {
		//printf(">>%s\n", buf);
	}

	pclose(fp);

	/* return char buffer to stringstream */
	temp_ss << ret_buf;
	/* stringstream to string */
	temp_ss >> ret_string;

	return ret_string;

}

int tas5825m_class::get_volume_index(int vol)
{
	int index;

	index = vol;

	if (index < TAS5825M_DIG_VOLUME_MIN)
		index = TAS5825M_DIG_VOLUME_MIN;

	if (index > TAS5825M_DIG_VOLUME_MAX)
		index = TAS5825M_DIG_VOLUME_MAX;

	return index;
}

bool tas5825m_class::i2c_tool_check(const std::string& path)
{
	bool ret = false ;
	FILE *fp;

#if 0
	cout << "path =" << path.c_str() <<endl;
#endif

	if((access(path.c_str(), F_OK)) != -1) {
		ret = true;
		//cout << ">> i2c tools exist" << endl;
		return ret;
	}
	ret =  false;
	cout << ">> i2c tools not exist" << endl;

	return ret;
}
