from crc_module import get_crc

from unitree_hg.msg import LowCmd

lowcmd = LowCmd()

for k in range(35):
    lowcmd.motor_cmd[k].mode = 1
    lowcmd.motor_cmd[k].q = float(k)
    lowcmd.motor_cmd[k].dq = float(k)
    lowcmd.motor_cmd[k].tau = float(k)
    lowcmd.motor_cmd[k].kp = float(k)
    lowcmd.motor_cmd[k].kd = float(k)

print(get_crc(lowcmd))
lowcmd.crc = get_crc(lowcmd)
