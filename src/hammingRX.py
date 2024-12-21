received_message = [1, 1, 1, 1, 1, 0, 0, 0, 1, 0]

#calcola la posizione dei bit di controllo
pos = []
for i in range(0, len(received_message)):
    if 2**i <= len(received_message):
        pos.append(2**i)
print(pos)

#rimuove i bit di controllo
received_message_cp = received_message.copy()
for i in sorted(pos, reverse=True):  
    received_message_cp.pop(i - 1)

print(received_message_cp, " messaggio senza bit di controllo")


#calcola posizione controlla dai bit di controllo
posToCheck = []
for j in pos:
    tmpPos = []
    for m in range(j, len(received_message) + 1):
        if j & m:
            tmpPos.append(m)
    posToCheck.append(tmpPos)
    print(posToCheck)

#calcola i bit di controllo
calcVal = []
for i in range(0, len(pos)):
    val = 0
    for j in posToCheck[i]:
        val = val ^ int(received_message[j-1])
    calcVal.append(val)

print(calcVal)
valErr = 0
for i,j in zip(calcVal, range(len(calcVal))):
    valErr += i * 2**j
print(valErr)

#corregge l'errore
print(received_message, " messaggio ricevuto")
if valErr != 0:
    if received_message[valErr-1] == 0:
        received_message[valErr-1] = 1
    else:
        received_message[valErr-1] = 0
print(received_message, " messaggio corretto")

#rimuove i bit di controllo
for i in sorted(pos, reverse=True):  
    received_message.pop(i - 1)
print(received_message, " messaggio senza bit di controllo")