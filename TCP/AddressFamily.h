namespace net
{
	namespace sockets {
		namespace AddressFamily {
			//
			// ������:
			//     ������ ����� ���������, ��������� System.Net.Sockets.Socket ����� ������������
			//     �����.
			enum AddressFamily
			{
				//
				// ������:
				//     ��������� ����������� �������.
				Unknown = -1,
				//
				// ������:
				//     ��������� ��������������� �������.
				Unspecified = 0,
				//
				// ������:
				//     ��������� ����� ���� UNIX.
				Unix = 1,
				//
				// ������:
				//     IP-����� ������ 4.
				InterNetwork = 2,
				//
				// ������:
				//     ����� ARPANET: ������ ������� ����������.
				ImpLink = 3,
				//
				// ������:
				//     ����� ���������� ��������.
				Pup = 4,
				//
				// ������:
				//     ����� ���������� ����� MIT.
				Chaos = 5,
				//
				// ������:
				//     ����� ���������� Xerox NS.
				NS = 6,
				//
				// ������:
				//     ����� IPX � SPX.
				Ipx = 6,
				//
				// ������:
				//     ����� ���������� ISO.
				Iso = 7,
				//
				// ������:
				//     ����� ���������� OSI.
				Osi = 7,
				//
				// ������:
				//     ����� ����������� ���������� �������������� ���������� (ECMA).
				Ecma = 8,
				//
				// ������:
				//     ����� ���������� Datakit.
				DataKit = 9,
				//
				// ������:
				//     ������ ���������� CCITT, �������� X.25.
				Ccitt = 10,
				//
				// ������:
				//     ����� IBM SNA.
				Sna = 11,
				//
				// ������:
				//     ����� DECnet.
				DecNet = 12,
				//
				// ������:
				//     ����� ���������� ������� ������ �������� ������.
				DataLink = 13,
				//
				// ������:
				//     LAT �����.
				Lat = 14,
				//
				// ������:
				//     ����� NSC Hyperchannel.
				HyperChannel = 15,
				//
				// ������:
				//     ����� AppleTalk.
				AppleTalk = 16,
				//
				// ������:
				//     ����� NetBios.
				NetBios = 17,
				//
				// ������:
				//     ����� VoiceView.
				VoiceView = 18,
				//
				// ������:
				//     ����� FireFox.
				FireFox = 19,
				//
				// ������:
				//     ����� Banyan.
				Banyan = 21,
				//
				// ������:
				//     ����� ����������� ����� ATM.
				Atm = 22,
				//
				// ������:
				//     ����� ��� IP ������ 6.
				InterNetworkV6 = 23,
				//
				// ������:
				//     ����� ���������� ��������� ���������� Microsoft.
				Cluster = 24,
				//
				// ������:
				//     ����� ������� ������ IEEE 1284.4.
				Ieee12844 = 25,
				//
				// ������:
				//     ����� IrDA.
				Irda = 26,
				//
				// ������:
				//     ����� ����� ���������� ���������� OSI ������������ ����.
				NetworkDesigners = 28,
				//
				// ������:
				//     ����� MAX.
				Max = 29
			};
		}
	}
}