#ifndef GPS_GPS_H
#define GPS_GPS_H

#define GPS_NMEA_MSG_MAXLEN	82	//!< Maximum length of a NMEA message

class GPS {
private:

public:
	volatile bool pending;

	GPS(void);

	void init(void);
	void trigger(void);
};

#ifndef GPS_NO_EXTERN
extern GPS GPSHandler;
#endif

#endif // GPS_GPS_H