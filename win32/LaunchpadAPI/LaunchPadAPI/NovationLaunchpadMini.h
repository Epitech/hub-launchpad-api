
#ifndef __LAUNCHPAD_H__
#define __LAUNCHPAD_H__

#include "LaunchPadAPI.h"

class NovationLaunchpadMini : public ILaunchPadDevice
{
private:
	RtMidiOut								m_out;
	RtMidiIn								m_in;

	std::function<void(LaunchPadEvent)>		m_eventCallback;

public:
	NovationLaunchpadMini();
	virtual ~NovationLaunchpadMini();

	/**
	* getPortsIn: return available MIDI compatible devices portid and names (in)
	* @return The port names sorted by IDs
	*/
	std::vector<std::string const> getPortsIn();

	/**
	* getPortsOut: return available MIDI compatible devices portid and names (out)
	* @return The port names sorted by IDs
	*/
	std::vector<std::string const> getPortsOut();

	/**
	* connect: connect to the specified port
	* @param unsigned int the device port id from the list
	* @param unsigned int the device port id from the list
	* @return true if the connections are both successfull
	*/
	virtual bool connect(unsigned int portidIn, unsigned int portidOut);

	/**
	* findAndConnect: automatically find the best device matching a string and connect to it
	* @return true if the connections are both successfull
	*/
	virtual bool findAndConnect();
	
	/**
	* reset: reset the launchpad state
	* @return void
	*/
	virtual void reset();

	/**
	* setKeyColor: turn on/off the defined key
	* @param unsigned char X coordinate of the board
	* @param unsigned char Y coordinate of the board
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setKeyColor(unsigned char x, unsigned char y, KeyStyle &style);

	/**
	* setKeyColor: turn on/off the defined key 
	* @param LP_KEY the key to be activated, ranging from 0 to 64
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setKeyColor(LP_KEY, KeyStyle &);

	/**
	* setLineColor: turn on/off the defined line
	* @param LP_LINE the line to be activated, ranging from 0 to 7
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setLineColor(LP_LINE, KeyStyle &);

	/**
	* setColumnColor: turn on/off the defined column
	* @param LP_COLUMN the line to be activated, ranging from 0 to 7
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setColumnColor(LP_COLUMN, KeyStyle &);

	/**
	* setControllerColor: turn on/off the defined control key
	* @param LP_CONTROLLER the line to be activated, ranging from 0 to 15
	* @param KeyStyle the style/color to be applied to the key
	* @return void
	*/
	virtual void setControllerColor(LP_CONTROLLER, KeyStyle &);

	/**
	* setEventCallback: sets the callback to be used in case of an event from the launchpad
	* @param std::function<void(LaunchPadEvent)> the callback
	* @return void
	*/
	virtual void setEventCallback(std::function<void(LaunchPadEvent)>);

	/**
	* parseCallback: parse the received MIDI message and transform it into an event struct
	* @param double delay from the last message received
	* @param std::vector< unsigned char > the message
	* @return void
	*/
	void parseCallback(double deltatime, std::vector< unsigned char > *message);

	/**
	* onLaunchPadCallback: linked static callback sent back to the handler
	* @param double delay from the last message received
	* @param std::vector< unsigned char > the message
	* @param void* associated object
	* @return void
	*/
	static void onLaunchPadCallback(double deltatime, std::vector< unsigned char > *message, void *);

};

#endif // !__LAUNCHPAD_H__
