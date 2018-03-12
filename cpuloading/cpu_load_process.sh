#!/bin/bash
# cpu_load_process shell script
# variables
IP=127.0.0.1
load=0
dur=0
#functions
function usage()
{
  echo "cpu_load_process [[-ip] [-i] [-i]]"
  echo "  -ip: vm's ip address"
  echo "  -i: vm's cpu loading limit"
  echo "  -t: execution duration"
}
function do_cpu_load()
{
  echo "ssh root@$IP cpulimit -i $load dd if=/dev/zero of=/dev/null &"
  echo "for $dur secs."
  #ssh root@"$IP" cpulimit -i "$load" dd if=/dev/zero of=/dev/null &
}
# command line arguments
while [[ "$1" != "" ]]; do
  case "$1" in
    -ip )   shift
            IP=$1
            ;;
    -t )    shift
            if [[ $1 -lt 0 ]]; then
              echo "duration must greater than 0!"
              exit 1
            else
              dur=$1
            ;;
    -i )    shift
            if [[ $1 -lt 0 || $1 -gt 100 ]]; then
              echo "limit should be ranged between 100 and 0!"
              exit 1
            else
              load=$1
            ;;
    * )     usage
            exit 1
  esac
  shift
done

do_cpu_load;
