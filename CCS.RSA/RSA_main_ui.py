# !usr/bin/env python
# -*- coding:utf-8 -*-


import sys
from RSA_Utility import *
from PyQt5.QtWidgets import *
from mainwindow import *
from decrywin import *
from encrywin import *


class Mainwin(QWidget, Ui_rsa_mainwin):
    def __init__(self):
        super(Mainwin, self).__init__()
        self.setupUi(self)
        self.setWindowTitle('RSA加/解密程序          @Author: Libre')
        self.rsa_encry_bt.clicked.connect(self.encryBtClicked)
        self.rsa_decry_bt.clicked.connect(self.decryBtClicked)
        self.rsa_genkey_bt.clicked.connect(self.genkeyBtClicked)

    def encryBtClicked(self):
        self.opwin = EncryWin()
        self.opwin.show()

    def decryBtClicked(self):
        self.opwin = DecryWin()
        self.opwin.show()

    def genkeyBtClicked(self):
        bitnum = str(self.bitnum_comboBox.currentText())
        if bitnum == '请选择密钥位数':
            self.e_textedit.setText('请先选择密钥位数！')
            return
        bitnum =  int(bitnum) // 2      # str 2 int
        e, d, n, p, q = genKeys(bitnum)
        self.e_textedit.setText(str(e))
        self.d_textedit.setText(str(d))
        self.n_textedit.setText(str(n))
        self.p_textedit.setText(str(p))
        self.q_textedit.setText(str(q))
        return


class EncryWin(QWidget, Ui_encryWin):
    def __init__(self):
        super(EncryWin, self).__init__()
        self.setupUi(self)
        self.setWindowTitle('RSA加密          @Author: Libre')
        self.encry_bt.clicked.connect(self.encry)
        self.export_bt.clicked.connect(self.export_ret)

    def encry(self):
        e = str(self.e_textedit.toPlainText())
        if not e.isdigit():
            self.e_textedit.setText('请输入合法的e！')
            return
        e = int(e)

        n = str(self.n_textedit.toPlainText())
        if not n.isdigit():
            self.n_textedit.setText('请输入合法的n！')
            return
        n = int(n)

        ps = str(self.plain_textedit.toPlainText())
        if ps is '':
            self.plain_textedit.setText('请输入要加密的明文！')
            return

        ret = RSA_encry(ps, (e, n))     # str
        self.encry_ret_textedit.setText(ret)
        return

    def export_ret(self):
        ret = self.encry_ret_textedit.toPlainText()
        if ret is '':
            return
        savefile = QFileDialog.getSaveFileName(self, '将加密结果保存为...', '.', '所有文件 (*);;文本文件 (*.txt)')
        if savefile[0]:
            f = open(savefile[0], 'w')
            f.write(ret)
            f.close()
        return


class DecryWin(QWidget, Ui_decryWin):
    def __init__(self):
        super(DecryWin, self).__init__()
        self.setupUi(self)
        self.setWindowTitle('RSA解密          @Author: Libre')
        self.decry_bt.clicked.connect(self.decry)
        self.import_bt.clicked.connect(self.import_cipher)

    def decry(self):
        d = str(self.d_textedit.toPlainText())
        if not d.isdigit():
            self.d_textedit.setText('请输入合法的d！')
            return
        d = int(d)

        n = str(self.n_textedit.toPlainText())
        if not n.isdigit():
            self.n_textedit.setText('请输入合法的n！')
            return
        n = int(n)

        cs = str(self.cipher_textedit.toPlainText())
        if not cs.isdigit():
            self.cipher_textedit.setText('请输入合法的密文！')
            return

        ret = RSA_decry(cs, (d, n))     # str
        self.decry_ret_textedit.setText(ret)
        return

    def import_cipher(self):
        fname = QFileDialog.getOpenFileName(self, '从文本文件中导入密文...', '.', '所有文件 (*);;文本文件 (*.txt)')
        data = None
        if fname[0]:
            f = open(fname[0], 'r')
            with f:
                data = f.read()
        self.cipher_textedit.setText(data)
        return


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainwin = Mainwin()
    mainwin.show()
    sys.exit(app.exec_())