package com.ecuapp.dashboard.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.ecuapp.dashboard.data.model.CANData
import com.ecuapp.dashboard.network.ESP32Client
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class DashboardViewModel : ViewModel() {
    private val esp32Client = ESP32Client()
    
    private val _canData = MutableStateFlow(CANData())
    val canData: StateFlow<CANData> = _canData.asStateFlow()
    
    private val _connectionStatus = MutableStateFlow("Disconnected")
    val connectionStatus: StateFlow<String> = _connectionStatus.asStateFlow()
    
    private val _lastUpdate = MutableStateFlow(0L)
    val lastUpdate: StateFlow<Long> = _lastUpdate.asStateFlow()
    
    private val _errorMessage = MutableStateFlow<String?>(null)
    val errorMessage: StateFlow<String?> = _errorMessage.asStateFlow()
    
    fun connectToESP32(ipAddress: String, port: Int = 81) {
        _connectionStatus.value = "Connecting..."
        
        esp32Client.connectWebSocket(
            ipAddress = ipAddress,
            port = port,
            onMessage = { canData ->
                viewModelScope.launch {
                    _canData.value = canData
                    _lastUpdate.value = canData.timestamp
                }
            },
            onError = { error ->
                viewModelScope.launch {
                    _errorMessage.value = error
                    _connectionStatus.value = "Error"
                }
            },
            onConnected = {
                viewModelScope.launch {
                    _connectionStatus.value = "Connected"
                    _errorMessage.value = null
                }
            },
            onDisconnected = {
                viewModelScope.launch {
                    _connectionStatus.value = "Disconnected"
                }
            }
        )
    }
    
    fun startSimulation() {
        _connectionStatus.value = "Simulating"
        
        viewModelScope.launch {
            esp32Client.getSimulatedData().collect { canData ->
                _canData.value = canData
                _lastUpdate.value = canData.timestamp
            }
        }
    }
    
    fun disconnect() {
        esp32Client.disconnect()
        _connectionStatus.value = "Disconnected"
    }
    
    fun sendCommand(command: String): Boolean {
        return esp32Client.sendCommand(command)
    }
    
    fun clearError() {
        _errorMessage.value = null
    }
    
    override fun onCleared() {
        super.onCleared()
        esp32Client.disconnect()
    }
}