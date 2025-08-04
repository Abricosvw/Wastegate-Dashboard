package com.ecuapp.dashboard.ui.component

import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.animation.core.tween
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.StrokeCap
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import kotlin.math.cos
import kotlin.math.sin

@Composable
fun CircularGauge(
    value: Float,
    min: Float,
    max: Float,
    label: String,
    unit: String,
    color: Color,
    warningThreshold: Float? = null,
    dangerThreshold: Float? = null,
    modifier: Modifier = Modifier
) {
    val animatedValue by animateFloatAsState(
        targetValue = value,
        animationSpec = tween(durationMillis = 300),
        label = "gauge_animation"
    )
    
    // Determine gauge color based on thresholds
    val gaugeColor = when {
        dangerThreshold != null && value >= dangerThreshold -> Color(0xFFFF4444)
        warningThreshold != null && value >= warningThreshold -> Color(0xFFFFA500)
        else -> color
    }
    
    Card(
        modifier = modifier
            .aspectRatio(1f)
            .fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color(0xFF2A2A2A)
        ),
        shape = RoundedCornerShape(16.dp)
    ) {
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp),
            contentAlignment = Alignment.Center
        ) {
            // Circular gauge canvas
            Canvas(
                modifier = Modifier.fillMaxSize()
            ) {
                val center = this.center
                val radius = size.minDimension / 2 - 20.dp.toPx()
                val strokeWidth = 12.dp.toPx()
                
                // Background arc (270° total)
                drawArc(
                    color = Color(0xFF404040),
                    startAngle = 135f,
                    sweepAngle = 270f,
                    useCenter = false,
                    style = Stroke(
                        width = strokeWidth,
                        cap = StrokeCap.Round
                    ),
                    size = androidx.compose.ui.geometry.Size(
                        radius * 2,
                        radius * 2
                    ),
                    topLeft = androidx.compose.ui.geometry.Offset(
                        center.x - radius,
                        center.y - radius
                    )
                )
                
                // Value arc
                val percentage = (animatedValue - min) / (max - min)
                val sweepAngle = percentage * 270f
                
                drawArc(
                    color = gaugeColor,
                    startAngle = 135f,
                    sweepAngle = sweepAngle,
                    useCenter = false,
                    style = Stroke(
                        width = strokeWidth,
                        cap = StrokeCap.Round
                    ),
                    size = androidx.compose.ui.geometry.Size(
                        radius * 2,
                        radius * 2
                    ),
                    topLeft = androidx.compose.ui.geometry.Offset(
                        center.x - radius,
                        center.y - radius
                    )
                )
                
                // Warning/Danger threshold indicators
                warningThreshold?.let { threshold ->
                    val thresholdPercentage = (threshold - min) / (max - min)
                    val thresholdAngle = 135f + thresholdPercentage * 270f
                    val thresholdRadius = radius + 8.dp.toPx()
                    
                    val thresholdX = center.x + thresholdRadius * cos(Math.toRadians(thresholdAngle.toDouble())).toFloat()
                    val thresholdY = center.y + thresholdRadius * sin(Math.toRadians(thresholdAngle.toDouble())).toFloat()
                    
                    drawCircle(
                        color = Color(0xFFFFA500),
                        radius = 4.dp.toPx(),
                        center = androidx.compose.ui.geometry.Offset(thresholdX, thresholdY)
                    )
                }
                
                dangerThreshold?.let { threshold ->
                    val thresholdPercentage = (threshold - min) / (max - min)
                    val thresholdAngle = 135f + thresholdPercentage * 270f
                    val thresholdRadius = radius + 8.dp.toPx()
                    
                    val thresholdX = center.x + thresholdRadius * cos(Math.toRadians(thresholdAngle.toDouble())).toFloat()
                    val thresholdY = center.y + thresholdRadius * sin(Math.toRadians(thresholdAngle.toDouble())).toFloat()
                    
                    drawCircle(
                        color = Color(0xFFFF4444),
                        radius = 4.dp.toPx(),
                        center = androidx.compose.ui.geometry.Offset(thresholdX, thresholdY)
                    )
                }
            }
            
            // Center value display
            Column(
                horizontalAlignment = Alignment.CenterHorizontally,
                modifier = Modifier.offset(y = 8.dp)
            ) {
                Text(
                    text = "${animatedValue.toInt()}",
                    color = Color.White,
                    fontSize = 24.sp,
                    fontWeight = FontWeight.Bold
                )
                Text(
                    text = unit,
                    color = Color.Gray,
                    fontSize = 12.sp
                )
            }
            
            // Label at bottom
            Box(
                modifier = Modifier
                    .align(Alignment.BottomCenter)
                    .offset(y = (-8).dp)
            ) {
                Text(
                    text = label,
                    color = Color.White,
                    fontSize = 14.sp,
                    fontWeight = FontWeight.Medium
                )
            }
        }
    }
}