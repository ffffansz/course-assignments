# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'encrywin.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_encryWin(object):
    def setupUi(self, encryWin):
        encryWin.setObjectName("encryWin")
        encryWin.resize(655, 751)
        self.label_2 = QtWidgets.QLabel(encryWin)
        self.label_2.setGeometry(QtCore.QRect(20, 30, 111, 31))
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
        self.plain_textedit = QtWidgets.QTextEdit(encryWin)
        self.plain_textedit.setGeometry(QtCore.QRect(20, 160, 611, 81))
        self.plain_textedit.setObjectName("plain_textedit")
        self.line = QtWidgets.QFrame(encryWin)
        self.line.setGeometry(QtCore.QRect(20, 80, 611, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.label = QtWidgets.QLabel(encryWin)
        self.label.setGeometry(QtCore.QRect(20, 130, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.e_textedit = QtWidgets.QTextEdit(encryWin)
        self.e_textedit.setGeometry(QtCore.QRect(20, 300, 611, 81))
        self.e_textedit.setObjectName("e_textedit")
        self.label_3 = QtWidgets.QLabel(encryWin)
        self.label_3.setGeometry(QtCore.QRect(20, 270, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.n_textedit = QtWidgets.QTextEdit(encryWin)
        self.n_textedit.setGeometry(QtCore.QRect(20, 440, 611, 81))
        self.n_textedit.setObjectName("n_textedit")
        self.label_4 = QtWidgets.QLabel(encryWin)
        self.label_4.setGeometry(QtCore.QRect(20, 410, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.encry_ret_textedit = QtWidgets.QTextEdit(encryWin)
        self.encry_ret_textedit.setGeometry(QtCore.QRect(20, 580, 611, 81))
        self.encry_ret_textedit.setObjectName("encry_ret_textedit")
        self.label_5 = QtWidgets.QLabel(encryWin)
        self.label_5.setGeometry(QtCore.QRect(20, 550, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.encry_bt = QtWidgets.QPushButton(encryWin)
        self.encry_bt.setGeometry(QtCore.QRect(150, 690, 101, 31))
        self.encry_bt.setObjectName("encry_bt")
        self.export_bt = QtWidgets.QPushButton(encryWin)
        self.export_bt.setGeometry(QtCore.QRect(360, 690, 161, 31))
        self.export_bt.setObjectName("export_bt")

        self.retranslateUi(encryWin)
        QtCore.QMetaObject.connectSlotsByName(encryWin)

    def retranslateUi(self, encryWin):
        _translate = QtCore.QCoreApplication.translate
        encryWin.setWindowTitle(_translate("encryWin", "Form"))
        self.label_2.setText(_translate("encryWin", "RSA 加密"))
        self.label.setText(_translate("encryWin", "请输入要加密的文本："))
        self.label_3.setText(_translate("encryWin", "请输入e："))
        self.label_4.setText(_translate("encryWin", "请输入n："))
        self.label_5.setText(_translate("encryWin", "加密结果："))
        self.encry_bt.setText(_translate("encryWin", "加密"))
        self.export_bt.setText(_translate("encryWin", "导出加密结果到文本文件"))

