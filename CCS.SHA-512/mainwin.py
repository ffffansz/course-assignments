# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwin.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Mainwin(object):
    def setupUi(self, Mainwin):
        Mainwin.setObjectName("Mainwin")
        Mainwin.resize(672, 425)
        self.label_3 = QtWidgets.QLabel(Mainwin)
        self.label_3.setGeometry(QtCore.QRect(410, 312, 239, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(9)
        self.label_3.setFont(font)
        self.label_3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_3.setObjectName("label_3")
        self.label_5 = QtWidgets.QLabel(Mainwin)
        self.label_5.setGeometry(QtCore.QRect(410, 380, 239, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_5.sizePolicy().hasHeightForWidth())
        self.label_5.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(9)
        self.label_5.setFont(font)
        self.label_5.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_5.setObjectName("label_5")
        self.label_4 = QtWidgets.QLabel(Mainwin)
        self.label_4.setGeometry(QtCore.QRect(410, 346, 239, 28))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_4.sizePolicy().hasHeightForWidth())
        self.label_4.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(9)
        self.label_4.setFont(font)
        self.label_4.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_4.setObjectName("label_4")
        self.label_2 = QtWidgets.QLabel(Mainwin)
        self.label_2.setGeometry(QtCore.QRect(30, 30, 191, 31))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(18)
        self.label_2.setFont(font)
        self.label_2.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.label_2.setObjectName("label_2")
        self.label = QtWidgets.QLabel(Mainwin)
        self.label.setGeometry(QtCore.QRect(30, 106, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.plain_textedit = QtWidgets.QTextEdit(Mainwin)
        self.plain_textedit.setGeometry(QtCore.QRect(30, 136, 411, 81))
        self.plain_textedit.setObjectName("plain_textedit")
        self.encry_bt = QtWidgets.QPushButton(Mainwin)
        self.encry_bt.setGeometry(QtCore.QRect(30, 246, 101, 31))
        self.encry_bt.setObjectName("encry_bt")
        self.ret_textedit = QtWidgets.QTextEdit(Mainwin)
        self.ret_textedit.setGeometry(QtCore.QRect(30, 306, 411, 91))
        self.ret_textedit.setObjectName("ret_textedit")
        self.line = QtWidgets.QFrame(Mainwin)
        self.line.setGeometry(QtCore.QRect(30, 70, 411, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.line_2 = QtWidgets.QFrame(Mainwin)
        self.line_2.setGeometry(QtCore.QRect(473, 140, 20, 261))
        self.line_2.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.import_bt = QtWidgets.QPushButton(Mainwin)
        self.import_bt.setGeometry(QtCore.QRect(410, 220, 31, 20))
        self.import_bt.setObjectName("import_bt")

        self.retranslateUi(Mainwin)
        QtCore.QMetaObject.connectSlotsByName(Mainwin)

    def retranslateUi(self, Mainwin):
        _translate = QtCore.QCoreApplication.translate
        Mainwin.setWindowTitle(_translate("Mainwin", "Form"))
        self.label_3.setText(_translate("Mainwin", "Libre, 15336036"))
        self.label_5.setText(_translate("Mainwin", "@SYSU"))
        self.label_4.setText(_translate("Mainwin", "branchvan379@gmail.com"))
        self.label_2.setText(_translate("Mainwin", "SHA512加密程序"))
        self.label.setText(_translate("Mainwin", "请输入要加密的文本："))
        self.encry_bt.setText(_translate("Mainwin", "加密"))
        self.import_bt.setText(_translate("Mainwin", "..."))
