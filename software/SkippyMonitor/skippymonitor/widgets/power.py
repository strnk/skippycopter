from PyQt5.QtWidgets import QLabel, QProgressBar
from struct import unpack

class PowerIndicator():
    parent = None
    progressbar = None
    text = None

    maxVoltPerCell = 0
    minVoltPerCell = 0

    fullStyle = "QProgressBar::chunk { background-color: #00EE00; }"
    halfStyle = "QProgressBar::chunk { background-color: #FFCC00; }"
    emptyStyle = "QProgressBar::chunk { background-color: #FF5050; }"

    def __init__(self, parent, minVoltPerCell = 3.2, maxVoltPerCell = 4.2):
        self.minVoltPerCell = 3.2
        self.maxVoltPerCell = 4.2

        self.parent = parent
        self.text = QLabel('', parent)

        self.progressbar = QProgressBar(parent)
        self.progressbar.setMaximumWidth(100)
        self.progressbar.setMaximumHeight(10)
        self.progressbar.setMaximum(100)
        self.progressbar.setMinimum(0)
        self.progressbar.setTextVisible(False)

        parent.statusBar.addPermanentWidget(self.progressbar)
        parent.statusBar.addPermanentWidget(self.text)

    def updateProgressBar(self, voltage):
        pct = ((voltage - self.minVoltPerCell) / (self.maxVoltPerCell-self.minVoltPerCell))*100
        self.progressbar.setValue(pct)

        if pct <= 20:
            self.progressbar.setStyleSheet(self.emptyStyle)
        elif pct <= 50:
            self.progressbar.setStyleSheet(self.halfStyle)
        else:
            self.progressbar.setStyleSheet(self.fullStyle)

    def newMessage(self, message, data):
        if message == 'POW':
            data = data.split()

            if len(data) < 2:
                return

            volts = unpack('!f', bytes.fromhex(data[0]))[0]
            amps = unpack('!f', bytes.fromhex(data[1]))[0]

            self.text.setText("%.2f V/cell, %.3f A" % (volts/4, amps))
            self.updateProgressBar(volts/4)