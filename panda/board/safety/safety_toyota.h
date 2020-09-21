// global torque limit
const int TOYOTA_MAX_TORQUE = 1500;       // max torque cmd allowed ever

// rate based torque limit + stay within actually applied
// packet is sent at 100hz, so this limit is 1000/sec
const int TOYOTA_MAX_RATE_UP = 10;        // ramp up slow
const int TOYOTA_MAX_RATE_DOWN = 25;      // ramp down fast
const int TOYOTA_MAX_TORQUE_ERROR = 350;  // max torque cmd in excess of torque motor

// real time torque limit to prevent controls spamming
// the real time limit is 1500/sec
const int TOYOTA_MAX_RT_DELTA = 375;      // max delta torque allowed for real time checks
const uint32_t TOYOTA_RT_INTERVAL = 250000;    // 250ms between real time checks

// longitudinal limits
const int TOYOTA_MAX_ACCEL = 1500;        // 1.5 m/s2
const int TOYOTA_MIN_ACCEL = -3000;       // -3.0 m/s2

const int TOYOTA_ISO_MAX_ACCEL = 2000;        // 2.0 m/s2
const int TOYOTA_ISO_MIN_ACCEL = -3500;       // -3.5 m/s2

const int TOYOTA_STANDSTILL_THRSLD = 100;  // 1kph

// Roughly calculated using the offsets in openpilot +5%:
// In openpilot: ((gas1_norm + gas2_norm)/2) > 15
// gas_norm1 = ((gain_dbc*gas1) + offset1_dbc)
// gas_norm2 = ((gain_dbc*gas2) + offset2_dbc)
// In this safety: ((gas1 + gas2)/2) > THRESHOLD
const int TOYOTA_GAS_INTERCEPTOR_THRSLD = 845;
#define TOYOTA_GET_INTERCEPTOR(msg) (((GET_BYTE((msg), 0) << 8) + GET_BYTE((msg), 1) + (GET_BYTE((msg), 2) << 8) + GET_BYTE((msg), 3)) / 2) // avg between 2 tracks

const CanMsg TOYOTA_TX_MSGS[] = {{0x283, 0, 7}, {0x2E6, 0, 8}, {0x2E7, 0, 8}, {0x33E, 0, 7}, {0x344, 0, 8}, {0x365, 0, 7}, {0x366, 0, 7}, {0x4CB, 0, 8},  // DSU bus 0
                                  {0x128, 1, 6}, {0x141, 1, 4}, {0x160, 1, 8}, {0x161, 1, 7}, {0x470, 1, 4},  // DSU bus 1
                                  {0x2E4, 0, 5}, {0x411, 0, 8}, {0x412, 0, 8}, {0x343, 0, 8}, {0x1D2, 0, 8},  // LKAS + ACC
                                  {0x200, 0, 6}};  // interceptor

AddrCheckStruct toyota_rx_checks[] = {
  {.msg = {{ 0xaa, 0, 8, .check_checksum = false, .expected_timestep = 12000U}}},
  {.msg = {{0x260, 0, 8, .check_checksum = true, .expected_timestep = 20000U}}},
  {.msg = {{0x1D2, 0, 8, .check_checksum = true, .expected_timestep = 30000U}}},
  {.msg = {{0x224, 0, 8, .check_checksum = false, .expected_timestep = 25000U},
           {0x226, 0, 8, .check_checksum = false, .expected_timestep = 25000U}}},
};
const int TOYOTA_RX_CHECKS_LEN = sizeof(toyota_rx_checks) / sizeof(toyota_rx_checks[0]);

// global actuation limit states
int toyota_dbc_eps_torque_factor = 100;   // conversion factor for STEER_TORQUE_EPS in %: see dbc file

static int toyota_rx_hook(CAN_FIFOMailBox_TypeDef *to_push) {
  UNUSED(to_push);
  return true;
}

static int toyota_tx_hook(CAN_FIFOMailBox_TypeDef *to_send) {
  UNUSED(to_send);
  return true;
}

static void toyota_init(int16_t param) {
  controls_allowed = 1;
  relay_malfunction_reset();
  gas_interceptor_detected = 0;
  toyota_dbc_eps_torque_factor = param;
}

static int toyota_fwd_hook(int bus_num, CAN_FIFOMailBox_TypeDef *to_fwd) {

  int bus_fwd = -1;
  if (!relay_malfunction) {
    if (bus_num == 0) {
      bus_fwd = 2;
    }
    if (bus_num == 2) {
      int addr = GET_ADDR(to_fwd);
      // block stock lkas messages and stock acc messages (if OP is doing ACC)
      // in TSS2, 0x191 is LTA which we need to block to avoid controls collision
      int is_lkas_msg = ((addr == 0x2E4) || (addr == 0x412) || (addr == 0x191));
      // in TSS2 the camera does ACC as well, so filter 0x343
      int is_acc_msg = (addr == 0x343);
      int block_msg = is_lkas_msg || is_acc_msg;
      if (!block_msg) {
        bus_fwd = 0;
      }
    }
  }
  return bus_fwd;
}

const safety_hooks toyota_hooks = {
  .init = toyota_init,
  .rx = toyota_rx_hook,
  .tx = toyota_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = toyota_fwd_hook,
  .addr_check = toyota_rx_checks,
  .addr_check_len = sizeof(toyota_rx_checks)/sizeof(toyota_rx_checks[0]),
};
