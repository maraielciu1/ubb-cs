using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace pdp_lab4
{
    public class AsyncAwaitDownloader
    {
        private class DownloadSession
        {
            private readonly string _url;
            private readonly string _host;
            private readonly string _path;
            private readonly int _port;
            private Socket? _socket;
            private readonly byte[] _buffer;
            private int _bufferPos;
            private int _bufferSize;
            private readonly StringBuilder _headerBuilder;
            private int _contentLength;

            public DownloadSession(string url)
            {
                _url = url;
                ParseUrl(url, out _host, out _path, out _port);
                _buffer = new byte[4096];
                _headerBuilder = new StringBuilder();
            }

            private void ParseUrl(string url, out string host, out string path, out int port)
            {
                if (!url.StartsWith("http://"))
                {
                    throw new ArgumentException("URL must start with http://");
                }

                url = url.Substring(7);
                int slashIndex = url.IndexOf('/');
                int colonIndex = url.IndexOf(':');

                if (colonIndex != -1 && (slashIndex == -1 || colonIndex < slashIndex))
                {
                    host = url.Substring(0, colonIndex);
                    int portEnd = slashIndex == -1 ? url.Length : slashIndex;
                    port = int.Parse(url.Substring(colonIndex + 1, portEnd - colonIndex - 1));
                    path = slashIndex == -1 ? "/" : url.Substring(slashIndex);
                }
                else
                {
                    if (slashIndex == -1)
                    {
                        host = url;
                        path = "/";
                    }
                    else
                    {
                        host = url.Substring(0, slashIndex);
                        path = url.Substring(slashIndex);
                    }
                    port = 80;
                }
            }

            public async Task DownloadAsync()
            {
                try
                {
                    IPHostEntry hostEntry = Dns.GetHostEntry(_host);
                    IPAddress ipAddress = hostEntry.AddressList[0];
                    IPEndPoint remoteEP = new IPEndPoint(ipAddress, _port);

                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                    await ConnectAsync(_socket, remoteEP);
                    Console.WriteLine($"Connected to {_host}:{_port} for {_url}");

                    string request = $"GET {_path} HTTP/1.1\r\nHost: {_host}\r\nConnection: close\r\n\r\n";
                    byte[] requestBytes = Encoding.ASCII.GetBytes(request);
                    await SendAsync(_socket, requestBytes, 0, requestBytes.Length);

                    await ReceiveHeadersAsync();

                    Finish();
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error downloading {_url}: {ex.Message}");
                    _socket?.Close();
                }
            }

            private Task ConnectAsync(Socket socket, IPEndPoint remoteEP)
            {
                TaskCompletionSource<object> tcs = new TaskCompletionSource<object>();
                socket.BeginConnect(remoteEP, (IAsyncResult ar) =>
                {
                    try
                    {
                        socket.EndConnect(ar);
                        tcs.SetResult(null!);
                    }
                    catch (Exception ex)
                    {
                        tcs.SetException(ex);
                    }
                }, null);
                return tcs.Task;
            }

            private Task<int> SendAsync(Socket socket, byte[] buffer, int offset, int count)
            {
                TaskCompletionSource<int> tcs = new TaskCompletionSource<int>();
                socket.BeginSend(buffer, offset, count, SocketFlags.None, (IAsyncResult ar) =>
                {
                    try
                    {
                        int bytesSent = socket.EndSend(ar);
                        tcs.SetResult(bytesSent);
                    }
                    catch (Exception ex)
                    {
                        tcs.SetException(ex);
                    }
                }, null);
                return tcs.Task;
            }

            private Task<int> ReceiveAsync(Socket socket, byte[] buffer, int offset, int count)
            {
                TaskCompletionSource<int> tcs = new TaskCompletionSource<int>();
                socket.BeginReceive(buffer, offset, count, SocketFlags.None, (IAsyncResult ar) =>
                {
                    try
                    {
                        int bytesRead = socket.EndReceive(ar);
                        tcs.SetResult(bytesRead);
                    }
                    catch (Exception ex)
                    {
                        tcs.SetException(ex);
                    }
                }, null);
                return tcs.Task;
            }

            private async Task ReceiveHeadersAsync()
            {
                _bufferPos = 0;
                _bufferSize = 0;

                while (true)
                {
                    _bufferSize = await ReceiveAsync(_socket!, _buffer, 0, _buffer.Length);
                    _bufferPos = 0;

                    if (_bufferSize == 0)
                    {
                        break;
                    }

                    // Look for end of headers (double CRLF)
                    int i = _bufferPos;
                    bool foundEnd = false;
                    while (i < _bufferSize - 1)
                    {
                        if (_buffer[i] == '\r' && _buffer[i + 1] == '\n')
                        {
                            if (i + 3 < _bufferSize && _buffer[i + 2] == '\r' && _buffer[i + 3] == '\n')
                            {
                                // Found end of headers
                                _headerBuilder.Append(Encoding.ASCII.GetString(_buffer, _bufferPos, i - _bufferPos + 2));
                                string headers = _headerBuilder.ToString();
                                ParseContentLength(headers);
                                _bufferPos = i + 4;
                                foundEnd = true;
                                break;
                            }
                        }
                        i++;
                    }

                    if (foundEnd)
                    {
                        break;
                    }

                    // Haven't found end of headers yet, need more data
                    _headerBuilder.Append(Encoding.ASCII.GetString(_buffer, _bufferPos, _bufferSize - _bufferPos));
                }
            }

            private void ParseContentLength(string headers)
            {
                string[] lines = headers.Split(new[] { "\r\n" }, StringSplitOptions.None);
                foreach (string line in lines)
                {
                    if (line.StartsWith("Content-Length:", StringComparison.OrdinalIgnoreCase))
                    {
                        string lengthStr = line.Substring(15).Trim();
                        if (int.TryParse(lengthStr, out int length))
                        {
                            _contentLength = length;
                            Console.WriteLine($"Content-Length for {_url}: {_contentLength}");
                            return;
                        }
                    }
                }
                Console.WriteLine($"Warning: Content-Length not found for {_url}");
            }

            private void Finish()
            {
                Console.WriteLine($"Download completed for {_url}");
                PrintHeadersWithSizes();
                _socket?.Close();
            }

            private void PrintHeadersWithSizes()
            {
                string headers = _headerBuilder.ToString();
                Console.WriteLine($"  Total headers size: {headers.Length} bytes");
                string[] lines = headers.Split(new[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (string line in lines)
                {
                    Console.WriteLine($"    [{line.Length} bytes] {line}");
                }
            }

        }

        public static async Task DownloadFilesAsync(string[] urls)
        {
            Console.WriteLine("\nAsync/Await Implementation");
            Task[] tasks = new Task[urls.Length];
            for (int i = 0; i < urls.Length; i++)
            {
                DownloadSession session = new DownloadSession(urls[i]);
                tasks[i] = session.DownloadAsync();
            }

            await Task.WhenAll(tasks);
        }
    }
}


