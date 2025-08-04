package com.ecuapp.dashboard.data.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class CANData(
    val mapPressure: Int = 100,      // 100-250 kPa
    val wastegatePos: Int = 0,       // 0-100 %
    val tpsPosition: Int = 0,        // 0-100 %
    val engineRpm: Int = 0,          // 0-7000 RPM
    val targetBoost: Int = 100,      // 100-250 kPa
    val tcuStatus: Int = 0,          // 0=OK, 1=WARN, 2=ERROR
    val timestamp: Long = 0L
) : Parcelable

// CAN message structure for parsing
data class CANMessage(
    val id: Int,                     // CAN ID (0x200, 0x380, 0x440)
    val data: ByteArray,             // 8 bytes payload
    val dlc: Int = 8,                // Data Length Code
    val timestamp: Long = System.currentTimeMillis()
) {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as CANMessage

        if (id != other.id) return false
        if (!data.contentEquals(other.data)) return false
        if (dlc != other.dlc) return false

        return true
    }

    override fun hashCode(): Int {
        var result = id
        result = 31 * result + data.contentHashCode()
        result = 31 * result + dlc
        return result
    }
}

// Connection types
enum class ConnectionType {
    WIFI,
    BLUETOOTH
}

// Connection state
data class ConnectionState(
    val type: ConnectionType = ConnectionType.WIFI,
    val status: String = "Disconnected",
    val deviceName: String = "",
    val ipAddress: String = "192.168.4.1",
    val port: Int = 81,
    val isConnected: Boolean = false,
    val lastError: String? = null
)