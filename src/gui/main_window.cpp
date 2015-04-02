#include "../stdafx.h"
#include "../net/socket.h"
#include "../infiniium/infiniium_proto.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath, true), m_oscopeConn(*this) {
		connectSignals();
		//assignTabs();
	}
	
	MainWindow::~MainWindow () {
		if (m_oscopeConn.status() == Plasma::Connection::CONNECTED) {
			m_oscopeConn.disconnect();
		}
	}

	void MainWindow::connectSignal (const std::string & wdg, 
			const std::string & signal, Handler fun) {
		registerEventHandler(GTK_WIDGET(this->operator[](wdg.c_str())), signal.c_str(),
				WINDOW_CALLBACK(fun), NULL);
	}
	
	void MainWindow::connectSignals () {
		connectSignal ("main_window",  "delete-event", &MainWindow::on_delete_event);
		connectSignal ("request_button", "clicked", &MainWindow::on_connect_clicked);
		//connectSignal ("csv_export_button", "clicked", &MainWindow::on_csv_export_button);
		connectSignal ("start_analysis_button", "clicked", &MainWindow::on_start_analysis_clicked);
	}
	
	//HELPERS

	void MainWindow::appendText (GtkTextBuffer * buf, const std::string & text) {
		GtkTextIter iter;
		gtk_text_buffer_get_end_iter (buf, &iter);
		gtk_text_buffer_insert (buf, &iter, text.c_str(), -1);
	}
	
	
	//==========================================
	//============EVENT HANDLERS================
	//==========================================

	gboolean MainWindow::on_delete_event (GtkWidget *widget,
			GdkEvent  *event,
			gpointer   data)  {
		g_print ("delete event occurred\n");
		return FALSE;
	}

	gboolean MainWindow::on_connect_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		
		if (m_oscopeConn.status() == Plasma::Connection::DISCONNECTED) {
			m_oscopeConn.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
					gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		}
		
		std::string cmd = std::string( gtk_entry_get_text (GTK_ENTRY( operator[]("command_entry" ))));
		Plasma::InfiniiumProtocol::CommType type = Plasma::InfiniiumProtocol::getCommType (cmd);
		
		
		//cmd += "\r\n\r\n";
		//gtk_entry_set_text (GTK_ENTRY(operator[]("command_entry")), "");
		
		
		//m_oscopeConn.sendString(cmd.c_str());
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), cmd+"\n\n");

		if (type == Plasma::InfiniiumProtocol::STR_REQUEST) {
			std::string resp;
			//m_oscopeConn.recvString(resp);
			m_oscopeConn.stringRequest(cmd, resp);
			g_print(resp.c_str());
			appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), resp);
		} else {
			m_oscopeConn.command(cmd);
		}
	}
	
	std::string MainWindow::digitizeCommand (bool ch1, bool ch2, bool ch3, bool ch4) {
		if (!ch1 && !ch2 && !ch3 && !ch4) {
			throw Plasma::Error ("Error: all channels disabled");
		}
		std::string out;
		out = std::string(":DIGITIZE ") + ((ch1)?"CHANNEL1,":"") + ((ch2)?"CHANNEL2,":"") 
				+ ((ch3)?"CHANNEL3,":"") + ((ch4)?"CHANNEL4,":"");
		g_print ("%s\n", out.c_str());
		return out;
	}
	
	bool MainWindow::getCbState (const std::string & cbId) {
		return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(operator[](cbId)));
	}


	void MainWindow::doDataAcquisition () {\
		std::stringstream ss1, ss2;
		std::string acqRangeStringStr = gtk_entry_get_text(GTK_ENTRY(operator[]("acquisition_range_entry")));
		std::string acqSamplingRateStr = gtk_entry_get_text(GTK_ENTRY(operator[]("sampling_rate_entry")));
		double acqRange, acqSRate;
		ss1 << acqRangeStringStr;
		ss1 >> acqRange;
		g_print ("ACQUIRE RANGE: %f\n", acqRange);
		ss2 << acqSamplingRateStr;
		ss2 >> acqSRate;
		g_print ("ACQUIRE SRATE: %f\n", acqSRate);
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
		m_oscopeConn.command(digitizeCommand(getCbState("cb_chan1"),getCbState("cb_chan2"),
				getCbState("cb_chan3"),getCbState("cb_chan4")));



		std::vector<int16_t> data;


		for (size_t chanNo = 1; chanNo <= 4; chanNo++) {
			std::stringstream ss;
			ss << chanNo;
			std::string no, chSelector(":WAVEFORM:SOURCE CHANNEL"), 
					fileSelector("/home/user/PlasmaGun/chan"), fileExtension(".csv");
			ss >> no;
			
			if (getCbState("cb_chan" + no)) {
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
		g_print("Y_INCREMENT: %lf\n", yIncr);
		
		g_print ("[WORD MODE] Got data: %i values, printing:", data.size());
		for (std::vector<int16_t>::iterator iter = data.begin(); iter != data.end(); iter++) {
			g_print("%i, ", *iter);
		}
		
		g_print("\n");
		
	}

	gboolean MainWindow::on_start_analysis_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   unused_data) {
		if (m_oscopeConn.status() == Plasma::Connection::DISCONNECTED) {
			m_oscopeConn.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
					gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		}

		try {
			doDataAcquisition();
		} catch (const Plasma::Error & ex) {
			GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
			GtkMessageDialog * dialog = GTK_MESSAGE_DIALOG (gtk_message_dialog_new (GTK_WINDOW(operator[]("main_window")),
					flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Error occurred: %s", ex.what()));
			gtk_dialog_run (GTK_DIALOG(dialog));
			gtk_widget_destroy(GTK_WIDGET(dialog));
			
			m_oscopeConn.disconnect();
		}
	}

	
	gboolean MainWindow::on_csv_export_button (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		
		if (m_oscopeConn.status() == Plasma::Connection::DISCONNECTED) {
			m_oscopeConn.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
					gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		}

		std::ofstream dataFile;
		std::string resp, splitResp;
		std::string cmd = ":WAVEFORM:DATA?";
		
		
		//m_oscopeConn.command(":ACQUIRE:MODE RTIME");
		//m_oscopeConn.command(":ACQUIRE:POINTS:AUTO OFF");
		//m_oscopeConn.command(":ACQUIRE:POINTS:ANALOG 500");


		//*RST

		m_oscopeConn.command("*RST");
		m_oscopeConn.command(":SYSTEM:HEADER OFF");

		m_oscopeConn.command(":ACQUIRE:MODE RTIME");
		m_oscopeConn.command(":ACQUIRE:INTERPOLATE OFF");
		m_oscopeConn.command(":ACQUIRE:AVERAGE OFF");

		m_oscopeConn.command(std::string(":TIMEBASE:RANGE ") + gtk_entry_get_text(GTK_ENTRY(operator[]("acquisition_range_entry"))));

		m_oscopeConn.command(":ACQUIRE:POINTS:AUTO ON");
		m_oscopeConn.command(":ACQUIRE:SRATE:AUTO OFF");
		m_oscopeConn.command(std::string(":ACQUIRE:SRATE ") + gtk_entry_get_text(GTK_ENTRY(operator[]("sampling_rate_entry"))));
		m_oscopeConn.command(":WAVEFORM:FORMAT ASCII");
		m_oscopeConn.command(":DIGITIZE CHANNEL3,CHANNEL4");
		//m_oscopeConn.command(":ACQUIRE:POINTS 500");

		//CHANNEL4
		m_oscopeConn.command(":WAVEFORM:SOURCE CHANNEL4");		
		//m_oscopeConn.command(":DIGITIZE CHANNEL4");
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), cmd+"\n Written to file\n");
		m_oscopeConn.command(cmd);
		m_oscopeConn.getStrData(resp);
		g_print(resp.c_str());
		m_oscopeConn.translateData(resp, ',', ';');
		
		dataFile.open("/home/user/PlasmaGun/chan4.txt", std::ios_base::out | std::ios_base::trunc);
		dataFile << resp;
		dataFile.close();
		

		//CHANNEL3
		m_oscopeConn.command(":WAVEFORM:SOURCE CHANNEL3");		
		//m_oscopeConn.command(":DIGITIZE CHANNEL3");
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), cmd+"\n Written to file\n");
		m_oscopeConn.command(cmd);
		m_oscopeConn.getStrData(resp);
		g_print(resp.c_str());
		m_oscopeConn.translateData(resp, ',', ';');
		
		dataFile.open("/home/user/PlasmaGun/chan3.txt", std::ios_base::out | std::ios_base::trunc);
		dataFile << resp;
		dataFile.close();
	}
	

	
	void MainWindow::onConnect () {
		gtk_switch_set_state (GTK_SWITCH(operator[]("connect_switch")), TRUE);
	}
	
	void MainWindow::onDisconnect () {
		gtk_switch_set_state (GTK_SWITCH(operator[]("connect_switch")), FALSE);
	}

	void MainWindow::writeDataToFile (const std::string & path, const std::vector<int16_t> & data) {
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

	void MainWindow::writeDataToFile (const std::string & path, const std::vector<int16_t> & data, 
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
	
	void MainWindow::writeDataToFile (const std::string & path, const std::vector<int16_t> & data,
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
}


