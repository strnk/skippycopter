import random
import matplotlib
matplotlib.use("Qt5Agg")
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QDialog, QMenu, QVBoxLayout, QSizePolicy, QMessageBox, QWidget
from numpy import arange, sin, pi
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


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

class Plot3(MathWidget):
    MAX_PTS = 1500

    def __init__(self, *args, **kwargs):
        MathWidget.__init__(self, *args, **kwargs)
        self.datax = []
        self.datay = []
        self.dataz = []
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.redraw)
        self.timer.start(300)

    def redraw(self):
        rangex = [ x for x in range(len(self.datax)) ]
        self.axes.plot(rangex, self.datax, 'r', rangex, self.datay, 'g', rangex, self.dataz, 'b')
        self.draw()

    def addData(self, x, y, z):
        self.datax.append(x)
        self.datay.append(y)
        self.dataz.append(z)

        if len(self.datax) > self.MAX_PTS:
            self.datax = self.datax[1:]
            self.datay = self.datay[1:]
            self.dataz = self.dataz[1:]

class PlotWindow(QDialog):
    def __init__(self, interpreterClass, parent=None):
        super(PlotWindow, self).__init__(parent)
        self.interpreter = interpreterClass()
        self.setupUi()

    def setupUi(self):
        self.setObjectName("PlotWindow")
        self.setModal(True)

        l = QVBoxLayout(self)

        self.gyroPlot = Plot3(self, width=5, height=3, dpi=50)
        self.accelPlot = Plot3(self, width=5, height=3, dpi=50)
        self.magPlot = Plot3(self, width=5, height=3, dpi=50)

        l.addWidget(self.gyroPlot)
        l.addWidget(self.accelPlot)
        l.addWidget(self.magPlot)

        self.setFocus()

    def newData(self, data):
        self.interpreter.decode(data)
        self.gyroPlot.addData(self.interpreter.gyro.x, self.interpreter.gyro.y, self.interpreter.gyro.z)
        self.accelPlot.addData(self.interpreter.accel.x, self.interpreter.accel.y, self.interpreter.accel.z)

        if self.interpreter.mag.x != 0 or self.interpreter.mag.y != 0 or self.interpreter.mag.z != 0:
            self.magPlot.addData(self.interpreter.mag.x, self.interpreter.mag.y, self.interpreter.mag.z)