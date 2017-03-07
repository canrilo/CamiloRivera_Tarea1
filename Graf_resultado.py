import numpy as np
import matplotlib.pyplot as plt

with open('output.txt','r') as infile:
	N=int(infile.readline().rstrip())
	T=int(infile.readline().rstrip())
	x=[float(line.rstrip()) for line in infile]

with open('Energies.txt','r') as infile:
	Modes=int(infile.readline().rstrip())
	T2=int(infile.readline().rstrip())
	E=[float(line.rstrip()) for line in infile]

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

E=np.array(E)
Energias=np.transpose(np.reshape(E,[T2,Modes]))

ejex=np.linspace(1,T2,T2)
fig2 = plt.figure()
ax2 = fig2.add_subplot(111)
ax2.plot(ejex,Energias[0,:],'b',label='k1')
ax2.plot(ejex,Energias[1,:],'r',label='k2')
ax2.plot(ejex,Energias[2,:],'g',label='k3')
ax2.legend()
#plt.show()
plt.title('Energias Modos')
plt.savefig('energies.png')

