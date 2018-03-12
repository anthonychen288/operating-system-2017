#!/bin/bash
sar -u 1 > m &
ssh slave1 sar -u 1 > s1 &
ssh slave2 sar -u 1 > s2 &
