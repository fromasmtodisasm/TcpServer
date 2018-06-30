namespace net
{
	namespace sockets {
		namespace SocketType {
			//
			// ������:
			//     ������ ��� ������, ��������� System.Net.Sockets.Socket ������������ �����.
			enum SocketType
			{
				//
				// ������:
				//     ������ ����������� Socket ����.
				Unknown = -1,
				//
				// ������:
				//     ������������ ��������, ������������-����������� �������� ������ ��� ������������
				//     ������ � ��� ���������� ������. A Socket ����� ���� ��������������� � ����� �����
				//     � ������� ����������� � ���������� ���� ����� ������� ������ �������. System.Net.Sockets.SocketType.Stream
				//     ���������� �������� ���������� ��������� (System.Net.Sockets.ProtocolType.Tcp)
				//     System.Net.Sockets.ProtocolType � InterNetworkSystem.Net.Sockets.AddressFamily.
				Stream = 1,
				//
				// ������:
				//     ������������ ���������� � ���������� ��������� ������������� (������ �����) ������������
				//     ������, ��� ������������ ����������. ��������� ����� ���� �������� ��� ����������
				//     � ����� �� ����������� �������. ������ System.Net.Sockets.Socket ���� System.Net.Sockets.SocketType.Dgram
				//     ��� ���������� �� �������� � ��������� ������ � ����� ����������������� �� ����������
				//     �����. System.Net.Sockets.SocketType.Dgram ���������� Datagram Protocol (System.Net.Sockets.ProtocolType.Udp)
				//     � System.Net.Sockets.AddressFamily.InterNetworkSystem.Net.Sockets.AddressFamily.
				Dgram = 2,
				//
				// ������:
				//     ������������ ������ � ��������� ������������� ���������. � ������� System.Net.Sockets.SocketTypeSystem.Net.Sockets.SocketType.Raw,
				//     ����� ����������������� � ������� ����������, ����� ��� Internet Control Message
				//     Protocol (System.Net.Sockets.ProtocolType.Icmp) � Internet Group Management Protocol
				//     (System.Net.Sockets.ProtocolType.Igmp). ��� ��������, ���������� ������ ������������
				//     ������ IP-���������. ���������� ���������� ������������ � ������� ����������
				//     � IP-��������� ��� ���������.
				Raw = 3,
				//
				// ������:
				//     ���������, ��������������� �� ���������, �������� ��������� ��������� � �����������
				//     ������ ��������� � ������. Unduplicated � � �������, ��������� ��������� Rdm
				//     (�������� �������� ���������). ����� ���� ����������� ������������ �����������
				//     � ������ ������ ���������. ���� ���������������� Socket � ������� System.Net.Sockets.SocketType.Rdm,
				//     �� ������� ����������� � ���������� ���� �� �������� � ��������� ������. � System.Net.Sockets.SocketType.Rdm,
				//     ����� ����������������� �� ���������� �����.
				Rdm = 4,
				//
				// ������:
				//     ������������ ���������������� �� ����������� � �������� ������������ ��������
				//     ������������� �������� ������� ����� ����. System.Net.Sockets.SocketType.Seqpacket
				//     ��������� ������ � ��������� ������� ������ ������ ������. ������ Socket ����
				//     System.Net.Sockets.SocketType.Seqpacket ��������������� � ����� ����� � �������
				//     ����������� � ���������� ���� ����� ������� �������� ������.
				Seqpacket = 5
			};
		}
	}
}