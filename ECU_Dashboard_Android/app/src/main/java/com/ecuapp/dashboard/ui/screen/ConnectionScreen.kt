package com.ecuapp.dashboard.ui.screen

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Bluetooth
import androidx.compose.material.icons.filled.Wifi
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.ecuapp.dashboard.data.model.ConnectionType
import com.ecuapp.dashboard.viewmodel.ConnectionViewModel

@Composable
fun ConnectionScreen(
    onNavigateToDashboard: () -> Unit,
    viewModel: ConnectionViewModel = viewModel()
) {
    val connectionState by viewModel.connectionState.collectAsState()
    val isConnecting by viewModel.isConnecting.collectAsState()
    val errorMessage by viewModel.errorMessage.collectAsState()
    
    var ipAddress by remember { mutableStateOf("192.168.4.1") }
    var port by remember { mutableStateOf("81") }
    var selectedConnectionType by remember { mutableStateOf(ConnectionType.WIFI) }
    
    // Navigate to dashboard when connected
    LaunchedEffect(connectionState.isConnected) {
        if (connectionState.isConnected) {
            onNavigateToDashboard()
        }
    }
    
    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(Color(0xFF0A0A0A))
            .padding(24.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        // Title
        Text(
            text = "ECU Dashboard",
            color = Color.White,
            fontSize = 32.sp,
            fontWeight = FontWeight.Bold
        )
        
        Text(
            text = "Connect to ESP32 CAN Interface",
            color = Color.Gray,
            fontSize = 16.sp,
            modifier = Modifier.padding(bottom = 32.dp)
        )
        
        // Connection Type Selection
        Card(
            modifier = Modifier.fillMaxWidth(),
            colors = CardDefaults.cardColors(
                containerColor = Color(0xFF2A2A2A)
            ),
            shape = RoundedCornerShape(16.dp)
        ) {
            Column(
                modifier = Modifier.padding(16.dp)
            ) {
                Text(
                    text = "Connection Type",
                    color = Color.White,
                    fontWeight = FontWeight.Medium,
                    modifier = Modifier.padding(bottom = 16.dp)
                )
                
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceEvenly
                ) {
                    // WiFi Option
                    FilterChip(
                        onClick = { selectedConnectionType = ConnectionType.WIFI },
                        label = { 
                            Row(verticalAlignment = Alignment.CenterVertically) {
                                Icon(
                                    Icons.Default.Wifi,
                                    contentDescription = null,
                                    modifier = Modifier.size(18.dp)
                                )
                                Spacer(modifier = Modifier.width(8.dp))
                                Text("WiFi")
                            }
                        },
                        selected = selectedConnectionType == ConnectionType.WIFI,
                        colors = FilterChipDefaults.filterChipColors(
                            selectedContainerColor = Color(0xFF00D4FF),
                            selectedLabelColor = Color.Black
                        )
                    )
                    
                    // Bluetooth Option (disabled for now)
                    FilterChip(
                        onClick = { /* TODO: Implement Bluetooth */ },
                        label = { 
                            Row(verticalAlignment = Alignment.CenterVertically) {
                                Icon(
                                    Icons.Default.Bluetooth,
                                    contentDescription = null,
                                    modifier = Modifier.size(18.dp)
                                )
                                Spacer(modifier = Modifier.width(8.dp))
                                Text("Bluetooth")
                            }
                        },
                        selected = selectedConnectionType == ConnectionType.BLUETOOTH,
                        enabled = false,
                        colors = FilterChipDefaults.filterChipColors(
                            disabledContainerColor = Color(0xFF404040),
                            disabledLabelColor = Color.Gray
                        )
                    )
                }
            }
        }
        
        Spacer(modifier = Modifier.height(24.dp))
        
        // WiFi Configuration
        if (selectedConnectionType == ConnectionType.WIFI) {
            Card(
                modifier = Modifier.fillMaxWidth(),
                colors = CardDefaults.cardColors(
                    containerColor = Color(0xFF2A2A2A)
                ),
                shape = RoundedCornerShape(16.dp)
            ) {
                Column(
                    modifier = Modifier.padding(16.dp)
                ) {
                    Text(
                        text = "ESP32 WiFi Settings",
                        color = Color.White,
                        fontWeight = FontWeight.Medium,
                        modifier = Modifier.padding(bottom = 16.dp)
                    )
                    
                    OutlinedTextField(
                        value = ipAddress,
                        onValueChange = { ipAddress = it },
                        label = { Text("IP Address", color = Color.Gray) },
                        colors = OutlinedTextFieldDefaults.colors(
                            focusedTextColor = Color.White,
                            unfocusedTextColor = Color.White,
                            focusedBorderColor = Color(0xFF00D4FF),
                            unfocusedBorderColor = Color.Gray
                        ),
                        modifier = Modifier.fillMaxWidth()
                    )
                    
                    Spacer(modifier = Modifier.height(12.dp))
                    
                    OutlinedTextField(
                        value = port,
                        onValueChange = { port = it },
                        label = { Text("Port", color = Color.Gray) },
                        keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                        colors = OutlinedTextFieldDefaults.colors(
                            focusedTextColor = Color.White,
                            unfocusedTextColor = Color.White,
                            focusedBorderColor = Color(0xFF00D4FF),
                            unfocusedBorderColor = Color.Gray
                        ),
                        modifier = Modifier.fillMaxWidth()
                    )
                }
            }
        }
        
        Spacer(modifier = Modifier.height(24.dp))
        
        // Error Message
        errorMessage?.let { error ->
            Card(
                modifier = Modifier.fillMaxWidth(),
                colors = CardDefaults.cardColors(
                    containerColor = Color(0xFF4A2626)
                ),
                shape = RoundedCornerShape(8.dp)
            ) {
                Text(
                    text = error,
                    color = Color(0xFFFF6B6B),
                    modifier = Modifier.padding(16.dp)
                )
            }
            
            Spacer(modifier = Modifier.height(16.dp))
        }
        
        // Connect Button
        Button(
            onClick = {
                when (selectedConnectionType) {
                    ConnectionType.WIFI -> {
                        viewModel.connectWiFi(ipAddress, port.toIntOrNull() ?: 81)
                    }
                    ConnectionType.BLUETOOTH -> {
                        // TODO: Implement Bluetooth connection
                    }
                }
            },
            enabled = !isConnecting,
            colors = ButtonDefaults.buttonColors(
                containerColor = Color(0xFF00D4FF),
                contentColor = Color.Black
            ),
            shape = RoundedCornerShape(12.dp),
            modifier = Modifier
                .fillMaxWidth()
                .height(56.dp)
        ) {
            if (isConnecting) {
                CircularProgressIndicator(
                    modifier = Modifier.size(24.dp),
                    color = Color.Black
                )
            } else {
                Text(
                    text = "Connect",
                    fontSize = 18.sp,
                    fontWeight = FontWeight.Bold
                )
            }
        }
        
        Spacer(modifier = Modifier.height(16.dp))
        
        // Demo Mode Button
        OutlinedButton(
            onClick = {
                viewModel.startDemoMode()
                onNavigateToDashboard()
            },
            colors = ButtonDefaults.outlinedButtonColors(
                contentColor = Color.White
            ),
            shape = RoundedCornerShape(12.dp),
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Demo Mode (No ESP32)")
        }
    }
}