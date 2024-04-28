from dataclasses import dataclass

@dataclass
class IDT_Config:
    # Number of memory nodes
    num_nodes:      int = 4

    # Watermark for enabling demotion
    demote_wmark:   int = 10

    # Wait seconds after applying action
    apply_wait:     int = 2

    # Time out seconds for waiting 
    wait_max:       int = 30
    
    # Wait seconds until there id demotion enabled node
    demotion_node_wait:     int = 1

    # Minimum value of action
    action_min:     int = 1
