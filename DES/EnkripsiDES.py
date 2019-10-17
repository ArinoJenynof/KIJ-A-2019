import secrets
# import untuk kelas random generator dikhususkan cryptography

# untuk marking dan generate pengisi 4x4
hexa = {
        '0':0,
        '1':0,
        '2':0,
        '3':0,
        '4':0,
        '5':0,
        '6':0,
        '7':0,
        '8':0,
        '9':0,
        'a':0,
        'b':0,
        'c':0,
        'd':0,
        'e':0,
        'f':0
        }
# value sebagai urutan key row transposition
rowTranspositionKey = {
        0:0,
        1:3,
        2:1,
        3:2        
        }
listKeys = []


def marking(val):
    for key in hexa:
        if(key==val and hexa[val]==0):
            hexa[val]=1
            
def adding():
    for key in hexa:
        if(hexa[key]==0):
            hexa[key]=1
            return key

def mergingList(result,flag):
    stringList = ''
#    untuk permutasi key
    if(flag==16 or flag==14):
        box5648bit = []
        for i in range(flag):
            box5648bit.append(secrets.randbelow(16))
        for i in box5648bit:
            stringList += result[i]
#    untuk permutasi plaintext
    else:
        for i in result:
            stringList += i
    return stringList

def shiftLeft(result):
    leftSlice = result[0:1]
    rightSlice = result[1:]
    return rightSlice + leftSlice

def rowTranspositionCipher(rowTranspose,perm,n,m):
    for i in range(4):
        for j in range(4):
#            jika len kurang dari 16 disisipi hexadec lain
            if(m==n or m>n):
                rowTranspose[i][j] = adding()
            else:
                marking(perm[m])
                rowTranspose[i][j] = perm[m]
            m += 1

def permutation(perm,flag):
#    permutasi dengan row transposition #1
    result = []
    n = len(perm)
    m = 0
    rowTranspose = [['0' for x in range(4)] for y in range(4)]
    rowTranspositionCipher(rowTranspose,perm,n,m)
#    refresh marking
    global hexa
    hexa = dict.fromkeys(hexa, 0)
    for x in range(4):
        for y in range(4):
            result.append(rowTranspose[y][rowTranspositionKey[x]])    
#    permutasi dengan row transposition #2 cipherText 48-bit    
    if(flag==1):
        shiftedString = shiftLeft(mergingList(result,16))
        for i in range(16):
            permutation(shiftedString,2)
            shiftedString = shiftLeft(shiftedString)
#    simpan keys tiap round ke list
    else:
        listKeys.append(mergingList(result,14))

def initPermutation(perm, flag):
#    permutasi dengan row transposition
    result = []
    n = len(perm)
    m = 0
    rowTranspose = [['0' for x in range(4)] for y in range(4)]
    rowTranspositionCipher(rowTranspose,perm,n,m)
#    refresh marking
    global hexa
    hexa = dict.fromkeys(hexa, 0)
#    branching ke init atau ke invInit
#    init
    if(flag==1):
        for x in range(4):
            for y in range(4):
                result.append(rowTranspose[y][rowTranspositionKey[x]])
#    invInit
    else:
        for x in range(4):
            for y in list(rowTranspositionKey.values()):
                result.append(rowTranspose[3-y][x])
    return mergingList(result,0)

def swap32bit(perm):
    length = int(len(perm)/2)
    leftSlice = perm[0:length]
    rightSlice = perm[length:]
    return rightSlice+leftSlice

def slice32bit(perm):
    length = int(len(perm)/2)
    leftSlice = perm[0:length]
    rightSlice = perm[length:]
    return leftSlice, rightSlice

def vigenereAutokeyCipher(plain,initKey):
    vigenereAutoKey = initKey + plain[0:2]
    result = ''
    for i,j in zip(plain,vigenereAutoKey):
#        print('i',i,'j',j)
        result += format((int(i,16)+int(j,16))%16,'x')
#    print('resultVAC',result)
    return result
    
def xorOperation(a,b):
    result=''
    for x,y in zip(a,b):
#        print('x',x,'y',y)
        result += format(int(x,16) ^ int(y,16),'x')
#    print('resultXOR',result)
    return result
    
def rounding(left,right,key):
    temp = right
    F = vigenereAutokeyCipher(right,key)    # sebagai fungsi F subtitusi
    right = xorOperation(left,F)
    left = temp
    return left, right

def hamming(plain,cipher):
    binPlain = bin(int(plain,16))[2:]
    binCipher = bin(int(cipher,16))[2:]
    return sum(p!=c for p,c in zip(binPlain,binCipher))

def main():
#    input plain text 64-bit
#    plainText = input()
#    plainText = secrets.token_hex(8)
    p = plainText = 'a9b5c29cdc479b0e'
    print('plainText', plainText)
    if(len(plainText)!=16):
        print("plainText tidak berjumlah 64-bit")
        return
    
#    input cipher key 64-bit
#    cipherKey = input()
#    cipherKey = secrets.token_hex(8)
    cipherKey = '4adc96b6989ad28d'
    print('cipherKey',cipherKey)
    if(len(plainText)!=16):
        print("cipherKey tidak berjumlah 64-bit")
        return
        
#    precomputing cipherKey
    permutation(cipherKey,1)
#    print('listKeys',listKeys)
    
#    initial permutation
    plainText = initPermutation(plainText,1)
#    print('permutatedPlainText', plainText)
    
#    rounding encryption
    left ,right = slice32bit(plainText)
    for i in range(16):
#        print('left',left)
#        print('right',right)
#        print(i,left+right)
        left, right = rounding(left,right,listKeys[i])

#    swap 32 bit
    plainText = swap32bit(plainText)
#    print('swapped',plainText)

#    inverse initial permutation
    plainText = initPermutation(plainText,0)
    print('cipherText', plainText)
    
#    jarak hamming / beda bit total
    print('beda biner',hamming(p,plainText))

if __name__=='__main__':    
        main()