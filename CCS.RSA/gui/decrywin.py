# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'decrywin.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_decryWin(object):
    def setupUi(self, decryWin):
        decryWin.setObjectName("decryWin")
        decryWin.resize(655, 751)
        self.label_2 = QtWidgets.QLabel(decryWin)
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
        self.cipher_textedit = QtWidgets.QTextEdit(decryWin)
        self.cipher_textedit.setGeometry(QtCore.QRect(20, 160, 611, 81))
        self.cipher_textedit.setObjectName("cipher_textedit")
        self.line = QtWidgets.QFrame(decryWin)
        self.line.setGeometry(QtCore.QRect(20, 80, 611, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.label = QtWidgets.QLabel(decryWin)
        self.label.setGeometry(QtCore.QRect(20, 130, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.d_textedit = QtWidgets.QTextEdit(decryWin)
        self.d_textedit.setGeometry(QtCore.QRect(20, 300, 611, 81))
        self.d_textedit.setObjectName("d_textedit")
        self.label_3 = QtWidgets.QLabel(decryWin)
        self.label_3.setGeometry(QtCore.QRect(20, 270, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.n_textedit = QtWidgets.QTextEdit(decryWin)
        self.n_textedit.setGeometry(QtCore.QRect(20, 440, 611, 81))
        self.n_textedit.setObjectName("n_textedit")
        self.label_4 = QtWidgets.QLabel(decryWin)
        self.label_4.setGeometry(QtCore.QRect(20, 410, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.decry_ret_textedit = QtWidgets.QTextEdit(decryWin)
        self.decry_ret_textedit.setGeometry(QtCore.QRect(20, 580, 611, 81))
        self.decry_ret_textedit.setObjectName("decry_ret_textedit")
        self.label_5 = QtWidgets.QLabel(decryWin)
        self.label_5.setGeometry(QtCore.QRect(20, 550, 151, 16))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.decry_bt = QtWidgets.QPushButton(decryWin)
        self.decry_bt.setGeometry(QtCore.QRect(150, 690, 101, 31))
        self.decry_bt.setObjectName("decry_bt")
        self.import_bt = QtWidgets.QPushButton(decryWin)
        self.import_bt.setGeometry(QtCore.QRect(360, 690, 161, 31))
        self.import_bt.setObjectName("import_bt")

        self.retranslateUi(decryWin)
        QtCore.QMetaObject.connectSlotsByName(decryWin)

    def retranslateUi(self, decryWin):
        _translate = QtCore.QCoreApplication.translate
        decryWin.setWindowTitle(_translate("decryWin", "Form"))
        self.label_2.setText(_translate("decryWin", "RSA 加密"))
        self.label.setText(_translate("decryWin", "请输入要解密的密文："))
        self.label_3.setText(_translate("decryWin", "请输入d："))
        self.label_4.setText(_translate("decryWin", "请输入n："))
        self.label_5.setText(_translate("decryWin", "解密结果："))
        self.decry_bt.setText(_translate("decryWin", "解密"))
        self.import_bt.setText(_translate("decryWin", "从文本文件导入密文"))

