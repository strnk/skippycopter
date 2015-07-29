from PyQt5 import QtCore, QtGui, QtWidgets
import threading
import time
 
class QtReader(QtCore.QThread):
    newLine = QtCore.pyqtSignal(object)
    newMessage = QtCore.pyqtSignal(object, object)
    notStopped = True

    def __init__(self, serial):
        QtCore.QThread.__init__(self)
        self.serial = serial
        
    def run(self):
        while self.notStopped:
            line = self.serial.readline()

            if len(line):
                if line[0] == '$':
                    message = line.split(' ', 1)
                    self.newMessage.emit(message[0][1:].strip(), message[1].strip())
                else:
                    self.newLine.emit(line)
            else:
                time.sleep(.01)

    def stop(self):
        self.notStopped = False