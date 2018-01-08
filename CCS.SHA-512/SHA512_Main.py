# !usr/bin/env python
# -*- coding:utf-8 -*-

import sys
from SHA512_Util import *
from PyQt5.QtWidgets import *
from mainwin import *


class Mainwin(QWidget, Ui_Mainwin):
    def __init__(self):
        super(Mainwin, self).__init__()
        self.setupUi(self)
        self.setWindowTitle('SHA512加密程序          @Author: Libre')
        self.encry_bt.clicked.connect(self.encryBtClicked)
        self.import_bt.clicked.connect(self.importBtClicked)

    def encryBtClicked(self):
        msg = str(self.plain_textedit.toPlainText())
        if msg is '':
            self.plain_textedit.setText('请输入要加密的文本！')
            return

        self.ret_textedit.setText(sha512(msg))
        return

    def importBtClicked(self):
        fname = QFileDialog.getOpenFileName(self, '从文件中导入文本...', '.', '所有文件 (*);;文本文件 (*.txt)')
        data = None
        if fname[0]:
            f = open(fname[0], 'r')
            with f:
                data = f.read()
        self.plain_textedit.setText(data)


if __name__ == '__main__':
    QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling)
    app = QApplication(sys.argv)
    mainwin = Mainwin()
    mainwin.show()
    sys.exit(app.exec_())
