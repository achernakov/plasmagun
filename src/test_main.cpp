#include "stdafx.h"
#include "net/socket.h"
#include "infiniium/infiniium_proto.h"
#include "oscope_conn.h"


std::string digitizeCommand (bool ch1, bool ch2, bool ch3, bool ch4) {
	if (!ch1 && !ch2 && !ch3 && !ch4) {
		throw Plasma::Error ("Error: all channels disabled");
	}
	std::string out;
	out = std::string(":DIGITIZE ") + ((ch1)?"CHANNEL1,":"") + ((ch2)?"CHANNEL2,":"") 
		+ ((ch3)?"CHANNEL3,":"") + ((ch4)?"CHANNEL4,":"");
	return out;
}


void writeDataToFile (const std::string & path, const std::vector<int16_t> & data, 
		double yIncr, double yOrigin) {
	std::ofstream file;
	file.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!file.is_open()) {
		throw Plasma::Error ("Can't open data file in write mode");
	}
	for (std::vector<int16_t>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		file << yOrigin + (*iter) * yIncr << ",";
	}	
	file.close();
}

void writeDataToFile (const std::string & path, const std::vector<int16_t> & data,
		double yIncr, double yOrigin, double timeScale) {
	std::ofstream file;
	file.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!file.is_open()) {
		throw Plasma::Error ("Can't open data file in write mode");
	}
	long long unsigned int timeCounter = 0;
	for (std::vector<int16_t>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		timeCounter++;
		file << timeCounter << "," << timeCounter * timeScale << "," << yOrigin + (*iter) * yIncr << std::endl;
	}	
	file.close();
}

void writeDataToFile (const std::string & path, const std::vector<int16_t> & data) {
	std::ofstream file;
	file.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!file.is_open()) {
		throw Plasma::Error ("Can't open data file in write mode");
	}
	for (std::vector<int16_t>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		file << *iter << ",";
	}	
	file.close();
}

void doDataAcquisition ( char ** argv) {
	Plasma::OscopeConn m_oscopeConn;

	m_oscopeConn.connect(argv[1], argv[2]);

	std::stringstream ss1, ss2;
	std::string acqRangeStringStr = "1.00E-6";
	std::string acqSamplingRateStr = "1.00E+10";
	double acqRange, acqSRate;
	ss1 << acqRangeStringStr;
	ss1 >> acqRange;
	ss2 << acqSamplingRateStr;
	ss2 >> acqSRate;
	m_oscopeConn.command("*RST");
	m_oscopeConn.command(":SYSTEM:HEADER OFF");

	m_oscopeConn.command(":ACQUIRE:MODE RTIME");
	m_oscopeConn.command(":ACQUIRE:INTERPOLATE OFF");
	m_oscopeConn.command(":ACQUIRE:AVERAGE OFF");

	m_oscopeConn.command(std::string(":TIMEBASE:RANGE ") + 
			acqRangeStringStr);

	m_oscopeConn.command(":ACQUIRE:POINTS:AUTO ON");
	m_oscopeConn.command(":ACQUIRE:SRATE:AUTO OFF");
	m_oscopeConn.command(std::string(":ACQUIRE:SRATE ") + 
			acqSamplingRateStr);

	m_oscopeConn.command(":WAVEFORM:BYTEORDER LSBFIRST");
	m_oscopeConn.command(":WAVEFORM:FORMAT WORD");
	m_oscopeConn.command(digitizeCommand(false, false, true, true));



	std::vector<int16_t> data;


	for (size_t chanNo = 1; chanNo <= 4; chanNo++) {
		std::stringstream ss;
		ss << chanNo;
		std::string no, chSelector(":WAVEFORM:SOURCE CHANNEL"), 
			fileSelector("chan"), fileExtension(".csv");
		ss >> no;

		if (chanNo > 2) {
			double yIncr, yOrigin, timeScale;
			m_oscopeConn.command((chSelector + no).c_str());
			m_oscopeConn.command(":WAVEFORM:DATA?");
			m_oscopeConn.getWordData(data);
			data.erase(data.begin()+(int)(acqRange * acqSRate), data.end());
			m_oscopeConn.getTypedParam(":WAVEFORM:YINCREMENT?", yIncr);
			m_oscopeConn.getTypedParam(":WAVEFORM:YORIGIN?", yOrigin);
			m_oscopeConn.getTypedParam(":WAVEFORM:XINCREMENT?", timeScale);
			writeDataToFile(fileSelector + no + fileExtension, data, yIncr, yOrigin, timeScale);
		}
	}

	double yIncr;
	m_oscopeConn.getTypedParam<double>(":WAVEFORM:YINCREMENT?", yIncr);
	//		g_print("Y_INCREMENT: %lf\n", yIncr);

	//		g_print ("[WORD MODE] Got data: %i values, printing:", data.size());
	/*		for (std::vector<int16_t>::iterator iter = data.begin(); iter != data.end(); iter++) {
			g_print("%i, ", *iter);
			}

			g_print("\n");*/
	m_oscopeConn.command(":CHANNEL3:DISPLAY ON");
	m_oscopeConn.command(":CHANNEL4:DISPLAY ON");
	m_oscopeConn.disconnect();
}		

int main (int argc, char ** argv) {
	if (argc < 3) {
		std::cout << "Too few args. Usage : test_plasmagun [IP] [port]" << std::endl;
		return 1;
	} 
	doDataAcquisition(argv);
	return 0;
}

