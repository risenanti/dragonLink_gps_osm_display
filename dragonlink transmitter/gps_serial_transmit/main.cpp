#include <vector>
#include <unistd.h>
#include <memory>

#include <Navio2/Led_Navio2.h>
#include <Navio+/Led_Navio.h>
#include <Common/Ublox.h>
#include <Common/Util.h>

#include <QCoreApplication>
#include <QtSerialPort>
#include <QString>

using namespace std;

#define SERIALPORTNAME "ttyUSB0"
#define SERIALBAUDRATE QSerialPort::Baud115200

void print_gps_data(vector<double> pos_data);

unique_ptr <Led> get_led()
{
    if (get_navio_version() == NAVIO2)
    {
        auto ptr = unique_ptr <Led>{ new Led_Navio2() };
        return ptr;
    } else
    {
        auto ptr = unique_ptr <Led>{ new Led_Navio() };
        return ptr;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);
    QSerialPort m_serialPort;
    m_serialPort.setPortName(SERIALPORTNAME);
	m_serialPort.setBaudRate(SERIALBAUDRATE);
    QByteArray input;
    QByteArray output;

    if (check_apm()) {
        return 1;
    }

    if (getuid()) {
        fprintf(stderr, "Not root. Please launch like this: sudo %s\n", argv[0]);
    }

    auto led = get_led();
    if (!led->initialize())
        return EXIT_FAILURE;
    
    vector<double> pos_data;
    Ublox gps;
    
    if(!gps.testConnection()){
		cout <<"ERROR READING FROM GPS"<<endl;
		return 1;
	}

    while (true) {
		//Beginning of Loop
        led->setColor(Colors::Green);
        
        if(!m_serialPort.open(QIODevice::ReadWrite)){
         cout << "ERROR CANNOT OPEN SERIAL PORT"<<endl;
         return -1;
         }  

		if(gps.decodeSingleMessage(Ublox::NAV_POSLLH, pos_data) == 1){
            
            led->setColor(Colors::Yellow);
            QString gpsDatString = "Latitude=";
            gpsDatString += QString::number(pos_data[2]);
            gpsDatString +=",Longitude=";
            gpsDatString += QString::number(pos_data[1]);
            gpsDatString += "\r\n";
            
            output=gpsDatString.toUtf8();
            
            if(m_serialPort.isWritable()){
			m_serialPort.write(output);
			m_serialPort.flush();
			m_serialPort.waitForBytesWritten(100);
			}
			
			if(m_serialPort.bytesAvailable()>=18)
			input = m_serialPort.read(18);
			
			usleep(500*1000);
         }
		else{
			led->setColor(Colors::Red);
			}	
			
		//End of Loop
        led->setColor(Colors::Blue);
        //sleep 500 MS
        m_serialPort.close();
        usleep(500*1000);
    }
        
    return coreApplication.exec();
}

void print_gps_data(vector<double> pos_data){
	printf("GPS Millisecond Time of Week: %.0lf s\n", pos_data[0]/1000);
    printf("Longitude: %lf\n", pos_data[1]/10000000);
    printf("Latitude: %lf\n", pos_data[2]/10000000);
    printf("Height above Ellipsoid: %.3lf m\n", pos_data[3]/1000);
    printf("Height above mean sea level: %.3lf m\n", pos_data[4]/1000);
    printf("Horizontal Accuracy Estimate: %.3lf m\n", pos_data[5]/1000);
    printf("Vertical Accuracy Estimate: %.3lf m\n", pos_data[6]/1000);
}
