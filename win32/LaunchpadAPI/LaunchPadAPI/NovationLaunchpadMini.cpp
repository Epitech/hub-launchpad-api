
#include "LaunchpadAPI.h"

NovationLaunchpadMini::NovationLaunchpadMini()
: ILaunchPadDevice()
{

}

NovationLaunchpadMini::~NovationLaunchpadMini()
{

}

std::vector<std::string const> NovationLaunchpadMini::getPortsIn()
{
	unsigned int i = 0;
	unsigned int nPorts = m_in.getPortCount();
	std::vector<std::string const> res;
	std::string portname;

	for (i = 0; i < nPorts; i++) 
	{
		portname = m_in.getPortName(i);
		res.push_back(portname);
	}
	return (res);
}

std::vector<std::string const> NovationLaunchpadMini::getPortsOut()
{
	unsigned int i = 0;
	unsigned int nPorts = m_out.getPortCount();
	std::vector<std::string const> res;
	std::string portname;

	for (i = 0; i < nPorts; i++)
	{
		portname = m_out.getPortName(i);
		res.push_back(portname);
	}
	return (res);
}

bool NovationLaunchpadMini::connect(unsigned int portidIn, unsigned int portidOut)
{
	try 
	{
		m_in.openPort(portidIn);
		m_out.openPort(portidOut);

		m_in.setCallback(&NovationLaunchpadMini::onLaunchPadCallback, this);

		// Don't ignore sysex, timing, or active sensing messages.
		m_in.ignoreTypes(false, false, false);

		reset();

		return (true);
	}
	catch (RtMidiError &error) 
	{
		std::cerr << error.getMessage() << std::endl;
		return (false);
	}
}

bool NovationLaunchpadMini::findAndConnect()
{
	std::vector<std::string const> ptIn = getPortsIn();
	std::vector<std::string const> ptOut = getPortsOut();
	int idIn = -1;
	int idOut = -1;
	unsigned i = 0;

	std::for_each(ptIn.begin(), ptIn.end(), [&idIn, &i](std::string const str) {
		if (str.find("Launchpad") == 0)
			idIn = i;
		i++;
	});
	i = 0;
	std::for_each(ptOut.begin(), ptOut.end(), [&idOut, &i](std::string const str) {
		if (str.find("Launchpad") == 0)
			idOut = i;
		i++;
	});

	if (idIn != -1 && idOut != -1)
	{
		return (connect(idIn, idOut));
	}
	return (false);
}

void NovationLaunchpadMini::reset()
{
	// Send Reset
	std::vector<unsigned char> msgout;
	
	msgout.push_back(176);
	msgout.push_back(0);
	msgout.push_back(0);
	m_out.sendMessage(&msgout);
}

void NovationLaunchpadMini::setKeyColor(unsigned char x, unsigned char y, KeyStyle &style)
{
	setKeyColor(XYKEY_TO_W(x, y), style);
}

void NovationLaunchpadMini::setKeyColor(LP_KEY key, KeyStyle &style)
{
	std::vector<unsigned char> msgout;
	unsigned char stl = style.concatToChar();

	/*key = CLR_LINE_CTRL_SIDE(key);
	key = LINE_APPLY_OFFSET(key);*/
	key = KEY_TO_LINE(key) * (LINE_INCR_OFFSET + LINE_LENGTH) + KEY_TO_COLUMN(key);
	msgout.push_back(144);
	msgout.push_back(key);
	msgout.push_back(stl);
	m_out.sendMessage(&msgout);
}

void NovationLaunchpadMini::setLineColor(LP_LINE ln, KeyStyle &style)
{
	std::vector<unsigned char> msgout;

	for (unsigned int i = 0; i < COL_LENGTH; i++)
	{
		unsigned char key = ln * (LINE_INCR_OFFSET + LINE_LENGTH) + i;

		msgout.clear();
		msgout.push_back(144);
		msgout.push_back(key);
		msgout.push_back(style.concatToChar());
		m_out.sendMessage(&msgout);
	}
}

void NovationLaunchpadMini::setColumnColor(LP_COLUMN col, KeyStyle &style)
{
	std::vector<unsigned char> msgout;

	for (unsigned int i = 0; i < LINE_LENGTH; i++)
	{
		unsigned char key = i * (LINE_INCR_OFFSET + LINE_LENGTH) + col;

		msgout.clear();
		msgout.push_back(144);
		msgout.push_back(key);
		msgout.push_back(style.concatToChar());
		m_out.sendMessage(&msgout);
	}
}

void NovationLaunchpadMini::setControllerColor(LP_CONTROLLER control, KeyStyle &style)
{
	std::vector<unsigned char> msgout;

	if (control > 7)
	{
		control = (control - CTRL_TOP_SIZE + 1) * LINE_LENGTH;
		control = LINE_APPLY_OFFSET(control);
		control -= 1;
		msgout.push_back(144);
		msgout.push_back(control);
		msgout.push_back(style.concatToChar());
		m_out.sendMessage(&msgout);
	}
	else if (control < 16)
	{
		msgout.push_back(176);
		msgout.push_back(104 + control);
		msgout.push_back(style.concatToChar());
		m_out.sendMessage(&msgout);
	}
}

void NovationLaunchpadMini::setEventCallback(std::function<void(LaunchPadEvent)> fnc)
{
	m_eventCallback = fnc;
}

void NovationLaunchpadMini::parseCallback(double deltatime, std::vector< unsigned char > *message)
{
	unsigned int nBytes = message->size();
	LaunchPadEvent ev;

	if (nBytes != 3)
		return ;
	ev.time = deltatime;
	ev.id = message->at(0);
	ev.key = message->at(1);
	ev.state = message->at(2);
	ev.line = 0;
	ev.col = 0;

	if (ev.id == EVENT_KEY_CHANGED)
	{
		ev.line = RAW_TO_LINE(ev.key);
		ev.col = RAW_TO_COLUMN(ev.key);
		if (ev.col == 8) {
			ev.key = RAW_TO_CTRL(ev.key);
			ev.id = EVENT_CTRL_CHANGED;
		}
		else {
			ev.key = ev.line * LINE_LENGTH + ev.col;
		}
	}
	else if (ev.id == EVENT_CTRL_CHANGED)
	{
		ev.key = ev.key - 104;
	}

	if (m_eventCallback)
		m_eventCallback(ev);
}

void NovationLaunchpadMini::onLaunchPadCallback(double deltatime, std::vector< unsigned char > *message, void *rawMe)
{
	NovationLaunchpadMini* me = (NovationLaunchpadMini*)rawMe;

	me->parseCallback(deltatime, message);
}