import { pgTable, text, serial, integer, boolean, timestamp, real } from "drizzle-orm/pg-core";
import { createInsertSchema } from "drizzle-zod";
import { z } from "zod";

export const users = pgTable("users", {
  id: serial("id").primaryKey(),
  username: text("username").notNull().unique(),
  password: text("password").notNull(),
});

export const ecuDataLogs = pgTable("ecu_data_logs", {
  id: serial("id").primaryKey(),
  timestamp: timestamp("timestamp").defaultNow().notNull(),
  mapPressure: real("map_pressure").notNull(),
  wastegatePosition: real("wastegate_position").notNull(),
  tpsPosition: real("tps_position").notNull(),
  engineRpm: real("engine_rpm").notNull(),
  targetBoost: real("target_boost").notNull(),
  tcuProtectionActive: boolean("tcu_protection_active").default(false),
  tcuLimpMode: boolean("tcu_limp_mode").default(false),
  torqueRequest: real("torque_request"),
});

export const systemSettings = pgTable("system_settings", {
  id: serial("id").primaryKey(),
  maxBoostLimit: real("max_boost_limit").default(250),
  maxRpmLimit: real("max_rpm_limit").default(7000),
  audioAlertsEnabled: boolean("audio_alerts_enabled").default(true),
  ecuAddress: text("ecu_address").default("ws://192.168.1.100:8080"),
});

export const insertUserSchema = createInsertSchema(users).pick({
  username: true,
  password: true,
});

export const insertEcuDataLogSchema = createInsertSchema(ecuDataLogs).omit({
  id: true,
  timestamp: true,
});

export const insertSystemSettingsSchema = createInsertSchema(systemSettings).omit({
  id: true,
});

export type InsertUser = z.infer<typeof insertUserSchema>;
export type User = typeof users.$inferSelect;
export type EcuDataLog = typeof ecuDataLogs.$inferSelect;
export type InsertEcuDataLog = z.infer<typeof insertEcuDataLogSchema>;
export type SystemSettings = typeof systemSettings.$inferSelect;
export type InsertSystemSettings = z.infer<typeof insertSystemSettingsSchema>;
