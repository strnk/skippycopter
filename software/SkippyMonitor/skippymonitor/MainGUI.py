from skippymonitor.ui import AboutUI, MainUI
from skippymonitor.uart import direct, bluetooth, thread
from skippymonitor.PlotWindow import PlotWindow
from skippymonitor.uart.interpreters import *

from optparse import OptionParser

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import pyqtSignal, pyqtSlot

import sys

class AboutDialog(QtWidgets.QDialog, AboutUI.Ui_Dialog):
    def __init__(self, parent=None):
        super(AboutDialog, self).__init__(parent)
        self.setupUi(self)

class MainWindow(QtWidgets.QMainWindow, MainUI.Ui_MainWindow):
    IMUWindow = None
    bRedirectData = False

    def __init__(self, interpreterClass, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.Interpreter = interpreterClass

    @pyqtSlot(bool)
    def on_actionAbout_triggered(self, checked):
        dialog = AboutDialog(self)
        dialog.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        dialog.show()

    @pyqtSlot(bool)
    def on_actionRecord_IMU_triggered(self, checked):
        self.IMUWindow = PlotWindow(self.Interpreter, self)
        self.IMUWindow.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        self.IMUWindow.destroyed.connect(self.on_plotWindow_closed)
        self.bRedirectData = True
        self.IMUWindow.show()

    def on_plotWindow_closed(self):
        self.bRedirectData = False
        self.IMUWindow = None

    def addLine(self, string):
        if not self.bRedirectData:
            newText = self.plainTextEdit.toPlainText() + string
            self.plainTextEdit.setPlainText('\n'.join((newText.split('\n'))[-50:]))
            self.plainTextEdit.moveCursor(QtGui.QTextCursor.End)
            self.plainTextEdit.ensureCursorVisible()
        else:
            if self.IMUWindow != None:
                self.IMUWindow.newData(string)


class MainGUI:
    def __init__(self):
        opt = OptionParser(usage="Usage: %prog < -bluetooth | -serial >", version="%prog 1.0")
        opt.add_option("-B", "--bluetooth",
                        action="store",
                        dest="bluetoothMacAddr",
                        help="Bluetooth MAC address")
        opt.add_option("-c", "--channel",
                        action="store", default="1",
                        dest="bluetoothChannel",
                        help="Bluetooth channel")

        opt.add_option("-S", "--serial",
                        action="store",
                        dest="serialDev",
                        help="Serial device")
        opt.add_option("-b", "--baudrate",
                        action="store", default="9600",
                        dest="serialSpeed",
                        help="Serial baudrate")

        opt.add_option("-i", "--imu-format",
                        action="store",
                        default="M3A3G3PTL",
                        dest="imuDataInterpreterClass",
                        help="IMU data format")
        (self.options, self.args) = opt.parse_args()


    def run(self):
        if self.options.bluetoothMacAddr != None:
            self.Serial = bluetooth.BluetoothSerial(self.options.bluetoothMacAddr, int(self.options.bluetoothChannel))
        elif self.options.serialDev != None:
            self.Serial = direct.DirectSerial(self.options.serialDev, int(self.options.serialSpeed))
        else:
            print("Bluetooth link or serial device must be specified")
            return

        self.Interpreter = eval(self.options.imuDataInterpreterClass)
        self.Serial.open()

        app = QtWidgets.QApplication(sys.argv)
        main = MainWindow(self.Interpreter)
        main.show()

        self.SerialThread = thread.QtReader(self.Serial)
        self.SerialThread.lineUpdate.connect(main.addLine)
        self.SerialThread.start()

        app.exec_()
        self.SerialThread.stop()
        self.Serial.close()