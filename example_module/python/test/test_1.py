import time
from valkka.example_module import ExampleThread

t = ExampleThread("test_thread")

t.startCall()

time.sleep(10)

t.stopCall()

