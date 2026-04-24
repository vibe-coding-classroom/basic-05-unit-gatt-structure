# GATT Structure Mapping

Please attach a screenshot of the nRF Connect app showing the GATT service tree here.

Expected Tree:
- ESP32-GATT-Lab (Device Name)
    - Drive Service (1234...5678)
        - Drive Control (Write)
    - Telemetry Service (8765...0987)
        - Telemetry Data (Notify + 2902 Descriptor)
        - Uptime (Read)
