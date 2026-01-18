using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace pdp_lab4
{
    public class CallbackBasedDownloader
    {
        private enum State
        {
            Connecting,
            Sending,
            ReceivingHeaders,
            Completed
        }

        private class DownloadSession
        {
            private readonly string _url;
            private readonly string _host;
            private readonly string _path;
            private readonly int _port;
            private Socket? _socket;
            private State _state;
            private readonly byte[] _buffer;
            private int _bufferPos;
            private int _bufferSize;
            private readonly StringBuilder _headerBuilder;
            private int _contentLength;
            private bool _isCompleted;
            private readonly TaskCompletionSource<object?> _completionSource;

            public DownloadSession(string url)
            {
                _url = url;
                ParseUrl(url, out _host, out _path, out _port);
                _buffer = new byte[4096];
                _headerBuilder = new StringBuilder();
                _state = State.Connecting;
                _completionSource = new TaskCompletionSource<object?>(TaskCreationOptions.RunContinuationsAsynchronously);
            }

            public Task CompletionTask => _completionSource.Task;

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
                    // Port specified
                    host = url.Substring(0, colonIndex);
                    int portEnd = slashIndex == -1 ? url.Length : slashIndex;
                    port = int.Parse(url.Substring(colonIndex + 1, portEnd - colonIndex - 1));
                    path = slashIndex == -1 ? "/" : url.Substring(slashIndex);
                }
                else
                {
                    // No port specified, default to 80
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

            public void Start()
            {
                try
                {
                    IPHostEntry hostEntry = Dns.GetHostEntry(_host);
                    IPAddress ipAddress = hostEntry.AddressList[0];
                    IPEndPoint remoteEP = new IPEndPoint(ipAddress, _port);

                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    _socket.BeginConnect(remoteEP, OnConnect, null);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error starting download for {_url}: {ex.Message}");
                    Fail(ex);
                }
            }

            private void OnConnect(IAsyncResult ar)
            {
                try
                {
                    _socket!.EndConnect(ar);
                    Console.WriteLine($"Connected to {_host}:{_port} for {_url}");

                    _state = State.Sending;
                    string request = $"GET {_path} HTTP/1.1\r\nHost: {_host}\r\nConnection: close\r\n\r\n";
                    byte[] requestBytes = Encoding.ASCII.GetBytes(request);
                    _socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, OnSend, null);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error connecting for {_url}: {ex.Message}");
                    _socket?.Close();
                    Fail(ex);
                }
            }

            private void OnSend(IAsyncResult ar)
            {
                try
                {
                    _socket!.EndSend(ar);
                    _state = State.ReceivingHeaders;
                    _bufferPos = 0;
                    _bufferSize = 0;
                    _socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, OnReceive, null);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error sending request for {_url}: {ex.Message}");
                    _socket?.Close();
                    Fail(ex);
                }
            }

            private void OnReceive(IAsyncResult ar)
            {
                try
                {
                    int bytesRead = _socket!.EndReceive(ar);
                    if (bytesRead == 0)
                    {
                        if (_state == State.ReceivingHeaders)
                        {
                            ProcessHeaders();
                        }
                        Finish();
                        return;
                    }

                    _bufferSize = bytesRead;
                    _bufferPos = 0;

                    if (_state == State.ReceivingHeaders)
                    {
                        ProcessHeaders();
                    }

                    if (_state != State.Completed)
                    {
                        _socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, OnReceive, null);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error receiving data for {_url}: {ex.Message}");
                    _socket?.Close();
                    Fail(ex);
                }
            }

            private void ProcessHeaders()
            {
                while (_bufferPos < _bufferSize)
                {
                    int i = _bufferPos;
                    while (i < _bufferSize - 1)
                    {
                        if (_buffer[i] == '\r' && _buffer[i + 1] == '\n')
                        {
                            if (i + 3 < _bufferSize && _buffer[i + 2] == '\r' && _buffer[i + 3] == '\n')
                            {
                                _headerBuilder.Append(Encoding.ASCII.GetString(_buffer, _bufferPos, i - _bufferPos + 2));
                                string headers = _headerBuilder.ToString();
                                ParseContentLength(headers);
                                _bufferPos = i + 4;
                                _state = State.Completed;
                                Finish();
                                return;
                            }
                        }
                        i++;
                    }

                    _headerBuilder.Append(Encoding.ASCII.GetString(_buffer, _bufferPos, _bufferSize - _bufferPos));
                    _bufferPos = _bufferSize;
                    return;
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
                if (_isCompleted)
                {
                    return;
                }

                _isCompleted = true;

                Console.WriteLine($"Download completed for {_url}");
                PrintHeadersWithSizes();
                _socket?.Close();
                _completionSource.TrySetResult(null);
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
            private void Fail(Exception ex)
            {
                if (_isCompleted)
                {
                    return;
                }

                _isCompleted = true;
                Console.WriteLine($"Download failed for {_url}: {ex.Message}");
                _socket?.Close();
                _completionSource.TrySetResult(null);
            }
        }

        public static Task DownloadFilesAsync(string[] urls)
        {
            Console.WriteLine("\nCallback-Based Implementation");
            DownloadSession[] sessions = new DownloadSession[urls.Length];
            for (int i = 0; i < urls.Length; i++)
            {
                sessions[i] = new DownloadSession(urls[i]);
                sessions[i].Start();
            }

            Task[] tasks = new Task[urls.Length];
            for (int i = 0; i < urls.Length; i++)
            {
                tasks[i] = sessions[i].CompletionTask;
            }

            return Task.WhenAll(tasks);
        }
    }
}

