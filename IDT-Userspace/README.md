# IDT-Userspace

IDT-Userspace contains IDT's RL model. The RL model autotunes the demotion policy according to the current execution context.

## Install

* Note that [**Linux Kernel**](https://github.com/scale-snu/IDT/tree/master/linux) needs to be installed. This [Wiki](https://github.com/scale-snu/IDT/wiki/Building-kernel-image) explains how to build the kernel from `linux/` source code.

* Install the dependencies of IDT-Userspace with the following:

  ```bash
  pip install -r requirements.txt
  ```

## Usage

1. Run the user-space RL model with the following command.

   ```bash
   python3 driver.py
   ```
   
2. In a separate shell, run the test script. Please check the `REAMDME.md` in the `experiments/` repository.

## Project Structure

The source code is explained in this [Wiki](https://github.com/scale-snu/IDT/wiki/IDT%E2%80%90Userspace-Code-Explained).
