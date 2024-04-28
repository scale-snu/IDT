import logging
import coloredlogs
import time
import os
import subprocess
import re

from datetime import datetime
from mem_stat import *


coloredlogs.install()
logging.basicConfig(
    format = '%(asctime)s [%(module)s:%(funcName)s]:%(levelname)s: %(message)s',
    datefmt = '%Y/%m/%d %H:%M:%S',
    level = logging.INFO
)


class ConfigNodes:
    """Config demotion path"""

    @staticmethod
    def __measure_node_latency(node):
        start_time = time.time()

        process = subprocess.run(['numactl', '--cpunodebind=0', '--membind={}'.format(node), 'python3', '_memory_access.py'], capture_output=True)
        if process.returncode != 0:
            logging.error("Memory access latency measure process dead")
            return None

        end_time = time.time()
        return end_time - start_time


    @staticmethod
    def __find_demotion_path(nodes):
        node_latency = {}

        for node in nodes:
            latency = ConfigNodes.__measure_node_latency(node)
            if latency is not None:
                node_latency[node] = latency

        sorted_nodes = sorted(node_latency.items(), key=lambda item: item[1])

        return [node for node, latency in sorted_nodes]


    @staticmethod
    def __get_available_nodes():
        process = subprocess.run(['numactl', '--hardware'], capture_output=True, text=True)

        if process.returncode != 0:
            return []

        output = process.stdout

        node_lines = re.findall(r'node \d+ cpus:', output)

        nodes = [int(re.search(r'\d+', line).group(0)) for line in node_lines]

        return nodes


    @staticmethod
    def set_demotion_path ():
        nodes = ConfigNodes.__get_available_nodes()
        path = ConfigNodes.__find_demotion_path(nodes)
        shifted_path = path[1:] + [-1]

        shifted_path_str = ' '.join(str(p) for p in shifted_path)
        if shifted_path != [1, 2, 3, -1]:
            logging.warning("Demotion path " + shifted_path_str)
        else:
            logging.info("Demotion path " + shifted_path_str)

        with open('/proc/idt_node', 'w') as f:
            f.write(shifted_path_str + "\n")


class Timer:
    """Timer used for an experiment"""

    def __init__ (self):
        pass


    def start (self):
        """Start timer"""

        self.start_t = datetime.now()


    def end (self):
        """End timer"""

        self.end_t = datetime.now()


    def print (self):
        """Print elapsed time"""

        self.delta_t = self.end_t - self.start_t
        logging.info(f"Elapsed time: {self.delta_t}\n")
