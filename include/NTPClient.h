#include <WiFiUdp.h>
#include "Logging.h"

time_t getNtpTime();

void updateTime();
void setTimeServer(IPAddress timesvr);

class NtpClient
{
public:
	NtpClient();
	~NtpClient();
};
