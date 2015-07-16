from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import QUrl
from PyQt5.QtWidgets import QApplication, QDialog, QMenu, QHBoxLayout, QVBoxLayout, QSizePolicy, QMessageBox, QWidget
from PyQt5.QtWebKitWidgets import QWebView
from PyQt5.QtOpenGL import *
from math import pi


class GPSWindow(QDialog):
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
        top.addLayout(ll)

        self.resize(640, 480)
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

function moveToLocation(lat, lng){
    var center = new google.maps.LatLng(lat, lng);
    map.panTo(center);
}

function initialize() {
  // Create the map.
  var mapOptions = {
    zoom: 4,
    center: new google.maps.LatLng(0, 0),
    mapTypeId: google.maps.MapTypeId.TERRAIN
  };

  map = new google.maps.Map(document.getElementById('map-canvas'),
      mapOptions);
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
            lat = float(fields[3]) * (1.0 if fields[4] == 'N' else -1.0)
            lon = float(fields[5]) * (1.0 if fields[4] == 'E' else -1.0)

            lat = int(lat/100) + (lat - (int(lat/100) * 100)) / 60
            lon = int(lon/100) + (lon - (int(lon/100) * 100)) / 60

            self.web.page().mainFrame().evaluateJavaScript('moveToLocation(%f, %f)' % (lat, lon))

    def newData(self, string):
        data = string.rstrip('\r\n').split()

        if data[0] != 'GPS':
            return

        self.parseNMEA(' '.join(data[1:]))
        