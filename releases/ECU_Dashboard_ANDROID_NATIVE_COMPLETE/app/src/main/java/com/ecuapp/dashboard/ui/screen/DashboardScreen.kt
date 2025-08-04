package com.ecuapp.dashboard.ui.screen

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material.icons.filled.WifiOff
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.ecuapp.dashboard.data.model.CANData
import com.ecuapp.dashboard.ui.component.CircularGauge
import com.ecuapp.dashboard.ui.component.TCUStatusIndicator
import com.ecuapp.dashboard.viewmodel.DashboardViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DashboardScreen(
    onNavigateToSettings: () -> Unit,
    onDisconnect: () -> Unit,
    viewModel: DashboardViewModel = viewModel()
) {
    val canData by viewModel.canData.collectAsState()
    val connectionStatus by viewModel.connectionStatus.collectAsState()
    val lastUpdate by viewModel.lastUpdate.collectAsState()
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { 
                    Text(
                        "ECU Dashboard",
                        color = Color.White,
                        fontWeight = FontWeight.Bold
                    )
                },
                actions = {
                    IconButton(onClick = onNavigateToSettings) {
                        Icon(
                            Icons.Default.Settings,
                            contentDescription = "Settings",
                            tint = Color.White
                        )
                    }
                    IconButton(onClick = onDisconnect) {
                        Icon(
                            Icons.Default.WifiOff,
                            contentDescription = "Disconnect",
                            tint = Color.White
                        )
                    }
                },
                colors = TopAppBarDefaults.topAppBarColors(
                    containerColor = Color(0xFF1A1A1A)
                )
            )
        }
    ) { paddingValues ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .background(Color(0xFF0A0A0A))
                .padding(paddingValues)
                .padding(16.dp)
        ) {
            // Connection Status
            ConnectionStatusCard(
                status = connectionStatus,
                lastUpdate = lastUpdate
            )
            
            Spacer(modifier = Modifier.height(16.dp))
            
            // Gauges Grid
            LazyVerticalGrid(
                columns = GridCells.Fixed(2),
                verticalArrangement = Arrangement.spacedBy(16.dp),
                horizontalArrangement = Arrangement.spacedBy(16.dp),
                modifier = Modifier.fillMaxWidth()
            ) {
                items(getGaugeData(canData)) { gauge ->
                    CircularGauge(
                        value = gauge.value,
                        min = gauge.min,
                        max = gauge.max,
                        label = gauge.label,
                        unit = gauge.unit,
                        color = gauge.color,
                        warningThreshold = gauge.warningThreshold,
                        dangerThreshold = gauge.dangerThreshold
                    )
                }
                
                // TCU Status as separate item
                item {
                    TCUStatusIndicator(
                        status = canData.tcuStatus,
                        modifier = Modifier.fillMaxWidth()
                    )
                }
            }
        }
    }
}

@Composable
private fun ConnectionStatusCard(
    status: String,
    lastUpdate: Long
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color(0xFF2A2A2A)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            Column {
                Text(
                    text = "Connection Status",
                    color = Color.Gray,
                    fontSize = 12.sp
                )
                Text(
                    text = status,
                    color = when (status) {
                        "Connected" -> Color.Green
                        "Connecting..." -> Color.Yellow
                        else -> Color.Red
                    },
                    fontWeight = FontWeight.Bold,
                    fontSize = 16.sp
                )
            }
            
            Column(horizontalAlignment = Alignment.End) {
                Text(
                    text = "Last Update",
                    color = Color.Gray,
                    fontSize = 12.sp
                )
                Text(
                    text = formatTime(lastUpdate),
                    color = Color.White,
                    fontSize = 14.sp
                )
            }
        }
    }
}

data class GaugeData(
    val value: Float,
    val min: Float,
    val max: Float,
    val label: String,
    val unit: String,
    val color: Color,
    val warningThreshold: Float? = null,
    val dangerThreshold: Float? = null
)

private fun getGaugeData(canData: CANData): List<GaugeData> = listOf(
    GaugeData(
        value = canData.mapPressure.toFloat(),
        min = 100f,
        max = 250f,
        label = "MAP Pressure",
        unit = "kPa",
        color = Color(0xFF00D4FF),
        warningThreshold = 200f,
        dangerThreshold = 230f
    ),
    GaugeData(
        value = canData.wastegatePos.toFloat(),
        min = 0f,
        max = 100f,
        label = "Wastegate",
        unit = "%",
        color = Color(0xFF00FF88)
    ),
    GaugeData(
        value = canData.tpsPosition.toFloat(),
        min = 0f,
        max = 100f,
        label = "TPS",
        unit = "%",
        color = Color(0xFFFFD700)
    ),
    GaugeData(
        value = canData.engineRpm.toFloat(),
        min = 0f,
        max = 7000f,
        label = "Engine RPM",
        unit = "RPM",
        color = Color(0xFFFF6B35),
        warningThreshold = 5500f,
        dangerThreshold = 6500f
    ),
    GaugeData(
        value = canData.targetBoost.toFloat(),
        min = 100f,
        max = 250f,
        label = "Target Boost",
        unit = "kPa",
        color = Color(0xFFFFD700)
    )
)

private fun formatTime(timestamp: Long): String {
    if (timestamp == 0L) return "--:--:--"
    val now = System.currentTimeMillis()
    val diff = now - timestamp
    return when {
        diff < 1000 -> "Now"
        diff < 60000 -> "${diff / 1000}s ago"
        else -> "Offline"
    }
}