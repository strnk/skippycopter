from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import QUrl
from PyQt5.QtWidgets import QApplication, QDialog, QMenu, QHBoxLayout, QVBoxLayout, QSizePolicy, QMessageBox, QWidget, QStatusBar
from PyQt5.QtWebKitWidgets import QWebView
from PyQt5.QtOpenGL import *
from math import pi


class GPSWindow(QDialog):
    satellites = 0      # Satellites in view
    hdop = 40           # Horizontal dilution of position

    def __init__(self, parent=None):
        super(GPSWindow, self).__init__(parent)
        self.setupUi()

    def setupUi(self):
        self.setObjectName("GPSWindow")
        self.setModal(True)

        top = QHBoxLayout(self)
        ll = QVBoxLayout()
        self.web = QWebView()
        ll.addWidget(self.web)

        self.statusBar = QStatusBar(self)
        ll.addWidget(self.statusBar)

        top.addLayout(ll)

        self.resize(640, 480)

        self.statusBar.showMessage("No fix")
        self.web.setHtml('''<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no">
    <meta charset="utf-8">
    <title>Circles</title>
    <style>
      html, body, #map-canvas {
        height: 100%;
        margin: 0;
        padding: 0;
      }

    </style>
    <script src="https://maps.googleapis.com/maps/api/js?v=3.exp&signed_in=true"></script>
    <script>
var map;
var quadcircle;

function updatePosition(lat, lng, err){
    quadCircle.setMap(map);
    quadCircle.setCenter(new google.maps.LatLng(lat, lng));
    quadCircle.setRadius(err);
}

function hidePosition(){
    quadCircle.setMap(null);
}

function moveToLocation(lat, lng){
    var center = new google.maps.LatLng(lat, lng);
    map.panTo(center);
}

function initialize() {
    // Create the map.
    map = new google.maps.Map(document.getElementById('map-canvas'), {
        zoom: 4,
        center: new google.maps.LatLng(0, 0),
        mapTypeId: google.maps.MapTypeId.TERRAIN
    });

    // Representation of the quadricopter on map
    quadCircle = new google.maps.Circle({
        strokeColor: '#FF0000',
        strokeOpacity: 0.8,
        strokeWeight: 2,
        fillColor: '#FF0000',
        fillOpacity: 0.35,
        map: null
    });
    hidePosition();
}

google.maps.event.addDomListener(window, 'load', initialize);

    </script>
  </head>
  <body>
    <div id="map-canvas"></div>
  </body>
</html>''')

    def parseNMEA(self, nmeaMessage):
        fields = nmeaMessage.split(',')

        # Recommended Minimum
        if fields[0] == '$GPRMC':
            if fields[3] != '' and fields[5] != '':
                lat = float(fields[3]) * (1.0 if fields[4] == 'N' else -1.0)
                lon = float(fields[5]) * (1.0 if fields[4] == 'E' else -1.0)

                lat = int(lat/100) + (lat - (int(lat/100) * 100)) / 60
                lon = int(lon/100) + (lon - (int(lon/100) * 100)) / 60

                self.web.page().mainFrame().evaluateJavaScript('moveToLocation(%f, %f)' % (lat, lon))
                self.web.page().mainFrame().evaluateJavaScript('updatePosition(%f, %f, %d)' % (lat, lon, self.hdop * 2.5))

                self.statusBar.showMessage("Fix, %d satellites in view" % (self.satellites))
            else:
                self.statusBar.showMessage("No fix, %d satellites in view" % (self.satellites))
                self.web.page().mainFrame().evaluateJavaScript('hidePosition()')
        elif fields[0] == '$GPGSV':
            self.satellites = int(fields[3])
        elif fields[0] == '$GPGSA':
            self.hdop = float(fields[16])

    def newData(self, string):
        data = string.rstrip('\r\n').split()

        if data[0] != 'GPS':
            return

        self.parseNMEA(' '.join(data[1:]))

    def newMessage(self, message, data):
        if message == 'GPS':
            self.parseNMEA(data)
        