# echo-client.py

# ...

import socket
HOST = "127.0.0.1"
PORT = 2222

def strToBin(str):
    return ''.join(format(ord(i), '08b') for i in str)
def serialize(code,msg):
    binStr = bin(code).replace('b','').zfill(8)
    length = str(len(msg)).zfill(4)
    binStr += strToBin(length)
    binStr += strToBin(msg)
    binStr.replace('b','')
    return binStr
def BinaryToDecimal(binary):
    binary1 = binary
    decimal, i, n = 0, 0, 0
    while (binary != 0):
        dec = binary % 10
        decimal = decimal + dec * pow(2, i)
        binary = binary // 10
        i += 1
    return (decimal)

def BinTostr(bin_data):
    str_data = ''
    for i in range(0, len(bin_data), 8):
        temp_data = int(bin_data[i:i + 8])
        decimal_data = BinaryToDecimal(temp_data)
        str_data = str_data + chr(decimal_data)
    return str_data

def deserialize(data):
    code = BinaryToDecimal(int(data[0:8]))
    print("CODE:"+str(code))
    len = BinaryToDecimal(int(data[8:40]))
    data = BinTostr(data[40:40+len])
    return str(code) + str(len) + data
def menu():
    print("Choose a request:")
    print('1 - signup')
    print('2 - login')
    print('3 - get high scores')
    print()
def getRequest():
    code = 0
    msg = ""
    menu()
    choice = int(input(""))
    if choice == 2:
        username = input("Enter username: ")
        password = input("Enter password: ")
        code = 7
        msg = '{"username":"' + username + '","password":"' + password + '"}'
    if choice == 1:
        username = input("Enter username: ")
        password = input("Enter password: ")
        email = input("Enter email: ")
        msg = '{"username":"' + username + '","password":"' + password + '","email":"' + email + '"}'
        code = 6
    if choice == 3:
        msg = '{}'
        code = 14
    return (code,msg)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    userData = getRequest()
    s.sendall(serialize(userData[0],userData[1]).encode())
    data = (s.recv(1024).decode())
    print(deserialize(data))

    userData = getRequest()
    s.sendall(serialize(userData[0], userData[1]).encode())
    data = (s.recv(1024).decode())
    print(deserialize(data))

    s.close()
    print("done")
