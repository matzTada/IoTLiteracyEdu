import serial
import time
import array
import sys

LED_HEADER = 'L'
ID_PACKET_OFFSET = '0'

def readSerial(ser): #can process the Escape mode API = 2
    var = ord(ser.read())
    if(var == 0x7D):
        var = ord(ser.read()) ^ 0x20
    return var

def chrWithEscape(value):
    var = chr(value)
    if(value == 0x7E or value == 0x7D or value == 0x11 or value == 0x13):
        var = chr(0x7D) + chr(value ^ 0x20)
    return var

def makeZigBeeTransmitRequestPacket(dst64addrH, dst64addrL, dst16addr, payLoad): #Frame type = 0x10 packet sender
    length = 1 + 1 + 8 + 2 + 1 + 1 + len(payLoad)

    frameData = chr(0x10) #frameType
    frameData += chr(0x01) #frameID

    sp8 = chr(dst64addrH % 256) #dst64addr High
    dst64addrH = dst64addrH / 256
    sp7 = chr(dst64addrH % 256)
    dst64addrH = dst64addrH / 256
    sp6 = chr(dst64addrH % 256) 
    dst64addrH = dst64addrH / 256
    sp5 = chr(dst64addrH % 256)

    sp12 = chr(dst64addrL % 256) #dst64addr Low
    dst64addrL = dst64addrL / 256
    sp11 = chr(dst64addrL % 256)
    dst64addrL = dst64addrL / 256
    sp10 = chr(dst64addrL % 256)
    dst64addrL = dst64addrL / 256
    sp9 = chr(dst64addrL % 256)

    frameData += sp5 + sp6 + sp7 + sp8 + sp9 + sp10 + sp11 + sp12

    frameData += chr((dst16addr / 256) % 256) #dst16addr
    frameData += chr(dst16addr % 256)

    frameData += chr(0x00) #Broadcast radius
    frameData += chr(0x00) #options

    frameData += payLoad #data payload

    checksum = 0 #checksum calculation
    for i in frameData:
        checksum += ord(i)
    checksum = 0xFF - ord(chr(checksum % 256))
    # print "checksum:", checksum

    tempPacket = chr((length / 256) % 256) + chr(length % 256) + frameData + chr(checksum)

    sendPacket = chr(0x7E)
    for i in tempPacket:
        sendPacket += chrWithEscape(ord(i))

    print "===> send", length, "bytes data. Data: ", payLoad
    # print "     hex:",
    # for i in sendPacket:
    #     print hex(ord(i)),
    # print ""

    return sendPacket

if __name__ == "__main__":
  #<=== Serial port initialization
  print "serial port initialization start"
  # port = '/dev/ttyUSB0' #XBee Explorer via USB that is for raspberry pi
  port = 'COM44' #XBee Explorer
  serialPort = serial.Serial(port, 9600, timeout = 1)
  print port + " is opend"
  time.sleep(2) #wait for establishing stable serial connection
  print "serial port initialization end"
  #===> Serial port initialization

  try:
    while True:
      #<=== packet receiving
      if serialPort.inWaiting() > 0: #if something is in serial port
        var = readSerial(serialPort)
        if(var == 0x7E): #and if this is the XBee packet

          frameData = []
          frameData.append(var)

          frameData.append(readSerial(serialPort))
          frameData.append(readSerial(serialPort))
          frameLength = frameData[1] * 256 + frameData[2]
          # print "<=== received hex:", hex(var), hex(frameData[1]), hex(frameData[2]),

          counter = 0
          checksumsum = 0
          while counter < frameLength + 1:
            var = readSerial(serialPort)
            frameData.append(var)
            checksumsum += var
            # print hex(var),
            counter += 1
            # print ""

          #print "frameLength:", frameLength,
          #print "frameData:", frameData
          #print "checksumsum:", hex(checksumsum)

          frameType = frameData[3]
          # print "frameType:", hex(frameType)

          if(frameType == 0x90): #ZigBee Receive Packet Response
            #print "<=== ZigBee Receive Packet: ",
            src64addrH = 0
            for i in range(0,4):
              src64addrH += frameData[i + 4] * pow(256,(3 - i))
              src64addrL = 0
            for i in range(0,4):
              src64addrL += frameData[i + 8] * pow(256,(3 - i))
              src16addr = frameData[12] * 256 + frameData[13]
            receiveOptions = frameData[14]
            receiveData = ""
            for i in range(15, frameLength + 3):
              receiveData += chr(frameData[i])
            # print "str(bytearray(receiveData)):", str(bytearray(receiveData))

            payloadType = receiveData[0]
            # print "payloadType: ", str(hex(payloadType)), "chr(payloadType): ", str(chr(payloadType))
            if payloadType == LED_HEADER and len(receiveData) > 11:
              tmp_id = int(ord(receiveData[1]) - ord(ID_PACKET_OFFSET))
              tmp_red = int(receiveData[2:5]) 
              tmp_green = int(receiveData[5:8]) 
              tmp_blue = int(receiveData[8:11]) 
              tmp_name = receiveData[11:len(receiveData)]
              
              # print "len:", frameLength, "data:", str(bytearray(receiveData)).strip()
              print  "id:", tmp_id, "tmp_red", tmp_red, "tmp_green", tmp_green, "tmp_blue", tmp_blue, "name:", tmp_name.strip()
      #===> packet receiving

      # #<=== broadcast packet sending
      # if not tmp_value == 0: 
      #   print "triger to send broadcast packet!!"
      #   cur = conn.cursor()
      #   cur.execute("UPDATE flagtest SET value=%s WHERE flagtest.name=%s", [0, "broadcastflag"])
      #   conn.commit()
      #   cur.close()

      #   broadcast_packet_str = "" + DOWNLINK_HEADER
      #   if tmp_value == 1: #sending Led packet
      #   	broadcast_packet_str += LED_INSTRUCTION
      #   	broadcast_packet_str += str(tmp_led)
      #   elif tmp_value == 2: #sending Servo packet
	     #    broadcast_packet_str += SERVO_INSTRUCTION
	     #    broadcast_packet_str += "{0:03d}".format(tmp_angle)

      #   print broadcast_packet_str
      #   temp = makeZigBeeTransmitRequestPacket(0x00000000, 0x0000FFFF, 0xFFFE, broadcast_packet_str)
      #   serialPort.write(temp)
      #   time.sleep(1)

      # #===> broadcast packet sending
      

  finally:
    serialPort.close()
    print port + " is closed."
    print "finish program"