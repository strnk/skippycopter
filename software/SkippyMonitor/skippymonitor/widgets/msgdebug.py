from PyQt5 import QtCore
from PyQt5.QtCore import pyqtSignal, pyqtSlot
from PyQt5.QtWidgets import QTreeWidgetItem
from PyQt5.QtGui import QBrush, QColor

class MainSystemMessageHandler:
    msgListWidget = None
    msgCfgWidget = None

    knownMessages = {
        'POW': [ 0, 'System power consumption', 'cyan' ],
        'AT': [ 0, 'Attitude', 'magenta' ],
        'GPS': [ 0, 'GPS messages', 'yellow' ]
    }

    brushes = {}

    def __init__(self, listWidget, cfgWidget):
        self.msgListWidget = listWidget
        self.msgCfgWidget = cfgWidget

        self.msgListWidget.setColumnCount(2)
        self.msgListWidget.setHeaderLabels(["Msg", "Data"])
        self.msgListWidget.header().resizeSection(0, 40)
        self.msgListWidget.setIndentation(0)

        self.msgCfgWidget.setColumnCount(2)
        self.msgCfgWidget.setHeaderLabels(["Message", "Description"])
        self.msgCfgWidget.header().resizeSection(0, 60)
        self.msgCfgWidget.setIndentation(0)
        self.msgCfgWidget.itemChanged.connect(self.cfgItemClicked)

        self.generateConfig()

    def generateConfig(self):
        self.msgCfgWidget.clear()
        self.brushes = {}

        for message, state in self.knownMessages.items():
            w = QTreeWidgetItem()
            w.setText(0, message)
            w.setText(1, state[1])
            w.setFlags(QtCore.Qt.ItemIsUserCheckable | QtCore.Qt.ItemIsEnabled)
            w.setCheckState(0, QtCore.Qt.Checked if state[0] else QtCore.Qt.Unchecked)

            bg = QColor(state[2])
            bg.setAlphaF(.5)
            self.brushes[message] = QBrush(bg)

            w.setBackground(0, self.brushes[message])
            w.setBackground(1, self.brushes[message])
            self.msgCfgWidget.addTopLevelItem(w)

        self.msgCfgWidget.resizeColumnToContents(0)

    @pyqtSlot(QTreeWidgetItem, int)
    def cfgItemClicked(self, item, column):
        self.knownMessages[item.text(0)][0] = 1 if item.checkState(0) == QtCore.Qt.Checked else 0

    def newMessage(self, message, data):
        if message not in self.knownMessages:
            self.knownMessages[message] = [ 1, '???', 'white' ]
            self.generateConfig()

        if self.knownMessages[message][0]:
            w = QTreeWidgetItem([message, data])
            w.setBackground(0, self.brushes[message])
            w.setBackground(1, self.brushes[message])

            self.msgListWidget.insertTopLevelItem(0, w)
            self.msgListWidget.resizeColumnToContents(0)