from skippymonitor.ui import AboutUI, MainUI
from skippymonitor.uart import direct, bluetooth, thread
from skippymonitor.imu_debug import PlotWindow
from skippymonitor.attitude import AttitudeWindow
from skippymonitor.uart.interpreters import *
from skippymonitor.calibration import IMUCalibration

from optparse import OptionParser

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import pyqtSignal, pyqtSlot
from PyQt5.QtWidgets import QProgressDialog, QMessageBox

import sys
import time

class AboutDialog(QtWidgets.QDialog, AboutUI.Ui_Dialog):
    def __init__(self, parent=None):
        super(AboutDialog, self).__init__(parent)
        self.setupUi(self)

class MainWindow(QtWidgets.QMainWindow, MainUI.Ui_MainWindow):
    IMUWindow = None
    AttitudeWindow = None
    calibration = None
    bRedirectData = False

    def __init__(self, linkType, interpreterClass, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.Interpreter = interpreterClass
        self.linkType = linkType
        self.statusBar.showMessage("Connected on " + linkType)

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

    @pyqtSlot(bool)
    def on_action3D_Attitude_triggered(self, checked):
        self.AttitudeWindow = AttitudeWindow(self.Interpreter, self)
        self.AttitudeWindow.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        self.AttitudeWindow.destroyed.connect(self.on_attitudeWindow_closed)
        self.bRedirectData = True
        self.AttitudeWindow.show()

    def on_attitudeWindow_closed(self):
        self.bRedirectData = False
        self.AttitudeWindow = None

    @pyqtSlot(bool)
    def on_actionCalibrate_triggered(self, checked):
        reply = QMessageBox.information(self, "IMU calibration",
            "The IMU will be calibrated. The board must stay flat and stable during the operations", 
            QMessageBox.Ok | QMessageBox.Cancel, QMessageBox.Cancel)

        if reply == QMessageBox.Cancel:
            return

        self.calibration = IMUCalibration(self.Interpreter, self)
        self.calibration.calibrationEnd.connect(self.on_calibration_end)
        self.calibration.calibrationCanceled.connect(self.on_calibration_canceled)
        self.bRedirectData = True
        self.calibration.start()

    def on_calibration_canceled(self):
        print("Calibration canceled")
        self.calibration = None
        self.bRedirectData = False

    def on_calibration_end(self):
        print("End of calibration:")
        print("  Gyro:")
        print("    X: offset={offset}, noise={noise}".format(offset=self.calibration.gyro.x.mean, noise=self.calibration.gyro.x.noise))
        print("    Y: offset={offset}, noise={noise}".format(offset=self.calibration.gyro.y.mean, noise=self.calibration.gyro.y.noise))
        print("    Z: offset={offset}, noise={noise}".format(offset=self.calibration.gyro.z.mean, noise=self.calibration.gyro.z.noise))
        self.calibration = None
        self.bRedirectData = False

    def addLine(self, string):
        if not self.bRedirectData:
            newText = self.plainTextEdit.toPlainText() + string
            self.plainTextEdit.setPlainText('\n'.join((newText.split('\n'))[-50:]))
            self.plainTextEdit.moveCursor(QtGui.QTextCursor.End)
            self.plainTextEdit.ensureCursorVisible()
        else:
            if self.IMUWindow != None:
                self.IMUWindow.newData(string)
            if self.AttitudeWindow != None:
                self.AttitudeWindow.newData(string)
            if self.calibration != None:
                self.calibration.newData(string)


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
            linkType = "bluetooth device {device}, CH{channel}".format(device=self.options.bluetoothMacAddr, channel=self.options.bluetoothChannel)
        elif self.options.serialDev != None:
            self.Serial = direct.DirectSerial(self.options.serialDev, int(self.options.serialSpeed))
            linkType = "serial device {device} at {speed} bps".format(device=self.options.serialDev, speed=self.options.serialSpeed)
        else:
            print("Bluetooth link or serial device must be specified")
            return

        self.Interpreter = eval(self.options.imuDataInterpreterClass)

        app = QtWidgets.QApplication(sys.argv)
        main = MainWindow(linkType, self.Interpreter)

        try:
            self.Serial.open()
        except OSError:
            QMessageBox.critical(main, "SkippyMonitor",
                "Unable to open " + linkType, QMessageBox.Close, QMessageBox.Close)
            return

        main.show()

        self.SerialThread = thread.QtReader(self.Serial)
        self.SerialThread.lineUpdate.connect(main.addLine)
        self.SerialThread.start()

        app.exec_()
        self.SerialThread.stop()
        self.Serial.close()