import matplotlib.pyplot as plt

filename = 'data.txt'

data = [0 for i in range(2500)]
with open(filename, 'r') as fin:
    for i, s in enumerate(fin):
        data[i] = float(s)

plt.plot(data)
plt.show()
