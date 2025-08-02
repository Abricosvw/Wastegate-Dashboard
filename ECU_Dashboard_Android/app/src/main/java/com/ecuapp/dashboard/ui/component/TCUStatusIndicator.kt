package com.ecuapp.dashboard.ui.component

import androidx.compose.animation.animateColorAsState
import androidx.compose.animation.core.*
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

@Composable
fun TCUStatusIndicator(
    status: Int,
    modifier: Modifier = Modifier
) {
    val (statusText, statusColor, shouldBlink) = when (status) {
        0 -> Triple("TCU OK", Color(0xFF00FF88), false)
        1 -> Triple("TCU WARN", Color(0xFFFFA500), false)
        2 -> Triple("TCU ERROR", Color(0xFFFF4444), true)
        else -> Triple("UNKNOWN", Color.Gray, false)
    }
    
    // Blinking animation for error state
    val infiniteTransition = rememberInfiniteTransition(label = "blink")
    val alpha by infiniteTransition.animateFloat(
        initialValue = 1f,
        targetValue = if (shouldBlink) 0.3f else 1f,
        animationSpec = infiniteRepeatable(
            animation = tween(500),
            repeatMode = RepeatMode.Reverse
        ),
        label = "alpha_animation"
    )
    
    val animatedColor by animateColorAsState(
        targetValue = statusColor.copy(alpha = alpha),
        label = "color_animation"
    )
    
    Card(
        modifier = modifier.aspectRatio(1f),
        colors = CardDefaults.cardColors(
            containerColor = Color(0xFF2A2A2A)
        ),
        shape = RoundedCornerShape(16.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            // Status indicator circle
            Box(
                modifier = Modifier
                    .size(80.dp)
                    .clip(CircleShape)
                    .background(animatedColor)
            )
            
            Spacer(modifier = Modifier.height(12.dp))
            
            // Status text
            Text(
                text = statusText,
                color = Color.White,
                fontSize = 16.sp,
                fontWeight = FontWeight.Bold
            )
            
            Text(
                text = "Transmission Control",
                color = Color.Gray,
                fontSize = 12.sp
            )
        }
    }
}