from EnkripsiDES import *

def main():
     #    input plain text 64-bit
#    plainText = input()
#    plainText = secrets.token_hex(8)
     myChiperText = '59abc2c97cd4eb90'
     p = plainText = myChiperText
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