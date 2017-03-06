import numpy as np
import matplotlib.pyplot as plt

with open('output.txt','r') as infile:
	N=int(infile.readline().rstrip())
	x=[int(line.rstrip()) for line in infile]

x=np.array(x)
matriz=np.reshape(x,[N,N])

fig = plt.figure()
ax = fig.add_subplot(111)
ax.matshow(matriz)
#plt.show()
plt.title('Evolucion temporal')
plt.savefig('evolution.png')
