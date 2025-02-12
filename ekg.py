import scipy
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def main():
    ekg = scipy.datasets.electrocardiogram()[0:255]
    ekg = pd.Series(ekg)
    ekg = scipy.signal.savgol_filter(ekg, polyorder=4, window_length=40)
    
    min = np.min(ekg)
    ekg += np.abs(min)

    max = np.max(ekg)
    ekg /= max
    ekg *= 255
    
    print(len(ekg))
    
    ekg = np.array([35 if x < 35 else x for x in ekg])

    plt.plot(ekg)
    plt.show()

    np.savetxt('ekg.txt', ekg, fmt='%d', newline=',')
    
if __name__ == '__main__':
    main()
