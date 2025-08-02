package com.ecuapp.dashboard.network

import com.ecuapp.dashboard.data.model.CANData
import com.google.gson.Gson
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.flow.flowOn
import kotlinx.coroutines.delay
import okhttp3.*
import okio.ByteString
import java.util.concurrent.TimeUnit

class ESP32Client {
    private val gson = Gson()
    private var webSocket: WebSocket? = null
    private val client = OkHttpClient.Builder()
        .connectTimeout(10, TimeUnit.SECONDS)
        .readTimeout(10, TimeUnit.SECONDS)
        .writeTimeout(10, TimeUnit.SECONDS)
        .build()
    
    fun connectWebSocket(
        ipAddress: String,
        port: Int = 81,
        onMessage: (CANData) -> Unit,
        onError: (String) -> Unit,
        onConnected: () -> Unit,
        onDisconnected: () -> Unit
    ) {
        val request = Request.Builder()
            .url("ws://$ipAddress:$port/ws")
            .build()
        
        webSocket = client.newWebSocket(request, object : WebSocketListener() {
            override fun onOpen(webSocket: WebSocket, response: Response) {
                onConnected()
            }
            
            override fun onMessage(webSocket: WebSocket, text: String) {
                try {
                    val canData = parseCANData(text)
                    onMessage(canData)
                } catch (e: Exception) {
                    onError("Failed to parse CAN data: ${e.message}")
                }
            }
            
            override fun onFailure(webSocket: WebSocket, t: Throwable, response: Response?) {
                onError("WebSocket connection failed: ${t.message}")
                onDisconnected()
            }
            
            override fun onClosed(webSocket: WebSocket, code: Int, reason: String) {
                onDisconnected()
            }
        })
    }
    
    private fun parseCANData(json: String): CANData {
        val jsonObject = gson.fromJson(json, Map::class.java)
        
        return CANData(
            mapPressure = (jsonObject["map_pressure"] as? Double)?.toInt() ?: 100,
            wastegatePos = (jsonObject["wastegate_pos"] as? Double)?.toInt() ?: 0,
            tpsPosition = (jsonObject["tps_position"] as? Double)?.toInt() ?: 0,
            engineRpm = (jsonObject["engine_rpm"] as? Double)?.toInt() ?: 0,
            targetBoost = (jsonObject["target_boost"] as? Double)?.toInt() ?: 100,
            tcuStatus = (jsonObject["tcu_status"] as? Double)?.toInt() ?: 0,
            timestamp = System.currentTimeMillis()
        )
    }
    
    // HTTP GET for testing connection
    suspend fun testConnection(ipAddress: String, port: Int = 80): Boolean {
        return try {
            val request = Request.Builder()
                .url("http://$ipAddress:$port/")
                .build()
            
            val response = client.newCall(request).execute()
            response.isSuccessful
        } catch (e: Exception) {
            false
        }
    }
    
    // Send command to ESP32
    fun sendCommand(command: String): Boolean {
        return webSocket?.send(command) ?: false
    }
    
    fun disconnect() {
        webSocket?.close(1000, "User disconnect")
        webSocket = null
    }
    
    // Simulate data for testing without ESP32
    fun getSimulatedData(): Flow<CANData> = flow {
        while (true) {
            val data = CANData(
                mapPressure = (100..250).random(),
                wastegatePos = (0..100).random(),
                tpsPosition = (0..100).random(),
                engineRpm = (0..7000).random(),
                targetBoost = (100..250).random(),
                tcuStatus = (0..2).random(),
                timestamp = System.currentTimeMillis()
            )
            emit(data)
            delay(100) // 10Hz update rate
        }
    }.flowOn(Dispatchers.IO)
}