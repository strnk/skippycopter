from skippymonitor.ui import AboutUI, MainUI
from PyQt5 import QtWidgets, QtCore
from PyQt5.QtCore import pyqtSignal, pyqtSlot

import sys

class AboutDialog(QtWidgets.QDialog, AboutUI.Ui_Dialog):
    def __init__(self, parent=None):
        super(AboutDialog, self).__init__(parent)
        self.setupUi(self)

class MainWindow(QtWidgets.QMainWindow, MainUI.Ui_MainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)

    @pyqtSlot(bool)
    def on_actionAbout_triggered(self, checked):
        dialog = AboutDialog(self)
        dialog.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        dialog.show()


class MainGUI:
	def __init__(self):
		pass

	def run(self):
	    app = QtWidgets.QApplication(sys.argv)
	    main = MainWindow()
	    main.show()
	    app.exec_()