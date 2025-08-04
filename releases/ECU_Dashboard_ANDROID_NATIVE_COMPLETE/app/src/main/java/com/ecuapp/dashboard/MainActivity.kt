package com.ecuapp.dashboard

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.ecuapp.dashboard.ui.screen.DashboardScreen
import com.ecuapp.dashboard.ui.screen.ConnectionScreen
import com.ecuapp.dashboard.ui.screen.SettingsScreen
import com.ecuapp.dashboard.ui.theme.ECUDashboardTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        
        setContent {
            ECUDashboardTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    ECUDashboardApp()
                }
            }
        }
    }
}

@Composable
fun ECUDashboardApp() {
    val navController = rememberNavController()
    
    NavHost(
        navController = navController,
        startDestination = "connection"
    ) {
        composable("connection") {
            ConnectionScreen(
                onNavigateToDashboard = {
                    navController.navigate("dashboard") {
                        popUpTo("connection") { inclusive = true }
                    }
                }
            )
        }
        
        composable("dashboard") {
            DashboardScreen(
                onNavigateToSettings = {
                    navController.navigate("settings")
                },
                onDisconnect = {
                    navController.navigate("connection") {
                        popUpTo("dashboard") { inclusive = true }
                    }
                }
            )
        }
        
        composable("settings") {
            SettingsScreen(
                onNavigateBack = {
                    navController.popBackStack()
                }
            )
        }
    }
}

@Preview(showBackground = true)
@Composable
fun DefaultPreview() {
    ECUDashboardTheme {
        ECUDashboardApp()
    }
}