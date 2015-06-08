from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtWidgets import QApplication, QDialog, QMenu, QHBoxLayout, QVBoxLayout, QSizePolicy, QMessageBox, QWidget
from PyQt5.QtOpenGL import *
from math import pi

from skippymonitor.fusion.madgwick2011 import Madgwick2011



class AttitudeWidget(QGLWidget):
    yaw = 0.0
    pitch = 0.0
    roll = 0.0

    def __init__(self, parent = None):
        super(AttitudeWidget, self).__init__(parent)
        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClearDepth(1.0) 

        self.timer = QtCore.QTimer(self)
        self.timer.setInterval(20)
        self.timer.timeout.connect(self.updateGL)
        self.timer.start()

    def paintGL(self):
        #print("Y {yaw}  P {pitch}  R {roll}".format(yaw=self.yaw, pitch=self.pitch, roll=self.roll))

        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()

        glMatrixMode(GL_MODELVIEW)

        glTranslatef(0.0,0.0,-6.0)

        glRotatef(self.pitch,1.0,0.0,0.0)
        glRotatef(self.yaw,0.0,1.0,0.0)
        glRotatef(self.roll,0.0,0.0,1.0)

        glBegin(GL_QUADS)
 
        glColor3f(0.0,1.0,0.0)
        glVertex3f( 1.0, 1.0,-1.0)
        glVertex3f(-1.0, 1.0,-1.0)
        glVertex3f(-1.0, 1.0, 1.0)
        glVertex3f( 1.0, 1.0, 1.0) 
 
        glColor3f(1.0,0.0,0.0)
        glVertex3f( 1.0,-1.0, 1.0)
        glVertex3f(-1.0,-1.0, 1.0)
        glVertex3f(-1.0,-1.0,-1.0)
        glVertex3f( 1.0,-1.0,-1.0) 
 
        glColor3f(0.0,1.0,1.0)
        glVertex3f( 1.0, 1.0, 1.0)
        glVertex3f(-1.0, 1.0, 1.0)
        glVertex3f(-1.0,-1.0, 1.0)
        glVertex3f( 1.0,-1.0, 1.0)
 
        glColor3f(1.0,1.0,0.0)
        glVertex3f( 1.0,-1.0,-1.0)
        glVertex3f(-1.0,-1.0,-1.0)
        glVertex3f(-1.0, 1.0,-1.0)
        glVertex3f( 1.0, 1.0,-1.0)
 
        glColor3f(0.0,0.0,1.0)
        glVertex3f(-1.0, 1.0, 1.0) 
        glVertex3f(-1.0, 1.0,-1.0)
        glVertex3f(-1.0,-1.0,-1.0) 
        glVertex3f(-1.0,-1.0, 1.0) 
 
        glColor3f(1.0,0.0,1.0)
        glVertex3f( 1.0, 1.0,-1.0) 
        glVertex3f( 1.0, 1.0, 1.0)
        glVertex3f( 1.0,-1.0, 1.0)
        glVertex3f( 1.0,-1.0,-1.0)

        glEnd()

    def resizeGL(self, w, h):
        glViewport(0, 0, w, h)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(60.0, float(w)/float(h), 0.001, 100.0)
        glMatrixMode(GL_MODELVIEW)

    def initializeGL(self):
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        glDepthFunc(GL_LESS)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_POLYGON_SMOOTH)
        glEnable(GL_BLEND)
        glEnable(GL_CULL_FACE);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glShadeModel(GL_SMOOTH)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    def setOrientation(self, yaw, pitch, roll):
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll
        #print("Y {yaw}  P {pitch}  R {roll}".format(yaw=yaw, pitch=pitch, roll=roll))
    

class AttitudeWindow(QDialog):
    def __init__(self, interpreterClass, parent=None):
        super(AttitudeWindow, self).__init__(parent)
        self.interpreter = interpreterClass()
        self.setupUi()
        self.fusion = Madgwick2011()

    def setupUi(self):
        self.setObjectName("AttitudeWindow")
        self.setModal(True)

        top = QHBoxLayout(self)
        ll = QVBoxLayout()
        self.glAttitude = AttitudeWidget(self)
        ll.addWidget(self.glAttitude)
        top.addLayout(ll)

        self.resize(640, 480)

    def newData(self, data):
        self.interpreter.decode(data)

        gx = self.interpreter.gyro.x * (2*pi / 360)
        gy = self.interpreter.gyro.y * (2*pi / 360)
        gz = self.interpreter.gyro.z * (2*pi / 360)

        self.fusion.update(gx, gy, gz, 
            self.interpreter.accel.x, self.interpreter.accel.y, self.interpreter.accel.z, 
            self.interpreter.mag.x, self.interpreter.mag.y, self.interpreter.mag.z)

        yaw, pitch, roll = self.fusion.to_euler()
        self.glAttitude.setOrientation(180*(yaw/pi), 180*(pitch/pi), 180*(roll/pi))