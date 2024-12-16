#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <unitree/idl/hg/LowCmd_.hpp>

namespace py = pybind11;
#pragma pack(1)

typedef struct
{
	uint8_t mode; // desired working mode
	float q=0;	  // desired angle (unit: radian)
	float dq=0;	  // desired velocity (unit: radian/second)
	float tau=0;	  // desired output torque (unit: N.m)
	float Kp=0;	  // desired position stiffness (unit: N.m/rad )
	float Kd=0;	  // desired velocity stiffness (unit: N.m/(rad/s) )
	uint32_t reserve=0;
} MotorCmd; // motor control

typedef struct
{
	uint8_t mode_pr;
    uint8_t mode_machine;
	std::array<MotorCmd, 35> motor_cmd;
	std::array<uint32_t, 4> reserve;
	
	uint32_t crc;
} LowCmd;       


uint32_t crc32_core(uint32_t* ptr, uint32_t len)
{
    unsigned int xbit = 0;
    unsigned int data = 0;
    unsigned int CRC32 = 0xFFFFFFFF;
    const unsigned int dwPolynomial = 0x04c11db7;

    for (unsigned int i = 0; i < len; i++)
    {
        xbit = 1 << 31;
        data = ptr[i];
        for (unsigned int bits = 0; bits < 32; bits++)
        {
            if (CRC32 & 0x80000000)
            {
                CRC32 <<= 1;
                CRC32 ^= dwPolynomial;
            }
            else
            {
                CRC32 <<= 1;
            }

            if (data & xbit)
                CRC32 ^= dwPolynomial;
            xbit >>= 1;
        }
    }

    return CRC32;
}

uint32_t get_crc(py::object msg)
{
    LowCmd raw{};

    raw.mode_pr = py::cast<uint8_t>(msg.attr("mode_pr"));

    raw.mode_machine = py::cast<uint8_t>(msg.attr("mode_machine"));

    std::list<py::object> motor_cmds = py::cast<std::list<py::object>>(msg.attr("motor_cmd"));
    for (int i = 0; i < 35; i++)
    {
        py::object motor_cmd = motor_cmds.front();
        uint8_t mode = py::cast<uint8_t>(motor_cmd.attr("mode"));
        float q = py::cast<float>(motor_cmd.attr("q"));
        float dq = py::cast<float>(motor_cmd.attr("dq"));
        float tau = py::cast<float>(motor_cmd.attr("tau"));
        float Kp = py::cast<float>(motor_cmd.attr("kp"));
        float Kd = py::cast<float>(motor_cmd.attr("kd"));
        raw.motor_cmd[i].mode = mode;
        raw.motor_cmd[i].q = q;
        raw.motor_cmd[i].dq = dq;
        raw.motor_cmd[i].tau = tau;
        raw.motor_cmd[i].Kp = Kp;
        raw.motor_cmd[i].Kd = Kd;
        motor_cmds.pop_front();
    }
    std::array<uint32_t, 4> reserve = py::cast<std::array<uint32_t, 4>>(msg.attr("reserve"));
    memcpy(&raw.reserve, &reserve, 4);
      
    uint32_t crc = crc32_core((uint32_t *)&raw, (sizeof(LowCmd)>>2)-1);
    return crc;
}

PYBIND11_MODULE(crc_module, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("crc32_core", &crc32_core, "A function which calculates the crc32 of a given array of uint32_t");
    m.def("get_crc", &get_crc, "A function which calculates the crc32 of a given LowCmd_ message");
}