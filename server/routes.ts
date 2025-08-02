import type { Express } from "express";
import { createServer, type Server } from "http";
import { WebSocketServer, WebSocket } from "ws";
import { storage } from "./storage";
import { z } from "zod";
import { insertEcuDataLogSchema, insertSystemSettingsSchema } from "@shared/schema";

// WebSocket message schemas
const ecuDataSchema = z.object({
  type: z.literal("ecuData"),
  data: z.object({
    mapPressure: z.number(),
    wastegatePosition: z.number(),
    tpsPosition: z.number(),
    engineRpm: z.number(),
    targetBoost: z.number(),
    tcuProtectionActive: z.boolean().optional().default(false),
    tcuLimpMode: z.boolean().optional().default(false),
    torqueRequest: z.number().optional(),
  })
});

const connectionStatusSchema = z.object({
  type: z.literal("connectionStatus"),
  connected: z.boolean(),
  message: z.string().optional(),
});

export async function registerRoutes(app: Express): Promise<Server> {
  const httpServer = createServer(app);

  // WebSocket Server for real-time ECU data
  const wss = new WebSocketServer({ server: httpServer, path: '/ws' });
  
  let connectedClients = new Set<WebSocket>();
  let lastEcuData: any = null;

  wss.on('connection', (ws) => {
    console.log('Client connected to WebSocket');
    connectedClients.add(ws);

    // Send last known data to new client if available
    if (lastEcuData) {
      ws.send(JSON.stringify({
        type: 'ecuData',
        data: lastEcuData
      }));
    }

    ws.on('message', async (message) => {
      try {
        const parsedMessage = JSON.parse(message.toString());
        
        if (parsedMessage.type === 'ecuData') {
          const validated = ecuDataSchema.parse(parsedMessage);
          lastEcuData = validated.data;
          
          // Store data log
          await storage.createEcuDataLog(validated.data);
          
          // Broadcast to all connected clients
          connectedClients.forEach(client => {
            if (client !== ws && client.readyState === WebSocket.OPEN) {
              client.send(JSON.stringify(validated));
            }
          });
        }
      } catch (error) {
        console.error('WebSocket message error:', error);
        ws.send(JSON.stringify({
          type: 'error',
          message: 'Invalid message format'
        }));
      }
    });

    ws.on('close', () => {
      console.log('Client disconnected from WebSocket');
      connectedClients.delete(ws);
    });

    ws.on('error', (error) => {
      console.error('WebSocket error:', error);
      connectedClients.delete(ws);
    });
  });

  // REST API Routes
  
  // Get recent ECU data logs
  app.get('/api/ecu-data', async (req, res) => {
    try {
      const limit = parseInt(req.query.limit as string) || 100;
      const logs = await storage.getRecentEcuDataLogs(limit);
      res.json(logs);
    } catch (error) {
      console.error('Error fetching ECU data:', error);
      res.status(500).json({ message: 'Failed to fetch ECU data' });
    }
  });

  // Get system settings
  app.get('/api/settings', async (req, res) => {
    try {
      const settings = await storage.getSystemSettings();
      res.json(settings);
    } catch (error) {
      console.error('Error fetching settings:', error);
      res.status(500).json({ message: 'Failed to fetch settings' });
    }
  });

  // Update system settings
  app.post('/api/settings', async (req, res) => {
    try {
      const validated = insertSystemSettingsSchema.parse(req.body);
      const settings = await storage.updateSystemSettings(validated);
      res.json(settings);
    } catch (error) {
      console.error('Error updating settings:', error);
      res.status(400).json({ message: 'Invalid settings data' });
    }
  });

  // Clear data logs
  app.delete('/api/ecu-data', async (req, res) => {
    try {
      await storage.clearEcuDataLogs();
      res.json({ message: 'Data logs cleared' });
    } catch (error) {
      console.error('Error clearing data logs:', error);
      res.status(500).json({ message: 'Failed to clear data logs' });
    }
  });

  // Export data logs
  app.get('/api/ecu-data/export', async (req, res) => {
    try {
      const logs = await storage.getAllEcuDataLogs();
      res.setHeader('Content-Type', 'application/json');
      res.setHeader('Content-Disposition', 'attachment; filename=ecu-data-export.json');
      res.json(logs);
    } catch (error) {
      console.error('Error exporting data:', error);
      res.status(500).json({ message: 'Failed to export data' });
    }
  });

  return httpServer;
}
