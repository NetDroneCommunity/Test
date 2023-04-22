/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;

namespace CompatibilityStandards {
	#region User Defined Types
	using UINT = System.UInt32;
	using BYTE = System.Byte;
	using WORD = System.UInt16;
	using DWORD = System.UInt32;
	using QWORD = System.UInt64;
	using ULONG = System.UInt32;
	using ULONG32 = System.UInt32;
	using ULONG64 = System.UInt64;
	using CHAR = System.Byte;
	using INT = System.Int32;
	using INT16 = System.Int16;
	using INT32 = System.Int32;
	using INT64 = System.Int64;
	using UINT16 = System.UInt16;
	using UINT32 = System.UInt32;
	using UINT64 = System.UInt64;
	using LONG32 = System.Int32;
	using LONG64 = System.Int64;
	using FLOAT = System.Single;
	using DOUBLE = System.Double;
	using tick_t = System.UInt64;
	using time_t = System.UInt64;
	using size_t = System.UInt64;
	using wchar_t = System.Char;
	#endregion

	public class main : GameFramework {
		public static void
		CallbackSigint(object sender, ConsoleCancelEventArgs args) {
			args.Cancel = true;

			g_kFramework.Release();
		}

		public static void
		Version() {
			PRINT(GetNetDroneVersion());
			PRINT(GetServiceVersion());
			PRINT("non-thread");
		}

		public static void Main(string[] args) {
			Console.CancelKeyPress += new ConsoleCancelEventHandler(CallbackSigint);

			string szRemoteIp = "127.0.0.1";
			UINT16 usRemotePort = 11000;
			bool bReliableUdp = false;
			bool bHeaderCrypt = true;
			string szLoginId = "dummy";
			INT iChannelIndex = 0;

			Console.Write("input remote ip (default: 127.0.0.1): ");
			string szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				szRemoteIp = szRead;
			}

			Console.Write("input remote port (default: 11000): ");
			szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				usRemotePort = Convert.ToUInt16(szRead);
			}

			Console.Write("input connector type (default: tcp, 1: tcp, 2: reliable udp): ");
			szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				if((0 == String.Compare(szRead, "2")) || (0 == String.Compare(szRead, "reliable udp"))) {
					bReliableUdp = true;
				}
			}

			Console.Write("input header crypt (default: true, 1: false, 2: true): ");
			szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				if((0 == String.Compare(szRead, "1")) || (0 == String.Compare(szRead, "false"))) {
					bHeaderCrypt = false;
				}
			}

			Console.Write("input login id (default: dummy): ");
			szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				szLoginId = szRead;
			}

			Console.Write("input channel id: (default: 1): ");
			szRead = Console.ReadLine();

			if(0 < szRead.Length) {
				if(0 < Convert.ToInt32(szRead)) {
					iChannelIndex = (Convert.ToInt32(szRead) - 1);
				}
			}

			PRINT("");
			if(0 < szRemoteIp.Length) {
				if(0 < usRemotePort) {
					if(0 < szLoginId.Length) {
						try {
							g_kCfgMgr.SetReliableUdp(bReliableUdp);
							g_kCfgMgr.SetHeaderCrypt(bHeaderCrypt);

							if(g_kFramework.Initialize()) {
								g_kCfgMgr.SetLoginId(szLoginId);
								g_kCfgMgr.SetChannelIndex(iChannelIndex);

								CConnector kConnnector = g_kNetMgr.GetConnector();
								if(isptr(kConnnector)) {
									kConnnector.SetRemoteAddress(ConvertToBytes(szRemoteIp), usRemotePort);

									PRINT("remote ip: " + ConvertToString(kConnnector.GetRemoteAddress()) + ", remote port: " + kConnnector.GetRemotePort());
									if(g_kCfgMgr.IsReliableUdp()) {
										PRINT("connector type: reliable udp");
									} else {
										PRINT("connector type: tcp");
									}
									PRINT("header crypt: " + g_kCfgMgr.IsHeaderCrypt());
									PRINT("channel index: " + g_kCfgMgr.GetChannelIndex());
									PRINT("login id: " + g_kCfgMgr.GetLoginId());

									PRINT("");
									Version();

									PRINT("");
									if(g_kNetMgr.Login()) {
										while(g_kFramework.IsDoing()) {
											if(g_kNetMgr.IsInput()) {
												string szMessage = "";

												Console.Write("input message: ");
												szRead = Console.ReadLine();

												if(0 < szRead.Length) {
													szMessage = szRead;
												}

												if(0 < szMessage.Length) {
													SEND_USER_CHAT(szMessage);
													g_kNetMgr.SetInput(false);
												}
											}

											g_kFramework.Update();
											Thread.Sleep(10);
										}
									}
								}
							}
						} catch(Exception e) {
							//Backtrace(e);
						} finally {
							g_kFramework.Release();
						}
					} else {
						PRINT("error: login id is null: ");
					}
				} else {
					PRINT("error: remote port is null: ");
				}
			} else {
				PRINT("error: remote ip is null: ");
			}

			PRINT("press any key to exit.");
			Console.ReadLine();
		}
	}
}

/* EOF */
