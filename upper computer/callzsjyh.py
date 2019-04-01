import sys
import math
import binascii

import serial
import serial.tools.list_ports
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from datetime import datetime
from matplotlib.dates import  date2num
from zsjyh import Ui_MainWindow


class mainWindow(QMainWindow, Ui_MainWindow):
    ser = serial.Serial()
    receiveCnt = 0
    sendCnt = 0
    DA = 0
    newData = []
    newTime = []
    launch = False
    #参数设置
    Tempvalue = 0
    timeValue1 = 0
    timeValue2 = 0
    def __init__(self, parent=None):
        super(mainWindow, self).__init__(parent)

        self.setupUi(self)
        #禁止拉伸窗口大小
        self.setFixedSize(self.width(), self.height())
                
        #信号与槽函数的连接
        self.initUi()

        #显示时间
        self.timerShow = QTimer(self)
        self.timerShow.timeout.connect(self.showTime)
        self.timerShow.start(1000) 
        #定时画图
        self.timerShow1 = QTimer(self)
        self.timerShow1.timeout.connect(self.Drawmap)
        self.timerShow1.start(1000) 
        #延时启动
        self.delautime = QTimer(self)
        self.delautime.setSingleShot(True)
        self.delautime.timeout.connect(self.Launchs)
        #延时停止
        self.destoptime = QTimer(self)
        self.destoptime.setSingleShot(True)
        self.destoptime.timeout.connect(self.Launchs)
               
        
        #自动获取当前可用串口
        self.serialCOMComboBox.addItems(self.serialList())
        
    def initUi(self):
        #端口设置：
        #设置默认参数
        self.serialBaudRateComboBox.setCurrentIndex(2)
        self.serialDataBitComboBox.setCurrentIndex(3)
        self.serialStopBitComboBox.setCurrentIndex(0)
        self.serialParityComboBox.setCurrentIndex(0)

        #连接打开串口按钮，并初始化可用
        self.openSerialButton.clicked.connect(self.serialOpen)
        self.openSerialButton.setEnabled(True)
        
        #连接关闭串口按钮，并初始化不可用
        self.closeSerialButton.clicked.connect(self.serialClose)
        self.closeSerialButton.setEnabled(False)
        
        #刷新当前可用串口，并刷新com列表显示
        self.refreshSerialButton.clicked.connect(self.serialRefresh)
        
        #时间设置：
        #LCD初始设置
        self.DelayedTimeDisplay.setProperty("value", 0.0)        
        #定时发送
        self.DelayedLaunchlineEdit.setText('0')
        self.DelayedLaunch.clicked.connect(self.DelayLaunch)
        
        #定时停止
        self.DelayedStoplineEdit.setText('0')
        self.DelayedStop.clicked.connect(self.DelayStop)
        
        #延时启动时间查询
        self.DelayedLaunchTimeRequest.clicked.connect(self.DelayLaunchTime)
        
        #延时停止时间查询
        self.DelayedStopTimeRequest.clicked.connect(self.DelayStopTime)

        #数模数据设置
        self.SetDALineEdit.setText('0')
        self.EnsureChangeDA.clicked.connect(self.DAchange)

        #温度设置：
        #LCD初始设置
        self.TempDisplay.setProperty("value", 0.0)
        #输入温度
        self.SetTempLineEdit.setText('0')
        #确定输入温度
        self.EnsureChangeTemp.clicked.connect(self.EnterTemp)
        #温度微调
        self.UpTemp1.clicked.connect(self.UpTemp)
        self.DownTemp1.clicked.connect(self.DownTemp)
        #启动停止
        self.Launch1.clicked.connect(self.Launchs)
        self.Stop_1.clicked.connect(self.Stop)
        #查询温度
        self.TempRequest.clicked.connect(self.TempReq)
        #状态查询
        self.StateRequest.clicked.connect(self.State)

    def sendData(self, ch):
        #有串口打开，才进行发送操作
        if self.ser.isOpen():
            if ch == 0:
                [Decimals, Interger] = math.modf(self.Tempvalue)
                Opera = chr(0xA0).encode()[1:2]
                IntergerC = chr(int(Interger)).encode()
                Decimals = round(Decimals, 2) * 100
                DecimalsC = chr(int(Decimals)).encode()
                
            elif ch == 1:
                Opera = chr(0xA1).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'
                
            elif ch == 2:
                Opera = chr(0xA2).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'
               
            elif ch == 3:
                Opera = chr(0xA3).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'
                
            elif ch == 4:
                Opera = chr(0xA4).encode()[1:2]
                [Decimals, Interger] = math.modf(self.timeValue1/60)
                IntergerC = chr(int(Interger)).encode()
                Decimals = round(Decimals, 2) * 100
                DecimalsC = hex(int(Decimals)).encode()
                
            elif ch == 5:
                Opera = chr(0xA5).encode()[1:2]
                [Decimals, Interger] = math.modf(self.timeValue1/60)
                IntergerC = chr(int(Interger)).encode()
                Decimals = round(Decimals, 2) * 100
                DecimalsC = chr(int(Decimals)).encode()

            elif ch == 6:
                Opera = chr(0xA6).encode()[1:2]
                IntergerC = self.DA
                if IntergerC > 127:
                    IntergerC = b'\xfe'
                else :
                    IntergerC = b'\x00'
                DecimalsC = chr(0x00).encode()
                self.launch = False

            elif ch == 8:
                Opera = chr(0xA8).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'

            elif ch == 10:
                Opera = chr(0xAA).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'

            elif ch == 11:
                Opera = chr(0xAB).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'
                
            elif ch == 14:
                Opera = chr(0xAE).encode()[1:2]
                IntergerC = b'\xff'
                DecimalsC = b'\xff'
            
            send_cout=6
            while(send_cout):
                #发送数据
                if send_cout == 6:
                    data = 'C'.encode()
                elif send_cout == 5:
                    data = 'O'.encode()
                elif send_cout == 4:
                    data = 'M'.encode()
                elif send_cout == 3:
                    data = Opera
                elif send_cout == 2:
                    data = IntergerC
                elif send_cout == 1:
                    data = DecimalsC
                send_cout-=1
                try:
                    self.ser.write(data)
                    self.debugLabel.setText("发送成功")
                except:
                    QMessageBox.critical(self, '错误提示','发送失败!!!\r\n没有可用的串口或当前串口被占用')
                    self.debugLabel.setText("发送失败")
                    return None
                
        else:
            QMessageBox.critical(self, '错误提示','发送失败!!!\r\n没有可用的串口或当前串口被占用')
            return None


    def Drawmap(self):
        if self.launch:
            self.sendData(8)
            receive = self.receiveData()
            if (receive[0] == 0xA8):
                Interger = int(receive[1],16)
                Decimals = int(receive[2],16)
                seconds = Interger + Decimals/100
                self.TempDisplay.setProperty("value", seconds)
                self.newData.append(seconds)
                newTimes = date2num(datetime.now())
                self.newTime.append(newTimes)
                self.widget.canvas.plot(self.newTime, self.newData)
            else :
                return None
        else:
            return None

        

    def DAchange(self):
        #数模转换
        if self.launch:
            if self.ser.isOpen():
                da1 = self.SetDALineEdit.text()
                try:
                    self.DA = int(da1, 16)
                except ValueError:
                    QMessageBox.critical(self, '错误提示','请输入有效的操作数！')
                    self.debugLabel.setText("请输入有效的操作数！")
                    return None
                if self.DA <= 0:
                    QMessageBox.critical(self, '错误提示','操作数必须大于0！')
                    self.debugLabel.setText("操作数必须大于0！")
                    return None
                self.sendData(6)
            else:
                self.debugLabel.setText("请打开串口！")
                return None
        else:
            self.debugLabel.setText("程序还未启动!")
            return None

    def UpTemp(self):
        self.sendData(1)
    
    def DownTemp(self):
        self.sendData(2)

    def TempReq(self):
        self.sendData(8)
        if True:#(self.receiveCnt == 1):
           
            receive = self.receiveData()
            if (receive[0] == 0xA8):
                Interger = int(receive[1],16)
                Decimals = int(receive[2],16)
                seconds = Interger + Decimals/100
                self.TempDisplay.setProperty("value", seconds)
                '''
                self.newData.append(seconds)
                newTimes = date2num(datetime.now())
                self.newTime.append(newTimes)
                self.widget.canvas.plot(self.newTime, self.newData)
                '''
            else:
                return None

        self.receiveCnt = 0

    
    def State(self):
        self.sendData(14)
        if True:#(self.receiveCnt == 1):
          
            receive = self.receiveData()
            if (receive[0] == 0xAE):
                sta = int(receive[1],16)
                if sta == 0xFF:
                    self.StateLabel.setText("启动")
                elif sta == 0x7F:
                    self.StateLabel.setText("停止")
            else:
                return None
          
        self.receiveCnt = 0
        




    def EnterTemp(self):
        #输入温度
        if not self.launch:
            if self.ser.isOpen():
                temp = self.SetTempLineEdit.text()
                try:
                    self.Tempvalue = int(temp, 10)
                except ValueError:
                    QMessageBox.critical(self, '错误提示','请输入有效的温度！')
                    self.debugLabel.setText("请输入有效的温度！")
                    return None
                if self.Tempvalue <= 0:
                    QMessageBox.critical(self, '错误提示','输入温度必须大于0！')
                    self.debugLabel.setText("输入度必须大于0！")
                    return None
                self.sendData(0)
            else:
                self.debugLabel.setText("请打开串口!")
        else:
            self.debugLabel.setText("请先关闭程序!")


    def DelayLaunchTime(self):
        #延时启动时间查询
        if self.delautime.isActive():
            seconds = self.delautime.remainingTime()
            self.DelayedTimeDisplay.setProperty("value", seconds)  
            
        else:
            seconds = 0
            self.DelayedTimeDisplay.setProperty("value", seconds)

    
    def DelayStopTime(self):
        #延时停止时间查询
        if self.destoptime.isActive():
            seconds = self.destoptime.remainingTime()
            self.DelayedTimeDisplay.setProperty("value", seconds)  
            
        else:
            seconds = 0
            self.DelayedTimeDisplay.setProperty("value", seconds)


    def DelayLaunch(self):
        #延时启动函数
        if not self.launch:
            if self.ser.isOpen():
                time1 = self.DelayedLaunchlineEdit.text()
                try:
                    self.timeValue1 = int(time1, 10)
                except ValueError:
                    QMessageBox.critical(self, '错误提示','请输入有效的定时时间！')
                    self.debugLabel.setText("请输入有效的定时时间！")
                    return None
                if self.timeValue1 <= 0:
                    QMessageBox.critical(self, '错误提示','定时时间必须大于0！')
                    self.debugLabel.setText("定时时间必须大于0！")
                    return None
                self.delautime.start(self.timeValue1)
            else:
                self.debugLabel.setText("请打开串口！")
                return None
        else:
            self.debugLabel.setText("程序已经开启!")
            return None

    def DelayStop(self):
        #延时停止函数
        if self.launch:
            if self.ser.isOpen():
                time2 = self.DelayedStoplineEdit.text()
                try:
                    self.timeValue2 = int(time2, 10)
                except ValueError:
                    QMessageBox.critical(self, '错误提示','请输入有效的定时时间！')
                    self.debugLabel.setText("请输入有效的定时时间！")
                    return None
                if self.timeValue2 <= 0:
                    QMessageBox.critical(self, '错误提示','定时时间必须大于0！')
                    self.debugLabel.setText("定时时间必须大于0！")
                    return None
                self.destoptime.start(self.timeValue2)
            else:
                self.debugLabel.setText("请打开串口！")
                return None
        else:
            self.debugLabel.setText("程序还未启动!")
            return None

    def Launchs(self):
        #启动程序函数
        if self.launch:
            self.debugLabel.setText("程序已经开始！")
            return None
        else:
            self.launch = True
            self.sendData(3)


    def Stop(self):
        #停止程序函数
        if not self.launch:
            self.debugLabel.setText("程序已经停止！")
            return None
        else:
            self.launch = False
            self.sendData(3)


    def showTime(self):
        #显示时间函数
        date = QDateTime.currentDateTime()
        date = date.toString("yyyy-MM-dd hh:mm:ss ddd")
        self.timeLabel.setText(date)


    #扫描获取端口号列表，并以列表形式返回
    def serialList(self):
        comList = []
        #获取当前可用串口信息
        portList = list(serial.tools.list_ports.comports())
        portList.sort()
        for port in portList:
            comList.append(port[0])
            '''
            #将串口信息格式重新调整，便于使用
            str = port[1]
            str = str[:-7]
            str = '%s:%s' % (port[0], str)
            #将调整后的串口信息加入列表中
            comList.append(str)
            #print(str)
            '''
        return comList
        
    def serialRefresh(self):
        self.serialCOMComboBox.clear()
        #自动获取当前可用串口
        self.serialCOMComboBox.addItems(self.serialList())
    
    def serialOpen(self):
        try:
            self.ser.port = self.serialCOMComboBox.currentText()
            self.ser.baudRate = self.serialBaudRateComboBox.currentText()
            self.ser.dataBits = self.serialDataBitComboBox.currentText()
            parityValue = self.serialParityComboBox.currentText()
            self.ser.parity = parityValue[0]
            self.ser.stopBits = int(self.serialStopBitComboBox.currentText())
            self.ser.timeout = 1.0
            self.ser.open()
        except:
            QMessageBox.critical(self, '错误提示','打开串口失败!!!\r\n没有可用的串口或当前串口被占用')
            return None
        
        if self.ser.isOpen():
            self.debugLabel.setText("串口打开成功")
            self.openSerialButton.setEnabled(False)
            self.closeSerialButton.setEnabled(True)
        else:
            self.debugLabel.setText("串口打开失败")
        
    def serialClose(self):
        self.receiveCnt = 0
        self.sendCnt = 0
        try:
            self.ser.close()
        except:
            QMessageBox.critical(self, '错误提示','关闭串口失败!!!')
            return None
        if self.ser.isOpen():
            self.debugLabel.setText("串口关闭失败")
        else:
            self.debugLabel.setText("串口关闭成功")
            self.openSerialButton.setEnabled(True)
            self.closeSerialButton.setEnabled(False)
        
            
    def receiveData(self):
        #串口接收数据
        self.debugLabel.setText("接收线程运行")
        try:
            self.ser.inWaiting()
        except:
            self.serialClose()
            self.debugLabel.setText("接收出错")
            QMessageBox.critical(self, '错误提示','串口被拔出')
            return None
        rece_cout = 6
        data=[]
        while(rece_cout):
            recedata = self.ser.read()
            if rece_cout == 6:
                if recedata == 'C'.encode():
                    rece_cout -= 1
                else:
                    rece_cout = 0
                    data=[]
                    data.append(0xFF)

            elif rece_cout == 5:
                if recedata == 'O'.encode():
                    rece_cout -= 1
                else:
                    rece_cout = 0
                    data=[]
                    data.append(0xFF)

            elif rece_cout == 4:
                if recedata == 'M'.encode():
                    rece_cout -= 1
                else:
                    rece_cout = 0
                    data=[]
                    data.append(0xFF)
            
            elif rece_cout == 3:
                recedata = binascii.b2a_hex(recedata)
                data.append(int(recedata.decode(),16))
                rece_cout -= 1

            elif rece_cout == 2:
                recedata = binascii.b2a_hex(recedata)
                data.append(recedata.decode())
                rece_cout -= 1

            elif rece_cout == 1:
                recedata = binascii.b2a_hex(recedata)
                data.append(recedata.decode())
                rece_cout -= 1
                self.receiveCnt = 1
        return data

            
        
           
    def closeEvent(self,event):
        result = QMessageBox.question(self,
                      "Confirm Exit...",
                      "Are you sure you want to exit ?",
                      QMessageBox.Yes| QMessageBox.No)
        event.ignore()

        if result == QMessageBox.Yes:
            event.accept()
            
        

        
if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = mainWindow()
    win.show()
    sys.exit(app.exec_())
