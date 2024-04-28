from utils import *


class MemStat:
    """Get node 0 memory statistics by parsing /sys/devices/system/node/node0/meminfo
    For fast memory, node 0 is used. Persistent memory uses node 2 and 3.
    Target workload should be configured to use node 0 memory.
    """

    mem_capacity = mem_left = mem_init = mem_used = pmem_used = 0
    meminfo_file = pmeminfo0_file = pmeminfo1_file = None

    @staticmethod
    def __init__ ():
        """Record initial memory"""
        try:
            MemStat.meminfo_file = open("/sys/devices/system/node/node0/meminfo", "r")
            MemStat.pmeminfo0_file = open("/sys/devices/system/node/node2/meminfo", "r")
            MemStat.pmeminfo1_file = open("/sys/devices/system/node/node3/meminfo", "r")
        except IOError:
            print("OS error occurred trying to open /sys/devices/system/node/node0, 2, 3/meminfo")
            exit()

        MemStat.get_mem_capacity()
        MemStat.mem_init = MemStat.get_mem_left()
                

    @staticmethod
    def get_mem_left ():
        """Get left space in node 0 fast memory.
        Obtained by reading 2nd line in /sys/devices/system/node/node0/meminfo.

        :return: Current left space in fast memory (node 0) in (MB)
        """

        if MemStat.meminfo_file is None:
            MemStat.__init__()

        MemStat.mem_left = int(MemStat.meminfo_file.readlines()[1].split()[3]) / 1000
        
        # set file_pos=start for next read
        MemStat.meminfo_file.seek(0) 

        return MemStat.mem_left

    
    @staticmethod
    def get_mem_capacity ():
        """Get node 0 fast memory space capacity.
        Obtained by reading 1st line in /sys/devices/system/node/node0/meminfo.

        :return: Capacity of fast memory (node 0) capacity in (MB)
        """

        if MemStat.meminfo_file is None:
            MemStat.__init__()

        MemStat.mem_capacity = int(MemStat.meminfo_file.readlines()[0].split()[3]) / 1000
        MemStat.pmem_capacity = int(MemStat.pmeminfo0_file.readlines()[0].split()[3]) / 1000
        
        # set file_pos=start for next read
        MemStat.meminfo_file.seek(0) 
        MemStat.pmeminfo0_file.seek(0) 


    @staticmethod
    def get_mem_used ():
        """Get memory usage.
        Obtained by reading 2nd line in /sys/devices/system/node/node0/meminfo.

        :return: Current used memory (node 0) in (MB)
        """

        if MemStat.mem_init == 0:
            MemStat.__init__()

        MemStat.mem_used = MemStat.mem_init - MemStat.mem_left
        
        return MemStat.mem_used


    @staticmethod
    def get_pmem_used ():
        """Get persistent memory usage.
        Obtained by reading 2nd line in /sys/devices/system/node/node2 & 3/meminfo.

        :return: Current used persistent memory (node 2 & 3) in (MB)
        """

        if (MemStat.pmeminfo0_file is None) or (MemStat.pmeminfo1_file is None):
            MemStat.__init__()

        pmem0_used = MemStat.pmem_capacity - int(MemStat.pmeminfo0_file.readlines()[1].split()[3]) / 1000
        pmem1_used = MemStat.pmem_capacity - int(MemStat.pmeminfo1_file.readlines()[1].split()[3]) / 1000
        MemStat.pmem_used = pmem0_used + pmem1_used
        
        # set file_pos=start for next read
        MemStat.pmeminfo0_file.seek(0) 
        MemStat.pmeminfo1_file.seek(0) 

        return MemStat.pmem_used

