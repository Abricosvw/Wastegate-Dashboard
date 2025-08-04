package com.ecuapp.dashboard.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.ecuapp.dashboard.data.model.ConnectionState
import com.ecuapp.dashboard.data.model.ConnectionType
import com.ecuapp.dashboard.network.ESP32Client
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class ConnectionViewModel : ViewModel() {
    private val esp32Client = ESP32Client()
    
    private val _connectionState = MutableStateFlow(ConnectionState())
    val connectionState: StateFlow<ConnectionState> = _connectionState.asStateFlow()
    
    private val _isConnecting = MutableStateFlow(false)
    val isConnecting: StateFlow<Boolean> = _isConnecting.asStateFlow()
    
    private val _errorMessage = MutableStateFlow<String?>(null)
    val errorMessage: StateFlow<String?> = _errorMessage.asStateFlow()
    
    fun connectWiFi(ipAddress: String, port: Int = 81) {
        _isConnecting.value = true
        _errorMessage.value = null
        
        // First test basic connection
        viewModelScope.launch {
            try {
                val isReachable = esp32Client.testConnection(ipAddress, 80)
                
                if (!isReachable) {
                    _errorMessage.value = "Cannot reach ESP32 at $ipAddress. Check WiFi connection."
                    _isConnecting.value = false
                    return@launch
                }
                
                // Try WebSocket connection
                esp32Client.connectWebSocket(
                    ipAddress = ipAddress,
                    port = port,
                    onMessage = { /* Handle in DashboardViewModel */ },
                    onError = { error ->
                        viewModelScope.launch {
                            _errorMessage.value = error
                            _isConnecting.value = false
                            _connectionState.value = _connectionState.value.copy(
                                status = "Error",
                                isConnected = false,
                                lastError = error
                            )
                        }
                    },
                    onConnected = {
                        viewModelScope.launch {
                            _isConnecting.value = false
                            _connectionState.value = _connectionState.value.copy(
                                type = ConnectionType.WIFI,
                                status = "Connected",
                                ipAddress = ipAddress,
                                port = port,
                                isConnected = true,
                                lastError = null
                            )
                        }
                    },
                    onDisconnected = {
                        viewModelScope.launch {
                            _connectionState.value = _connectionState.value.copy(
                                status = "Disconnected",
                                isConnected = false
                            )
                        }
                    }
                )
                
            } catch (e: Exception) {
                _errorMessage.value = "Connection failed: ${e.message}"
                _isConnecting.value = false
            }
        }
    }
    
    fun connectBluetooth(deviceAddress: String) {
        // TODO: Implement Bluetooth connection
        _errorMessage.value = "Bluetooth connection not yet implemented"
    }
    
    fun startDemoMode() {
        _connectionState.value = _connectionState.value.copy(
            type = ConnectionType.WIFI,
            status = "Demo Mode",
            deviceName = "Simulated ESP32",
            isConnected = true,
            lastError = null
        )
    }
    
    fun disconnect() {
        esp32Client.disconnect()
        _connectionState.value = _connectionState.value.copy(
            status = "Disconnected",
            isConnected = false
        )
    }
    
    fun clearError() {
        _errorMessage.value = null
    }
    
    override fun onCleared() {
        super.onCleared()
        esp32Client.disconnect()
    }
}