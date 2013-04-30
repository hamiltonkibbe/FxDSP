#!/usr/bin/env python

import scipy.signal as signal
from pylab import *


taps = signal.firwin(21, cutoff=0.3, window='hamming')
input = []
factor = (6*pi)/100
for i in range(100):
	input.append(sin(i*factor))
output = signal.lfilter(taps, 1.0, input)

