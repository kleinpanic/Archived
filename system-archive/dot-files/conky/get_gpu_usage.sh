#!/bin/bash
sudo intel_gpu_top -J -s 1 2>/dev/null | grep -oP '\"busy\": \K\d+' | head -1
