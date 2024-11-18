/*
  Position.h 
*/


#ifndef Position_h
#define Position_h

#include <Arduino.h>

namespace position {

class Position
{
	
  public:
    Position();
	void setLatitude(String latitude);
	void setLongitude(String latitude);
	String getJSONString(void);
	String getLatitude();
	String getLongitude();

  private:
    String latitude;
	String longitude;
};

}

#endif
