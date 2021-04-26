import time
import numpy as np
from valkka.example_module.core import ExampleThread, MyFrameFilter, testnumpy

print("testing numpy")
a = np.zeros(100, dtype=np.uint8)
t = testnumpy(a)
print("numpy didn't crash - it's your lucky day")

from valkka import core
print("testing numpy - again")
a = np.zeros(100, dtype=np.uint8)
t = testnumpy(a)
print("numpy didn't crash - it's your lucky day")

print("testing thread")
t = ExampleThread("example")
t.startCall()
time.sleep(3)
t.stopCall()
print("bye!")
