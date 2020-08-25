#include <string>
#include <iostream>

/* tas5825m control header file*/

#define TAS5825M_DIG_VOLUME_MAX  (255)
#define TAS5825M_DIG_VOLUME_MIN  (0)

#define TAS5825M_DEBUG 1
#define TAS5825M_I2C_SLAVE_ADDRESS "0x4C"
#define I2C_TOOL_DETECT "/system/bin/i2cdetect"


using namespace std;

namespace tas5825m_control
{
class tas5825m_class
{

private:
	int i2c_send_set_cmd(string shell_cmd);
	string i2c_send_get_cmd(string shell_cmd);

public:

	const int I2C_SET  = 0x73;
	const int I2C_GET  = 0x67;

	void debug_msg(std::string debug_msg);
	int get_volume_index(int vol);
	bool  i2c_tool_check(const std::string& path);

	int i2c_set(std::string send_shell_cmd);
	std::string i2c_get(void);


};

}
