import matplotlib
matplotlib.use("Qt5Agg")
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QDialog, QMenu, QHBoxLayout, QVBoxLayout, QSizePolicy, QMessageBox, QWidget
from math import pi
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from skippymonitor.fusion.madgwick2011 import Madgwick2011


class MathWidget(FigureCanvas):
    def __init__(self, parent=None, width=5, height=3, dpi=50):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        self.axes.hold(False)

        #
        FigureCanvas.__init__(self, fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                QSizePolicy.Expanding,
                QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

class Plot4(MathWidget):
    MAX_PTS = 1500

    def __init__(self, *args, **kwargs):
        MathWidget.__init__(self, *args, **kwargs)
        self.dataw = [ 0 ] * self.MAX_PTS
        self.datax = [ 0 ] * self.MAX_PTS
        self.datay = [ 0 ] * self.MAX_PTS
        self.dataz = [ 0 ] * self.MAX_PTS
        self.rangex = [ x for x in range(len(self.datax)) ]
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.redraw)
        self.timer.start(300)

    def redraw(self):
        self.axes.plot(self.rangex, self.datax, 'r', self.rangex, self.datay, 'g', self.rangex, self.dataz, 'b', self.rangex, self.dataw, 'y')
        self.draw()

    def addData(self, w, x, y, z):
        self.dataw.append(w)
        self.datax.append(x)
        self.datay.append(y)
        self.dataz.append(z)

        self.dataw = self.dataw[1:]
        self.datax = self.datax[1:]
        self.datay = self.datay[1:]
        self.dataz = self.dataz[1:]

class Plot3(MathWidget):
    MAX_PTS = 1500

    def __init__(self, *args, **kwargs):
        MathWidget.__init__(self, *args, **kwargs)
        self.datax = [ 0 ] * self.MAX_PTS
        self.datay = [ 0 ] * self.MAX_PTS
        self.dataz = [ 0 ] * self.MAX_PTS
        self.rangex = [ x for x in range(len(self.datax)) ]
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.redraw)
        self.timer.start(300)

    def redraw(self):
        self.axes.plot(self.rangex, self.datax, 'r', self.rangex, self.datay, 'g', self.rangex, self.dataz, 'b')
        self.draw()

    def addData(self, x, y, z):
        self.datax.append(x)
        self.datay.append(y)
        self.dataz.append(z)

        self.datax = self.datax[1:]
        self.datay = self.datay[1:]
        self.dataz = self.dataz[1:]

class Plot1(MathWidget):
    MAX_PTS = 1500

    def __init__(self, *args, **kwargs):
        MathWidget.__init__(self, *args, **kwargs)
        self.data = [ 0 ] * self.MAX_PTS
        self.rangex = [ x for x in range(len(self.data)) ]
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.redraw)
        self.timer.start(300)

    def redraw(self):
        self.axes.plot(self.rangex, self.data, 'r')
        self.draw()

    def addData(self, x):
        self.data.append(x)
        self.data = self.data[1:]

class PlotWindow(QDialog):
    def __init__(self, interpreterClass, parent=None):
        super(PlotWindow, self).__init__(parent)
        self.interpreter = interpreterClass()
        self.setupUi()
        self.fusion = Madgwick2011()

    def setupUi(self):
        self.setObjectName("PlotWindow")
        self.setModal(True)

        top = QHBoxLayout(self)

        ll = QVBoxLayout()
        ll1 = QVBoxLayout()
        self.gyroPlot = Plot3(self, width=5, height=3, dpi=50)
        self.accelPlot = Plot3(self, width=5, height=3, dpi=50)
        self.magPlot = Plot3(self, width=5, height=3, dpi=50)
        ll1.addWidget(self.gyroPlot)
        ll1.addWidget(self.accelPlot)
        ll1.addWidget(self.magPlot)
        ll.addLayout(ll1)

        llh = QHBoxLayout()
        self.presPlot = Plot1(self, width=2.5, height=3, dpi=50)
        self.tempPlot = Plot1(self, width=2.5, height=3, dpi=50)
        llh.addWidget(self.presPlot)
        llh.addWidget(self.tempPlot)

        ll.addLayout(llh)
        top.addLayout(ll)

        lg = QVBoxLayout()
        self.yawPlot = Plot1(self, width=5, height=3, dpi=50)
        self.pitchPlot = Plot1(self, width=5, height=3, dpi=50)
        self.rollPlot = Plot1(self, width=5, height=3, dpi=50)
        self.quatPlot = Plot4(self, width=5, height=3, dpi=50)
        lg.addWidget(self.yawPlot)
        lg.addWidget(self.pitchPlot)
        lg.addWidget(self.rollPlot)
        lg.addWidget(self.quatPlot)

        top.addLayout(lg)

        self.setFocus()

    def newData(self, data):
        self.interpreter.decode(data)
        self.gyroPlot.addData(self.interpreter.gyro.x, self.interpreter.gyro.y, self.interpreter.gyro.z)
        self.accelPlot.addData(self.interpreter.accel.x, self.interpreter.accel.y, self.interpreter.accel.z)
        self.presPlot.addData(self.interpreter.pressure)
        self.tempPlot.addData(self.interpreter.temperature)

        if self.interpreter.mag.x != 0 or self.interpreter.mag.y != 0 or self.interpreter.mag.z != 0:
            self.magPlot.addData(self.interpreter.mag.x, self.interpreter.mag.y, self.interpreter.mag.z)

        gx = self.interpreter.gyro.x * (2*pi / 360)
        gy = self.interpreter.gyro.y * (2*pi / 360)
        gz = self.interpreter.gyro.z * (2*pi / 360)
        self.fusion.update(gx, gy, gz, 
            self.interpreter.accel.x, self.interpreter.accel.y, self.interpreter.accel.z, 
            self.interpreter.mag.x, self.interpreter.mag.y, self.interpreter.mag.z)

        yaw, pitch, roll = self.fusion.to_euler()
        self.yawPlot.addData(yaw)
        self.pitchPlot.addData(pitch)
        self.rollPlot.addData(roll)
        self.quatPlot.addData(self.fusion.q0, self.fusion.q1, self.fusion.q2, self.fusion.q3)