length = 13665
print(length)
binval = list(bin(length)[2:])
binval.reverse()
print(binval)
i = 0       

while (2**i) < (len(binval) + i + 1):  # calcola il numero di bit di controllo necessari
    i += 1

print(i)

pos = [] # calcola la posizione dei bit ci controllo

for j in range(i):
    pos.append(2**j)

print(pos)

#calcola i valore di bit di controllo 
posToCheck = []
for j in pos:
    tmpPos = []
    for m in range(j, len(binval) + i + 1):
        if j & m:
            tmpPos.append(m)
    posToCheck.append(tmpPos)
print(posToCheck)


#inserisce i bit di controllo
for j in pos:
    binval.insert(j-1, 0) 
binval2 = binval.copy()
binval2.reverse()
print(binval2)

#calcola i bit di controllo
for j, k in zip(pos, range(i)):
    val = 0
    for m in posToCheck[k]:
        val = val ^ int(binval[m-1])
    print(val)
    binval[j-1] = val

binval.reverse()

for i in range(len(binval)):
    binval[i] = int(binval[i])
print(binval)