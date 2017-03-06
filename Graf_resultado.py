import numpy as np
import matplotlib.pyplot as plt

with open('output.txt','r') as infile:
	N=int(infile.readline().rstrip())
	T=int(infile.readline().rstrip())
	x=[float(line.rstrip()) for line in infile]

x=np.array(x)
matriz=np.transpose(np.reshape(x,[T,N]))

fig = plt.figure()
ax = fig.add_subplot(111)
ax.matshow(matriz)
ax.set_xticks([]) 
ax.set_yticks([])
#plt.show()
plt.title('Evolucion temporal')
plt.savefig('evolution.png')
