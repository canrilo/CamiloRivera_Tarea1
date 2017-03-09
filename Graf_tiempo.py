import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

with open('Times.txt','r') as infile:
	#x=[[float(time) for time in times.rstrip('\n').split('\t')[1].rstrip('s').split('m')] for times in infile]
	x=[[float(time) for time in times.rstrip('\n').split('\t')] for times in infile]

x=np.matrix(x)
#y=x[:,0]*60+x[:,1]
fig = plt.figure()
ax = fig.add_subplot(111)

ax.plot(x[:,0],x[:,1],'s-')
z = np.array(x[:,0])
b=['1','2','4']
plt.xticks(z,b)
plt.ylabel('Tiempo (s)')
plt.xlabel('# Procs')
plt.title('Tiempos de Procesamiento')
plt.savefig('times.png')
#plt.show()
