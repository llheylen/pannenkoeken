import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

# voor file die alle data bevat
def orbits_plotten_a(datafile):
    data = np.loadtxt(datafile)

    plt.figure()
    plt.title(datafile)
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.axis("equal")
    plt.grid()

    N = (len(data[0,:]) - 3) // 6 #tijd, energie en energiefout staan er ook in
    for i in range(N):
        plt.plot(data[:,6 * i + 1], data[:,6 * i + 2], label = "deeltje {}".format(i + 1))
    plt.legend()
    plt.show()

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
