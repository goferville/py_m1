#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
ZetCode PyQt5 tutorial

This program creates a menubar. The
menubar has one menu with an exit action.

Author: Jan Bodnar
Website: zetcode.com
Last edited: January 2017
"""

import sys
from PyQt5.QtWidgets import QMainWindow, QAction, qApp, QApplication, QMenu
from PyQt5.QtGui import QIcon


class Example(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # add Menu & sub Menu  ----------------------
        exitAct = QAction(QIcon('exit.png'), '&Exit', self)  # each action will be one item under 'File'
        exitAct.setShortcut('Ctrl+Q')
        exitAct.setStatusTip('Exit application')
        exitAct.triggered.connect(qApp.quit)

        openAct = QAction(QIcon('open.png'), '&Open', self)
        openAct.setShortcut(('Ctrl+O'))
        openAct.setStatusTip('Open File')

        subFileAct1 = QAction('&subFile1', self)
        subFileAct1.setStatusTip('Sub File 1')
        subFileAct1.setShortcut(('Ctrl+S'))
        subFileAct2 = QAction('&subFile2', self)
        subFileAct2.setStatusTip('Sub File 2')
        subFileAct2.setShortcut(('Ctrl+D'))

        self.statusBar()  # setup status bar
        menubar = self.menuBar()  # setup menu bar

        subMenu = QMenu('Open1', self)  # QMenu is for 2nd level sub menus

        subMenu.addAction(subFileAct1)
        subMenu.addAction(subFileAct2)

        fileMenu = menubar.addMenu('&File')  # add 'File' in main menu bar
        # main menu bar can add 'Action' (single choice) or 'QMenu' ( multi choice sub menus)
        fileMenu.addAction(openAct)  # This is one of items uner 'File',add for Koala test
        fileMenu.addMenu(subMenu)  # Add sub menu
        fileMenu.addAction(exitAct)  # This is another one of items uner 'File'
        # add tool bar  -------------------------------
        # we add actions to tool bar
        toolbar = self.addToolBar('Tools')
        toolbar.addAction(exitAct)
        toolbar.addAction(openAct)

        # run   -----------------------------------------
        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('Simple menu')
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())