from matplotlib import pyplot as plt
import numpy as np
from mpl_toolkits import mplot3d
#from mpl_toolkits import Axes3D
#gets the data in a matrix
def get_data_points2(file_name):
    f = open(file_name,"r")
    DB = []
    for x in f:
        y = x.split(", ")
        y= (int(y[i]) for i in range(len(y)))
        DB.append(tuple(y))
    f.close()
    return DB
#second gets the data in a matrix
def get_data_points(file_name):
    with open(file_name, 'r') as f:
        data = f.read()
    rows = data.strip().split('\n')
    rows = [list(map(float, row.split(','))) for row in rows]
    tuples_data = [tuple(row) for row in rows]
    return tuples_data


#creates an output file
def output_file(file_name,mu):
    f = open(file_name, "a")
    for i in range(len(mu)):
        f.write(mu[i])
    f.close()
#euclidean distance
def vecadd(t1, t2, alpha=1):#tuple vector addition
    assert len(t1)==len(t2),"vectors must be of the same length"
    return tuple([t1[i]+alpha*t2[i] for i in range(len(t1))])
def dist(p, q):
    sum = 0
    for i in range(len(p)):
        sum+= (p[i]-q[i])**2
    return sum**0.5
def argmin(l):
    return [i for i in range(len(l)) if l[i]==min(l)]
def delta(mu, epsilon):
    for i in range(len(mu)):
        if(dist(mu[i],mu[i-1])>=epsilon):
            return False
    return True
def k_means(K, iter=200, input_data=""):
    N = len(input_data)
    x = input_data #rename
    mu = [x[k] for k in range(K)]
    epsilon = 0.001
    iteration_number = 0
    assignment = []
    while(delta(mu, epsilon) or iteration_number<iter):
        assignment = [] #assign to every x the closest cluster
        for x_i in x:
            assignment.append(argmin([dist(x_i,mu[k]) for k in range(K)])[0])#the index of the closest cluster
        for k in range(K):
            mu[k] = vecadd(mu[k],mu[k],-1)
            num = 0#cluster size
            for i in range(N):
                if(assignment[i] == k):#x_i belongs to cluster k
                    num = num +1
                    mu[k] = vecadd(mu[k], input_data[i])
            mu[k] = vecadd(vecadd(mu[k],mu[k],-1),mu[k],1/num)
        iteration_number = iteration_number + 1
    return mu
    
def main():
    for y in range(4):
        # Create a 3D plot
        DB = get_data_points("tests/input_1.txt")
        print(k_means(3, y, DB))
        mat = np.array([np.array(DB[i]) for i in range(len(DB))])
        mu = np.array([np.array(k_means(3, y, DB)[i]) for i in range(len(k_means(3, 200, DB)))])
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.plot(mat[:,0],mat[:,1], mat[:,2], 'o', markersize=1)
        ax.plot(mu[:,0],mu[:,1], mu[:,2], 'o', markersize=1)
        ax.set_xlabel('X Axis')
        ax.set_ylabel('Y Axis')
        ax.set_zlabel('Z Axis')
        plt.savefig('output'+str(y)+'.png')
        plt.show()
        """ax = plt.axes(projection ='3d')
        ax.plot3D(mat[:,0],mat[:,1], mat[:,2],'o', color='red', markersize=2)
        ax.plot3D(mu[:,0],mu[:,1],mu[:,2],'o',color='green',markersize=2)
        ax.show()
        print(k_means(3, 200, DB))"""
if __name__== "__main__":
    main()