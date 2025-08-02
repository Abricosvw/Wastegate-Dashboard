import { users, ecuDataLogs, systemSettings, type User, type InsertUser, type EcuDataLog, type InsertEcuDataLog, type SystemSettings, type InsertSystemSettings } from "@shared/schema";
import { db } from "./db";
import { eq, desc } from "drizzle-orm";

export interface IStorage {
  getUser(id: number): Promise<User | undefined>;
  getUserByUsername(username: string): Promise<User | undefined>;
  createUser(user: InsertUser): Promise<User>;
  
  createEcuDataLog(data: InsertEcuDataLog): Promise<EcuDataLog>;
  getRecentEcuDataLogs(limit: number): Promise<EcuDataLog[]>;
  getAllEcuDataLogs(): Promise<EcuDataLog[]>;
  clearEcuDataLogs(): Promise<void>;
  
  getSystemSettings(): Promise<SystemSettings>;
  updateSystemSettings(settings: InsertSystemSettings): Promise<SystemSettings>;
}

export class DatabaseStorage implements IStorage {
  async getUser(id: number): Promise<User | undefined> {
    const [user] = await db.select().from(users).where(eq(users.id, id));
    return user || undefined;
  }

  async getUserByUsername(username: string): Promise<User | undefined> {
    const [user] = await db.select().from(users).where(eq(users.username, username));
    return user || undefined;
  }

  async createUser(insertUser: InsertUser): Promise<User> {
    const [user] = await db
      .insert(users)
      .values(insertUser)
      .returning();
    return user;
  }

  async createEcuDataLog(data: InsertEcuDataLog): Promise<EcuDataLog> {
    const [log] = await db
      .insert(ecuDataLogs)
      .values({
        ...data,
        tcuProtectionActive: data.tcuProtectionActive ?? false,
        tcuLimpMode: data.tcuLimpMode ?? false,
        torqueRequest: data.torqueRequest ?? null
      })
      .returning();
    return log;
  }

  async getRecentEcuDataLogs(limit: number): Promise<EcuDataLog[]> {
    const logs = await db
      .select()
      .from(ecuDataLogs)
      .orderBy(desc(ecuDataLogs.timestamp))
      .limit(limit);
    return logs;
  }

  async getAllEcuDataLogs(): Promise<EcuDataLog[]> {
    const logs = await db
      .select()
      .from(ecuDataLogs)
      .orderBy(desc(ecuDataLogs.timestamp));
    return logs;
  }

  async clearEcuDataLogs(): Promise<void> {
    await db.delete(ecuDataLogs);
  }

  async getSystemSettings(): Promise<SystemSettings> {
    const [settings] = await db.select().from(systemSettings).limit(1);
    
    if (!settings) {
      // Create default settings if none exist
      const [newSettings] = await db
        .insert(systemSettings)
        .values({
          maxBoostLimit: 250,
          maxRpmLimit: 7000,
          audioAlertsEnabled: true,
          ecuAddress: "ws://192.168.1.100:8080"
        })
        .returning();
      return newSettings;
    }
    
    return settings;
  }

  async updateSystemSettings(settings: InsertSystemSettings): Promise<SystemSettings> {
    const currentSettings = await this.getSystemSettings();
    
    const [updatedSettings] = await db
      .update(systemSettings)
      .set(settings)
      .where(eq(systemSettings.id, currentSettings.id))
      .returning();
    
    return updatedSettings;
  }
}

export const storage = new DatabaseStorage();
