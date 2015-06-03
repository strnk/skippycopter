from PyQt5 import QtCore, QtGui, QtWidgets
import threading
import time
 
class QtReader(QtCore.QThread):
    lineUpdate = QtCore.pyqtSignal(object)  
    notStopped = True

    def __init__(self, serial):
        QtCore.QThread.__init__(self)
        self.serial = serial
        self.addLine("Run")
        
    def run(self):
        while self.notStopped:
            line = self.serial.readline()

            if len(line):
                self.addLine(line)
            else:
                time.sleep(.01)

    def addLine(self, line):
        self.lineUpdate.emit(line)

    def stop(self):
        self.notStopped = False