namespace net
{
	namespace sockets {
		namespace SocketType {
			//
			// Сводка:
			//     Задает тип сокета, экземпляр System.Net.Sockets.Socket представляет класс.
			enum SocketType
			{
				//
				// Сводка:
				//     Задает неизвестный Socket типа.
				Unknown = -1,
				//
				// Сводка:
				//     Поддерживает надежные, двустороннее-подключений байтовые потоки без дублирования
				//     данных и без сохранения границ. A Socket этого типа взаимодействует с одним узлом
				//     и требует подключения к удаленному узлу перед началом обмена данными. System.Net.Sockets.SocketType.Stream
				//     использует протокол управления передачей (System.Net.Sockets.ProtocolType.Tcp)
				//     System.Net.Sockets.ProtocolType и InterNetworkSystem.Net.Sockets.AddressFamily.
				Stream = 1,
				//
				// Сводка:
				//     Поддерживает датаграммы — ненадежные сообщения фиксированной (обычно малой) максимальной
				//     длиной, без установления соединения. Сообщения могут быть потеряны или копируется
				//     и может не соблюдаться порядок. Объект System.Net.Sockets.Socket типа System.Net.Sockets.SocketType.Dgram
				//     без соединения до отправки и получения данных и могут взаимодействовать со множеством
				//     узлов. System.Net.Sockets.SocketType.Dgram использует Datagram Protocol (System.Net.Sockets.ProtocolType.Udp)
				//     и System.Net.Sockets.AddressFamily.InterNetworkSystem.Net.Sockets.AddressFamily.
				Dgram = 2,
				//
				// Сводка:
				//     Поддерживает доступ к основному транспортному протоколу. С помощью System.Net.Sockets.SocketTypeSystem.Net.Sockets.SocketType.Raw,
				//     могут взаимодействовать с помощью протоколов, таких как Internet Control Message
				//     Protocol (System.Net.Sockets.ProtocolType.Icmp) и Internet Group Management Protocol
				//     (System.Net.Sockets.ProtocolType.Igmp). При отправке, приложение должно обеспечивать
				//     полный IP-заголовок. Полученные датаграммы возвращаются с помощью параметров
				//     и IP-заголовок без изменений.
				Raw = 3,
				//
				// Сводка:
				//     Поддержка, ориентированных на сообщения, надежной доставкой сообщений и сохранением
				//     границ сообщений в данных. Unduplicated и в порядке, получении сообщений Rdm
				//     (надежной доставки сообщений). Кроме того отправителю отправляется уведомление
				//     в случае потери сообщений. Если инициализировать Socket с помощью System.Net.Sockets.SocketType.Rdm,
				//     не требуют подключения к удаленному узлу до отправки и получения данных. С System.Net.Sockets.SocketType.Rdm,
				//     могут взаимодействовать со множеством узлов.
				Rdm = 4,
				//
				// Сводка:
				//     Обеспечивает ориентированного на подключение и надежных двустороннюю передачу
				//     упорядоченных байтовых потоков через сеть. System.Net.Sockets.SocketType.Seqpacket
				//     дублирует данные и сохраняет границы внутри потока данных. Объект Socket типа
				//     System.Net.Sockets.SocketType.Seqpacket взаимодействует с одним узлом и требует
				//     подключения к удаленному узлу перед началом передачи данных.
				Seqpacket = 5
			};
		}
	}
}