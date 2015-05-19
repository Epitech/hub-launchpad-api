
#ifndef __ILAUNCHPADDEVICE_H__
#define __ILAUNCHPADDEVICE_H__

#include "LaunchPadAPI.h"

class ILaunchPadDevice
{
public:
	virtual ~ILaunchPadDevice() { };

	/**
	* connect: connect to the specified port
	* @param unsigned int the device port id from the list
	* @param unsigned int the device port id from the list
	* @return true if the connections are both successfull
	*/
	virtual bool connect(unsigned int portidIn, unsigned int portidOut) = 0;

	/**
	* findAndConnect: automatically find the best device matching a string and connect to it
	* @return true if the connections are both successfull
	*/
	virtual bool findAndConnect() = 0;

	/**
	* reset: reset the launchpad state
	* @return void
	*/
	virtual void reset() = 0;

	/**
	* setKeyColor: turn on/off the defined key
	* @param unsigned char X coordinate of the board
	* @param unsigned char Y coordinate of the board
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setKeyColor(unsigned char x, unsigned char y, KeyStyle &style) = 0;

	/**
	* setKeyColor: turn on/off the defined key
	* @param LP_KEY the key to be activated, ranging from 0 to 64
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setKeyColor(LP_KEY, KeyStyle &) = 0;

	/**
	* setLineColor: turn on/off the defined line
	* @param LP_LINE the line to be activated, ranging from 0 to 7
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setLineColor(LP_LINE, KeyStyle &) = 0;

	/**
	* setColumnColor: turn on/off the defined column
	* @param LP_COLUMN the line to be activated, ranging from 0 to 7
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setColumnColor(LP_COLUMN, KeyStyle &) = 0;

	/**
	* setControllerColor: turn on/off the defined control key
	* @param LP_CONTROLLER the line to be activated, ranging from 0 to 15
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setControllerColor(LP_CONTROLLER, KeyStyle &) = 0;

	/**
	* setEventCallback: sets the callback to be used in case of an event from the launchpad
	* @param std::function<void(LaunchPadEvent)> the callback
	* @return void
	*/
	virtual void setEventCallback(std::function<void(LaunchPadEvent)>) = 0;
};


#endif
