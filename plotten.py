import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

# voor file die alle data bevat
# ik vind het iets handiger/overzichtelijker om eerst alle data in een 'spacetime' lijst te hebben in de vorm 
# [t,(x1,y1,z1),...(xN,yN,zN)] zodat je nadien gewoon over deze lijst kan loopen, en gemakkelijk eender welk tijdstip eruit pikken enz.
# deze aanpak is ook cruciaal om 'lijnen'(hoe orbits meestal voorgesteld worden) te plotten (aangezien je dan alle data voor het 
# instantane tijdstip 't' ook moet kunnen plotten
def orbits_plotten_a(methode='RK4_1'):
    data = np.loadtxt('output_{}.txt'.format(RK4_1)) #er van uitgaande dat we onze outpilenames standaardiseren naar 'output_methode.txt'
    #Bepaal aantal bodies = lengte van regel -1, gedeeld door 3+3+2=8 (tijd, energie en energiefout staan er ook in)
    #PS: ik laat de snelheid erin, aangezien dit in de verre toekomst te pas kan komen om richtingsvectoren te visualiseren
    N=int((len(data[0])-1)/8)
    time=0
    spacetime=[]
    
   #write 3N+1 configuration spacetime
    for i in data[0:,]:
        space=[time]
        for n in range(N):
            x,y,z=i[n*8+1],i[n*8+2],i[n*8+3]
            space.append((x,y,z))
        spacetime.append(space)
        time+=1
    
    #making the figures, nog niet esthetisch in orde natuurlijk.
    for point in spacetime:
        x,y,z=[i[0] for i in point[1:]],[i[1] for i in point[1:]],[i[2] for i in point[1:]]
        print(x)
        fig=plt.figure()
        plt.title(methode)
        ax=fig.add_subplot(111,projection='3d')
        ax.set_xlabel('x'),ax.set_ylabel('y'),ax.set_zlabel('z')
        ax.set_xlim(0,4),ax.set_ylim(0,4),ax.set_zlim(0,4)
        if vectoren=='V':
            ax.quiver(0,0,0,x,y,z)
            ax.scatter(x,y,z, s=200, marker='*')
        elif vectoren=='*':
            ax.scatter(x,y,z, s=200, marker='*')
        elif vectoren=='.':
            ax.scatter(x,y,z, s=100, marker='o')
        #hier komt weldra de methode om effectief banen te plotten (dit zijn gewoon discrete punten)
        plt.savefig('tijd{}.png'.format(time))

    '''
    ik doe dit nog niet weg, aangezien ik mijn deeltjes nog niet label ;-p
    for i in range(N):
        plt.plot(data[:,6 * i + 1], data[:,6 * i + 2], label = "deeltje {}".format(i + 1))
    plt.legend()
    '''
#voor file die enkel de posities bevat
def orbits_plotten_b(datafile):
    data = np.loadtxt(datafile)

    plt.figure()
    plt.title(datafile)
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.axis("equal")
    plt.grid()

    N = (len(data[0,:]) - 1) // 3 #tijd, energie en energiefout staan er ook in
    for i in range(N):
        plt.plot(data[:,3 * i + 1], data[:,3 * i + 2], label = "deeltje {}".format(i + 1))
    plt.legend()
    plt.show()

#orbit plotten RK4
orbits_plotten_a("output_RK4_1.txt")
orbits_plotten_a("output_RK4_2.txt")
orbits_plotten_b("posities_ERK.txt")
