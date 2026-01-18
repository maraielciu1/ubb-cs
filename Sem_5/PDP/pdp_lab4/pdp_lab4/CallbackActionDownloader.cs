using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace pdp_lab4
{
    /// <summary>
    /// Alternative callback-based downloader that signals completion through an Action callback,
    /// mirroring the sample approach provided by the professor.
    /// </summary>
    public class CallbackActionDownloader
    {
        private class DownloadState
        {
            public Socket? Socket;
            public readonly string Url;
            public readonly string Host;
            public readonly string Path;
            public readonly int Port;
            public readonly byte[] Buffer = new byte[4096];
            public int BufferPos;
            public int BufferSize;
            public readonly StringBuilder HeaderBuilder = new StringBuilder();
            public int ContentLength;
            public bool IsCompleted;
            public Action? OnComplete;

            public DownloadState(string url, Action onComplete)
            {
                Url = url;
                OnComplete = onComplete;
                ParseUrl(url, out Host, out Path, out Port);
            }

            private static void ParseUrl(string url, out string host, out string path, out int port)
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
        }

        public void StartDownload(string url, Action onComplete)
        {
            DownloadState state = new DownloadState(url, onComplete);
            try
            {
                IPHostEntry hostEntry = Dns.GetHostEntry(state.Host);
                IPAddress ipAddress = hostEntry.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, state.Port);

                state.Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                state.Socket.BeginConnect(remoteEP, OnConnect, state);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[CallbackAction] Error starting {url}: {ex.Message}");
                state.OnComplete?.Invoke();
            }
        }

        private void OnConnect(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState!;
            try
            {
                state.Socket!.EndConnect(ar);
                Console.WriteLine($"[CallbackAction] Connected to {state.Host}:{state.Port} for {state.Url}");

                string request = $"GET {state.Path} HTTP/1.1\r\nHost: {state.Host}\r\nConnection: close\r\n\r\n";
                byte[] requestBytes = Encoding.ASCII.GetBytes(request);
                state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, OnSend, state);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[CallbackAction] Connect error for {state.Url}: {ex.Message}");
                Fail(state);
            }
        }

        private void OnSend(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState!;
            try
            {
                state.Socket!.EndSend(ar);
                state.BufferPos = 0;
                state.BufferSize = 0;
                state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, OnReceive, state);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[CallbackAction] Send error for {state.Url}: {ex.Message}");
                Fail(state);
            }
        }

        private void OnReceive(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState!;
            try
            {
                int bytesRead = state.Socket!.EndReceive(ar);
                if (bytesRead == 0)
                {
                    if (!state.IsCompleted)
                    {
                        ProcessHeaders(state);
                    }
                    Finish(state);
                    return;
                }

                state.BufferSize = bytesRead;
                state.BufferPos = 0;
                if (!state.IsCompleted)
                {
                    ProcessHeaders(state);
                }

                if (!state.IsCompleted)
                {
                    state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, OnReceive, state);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[CallbackAction] Receive error for {state.Url}: {ex.Message}");
                Fail(state);
            }
        }

        private static void ProcessHeaders(DownloadState state)
        {
            while (state.BufferPos < state.BufferSize)
            {
                int i = state.BufferPos;
                while (i < state.BufferSize - 1)
                {
                    if (state.Buffer[i] == '\r' && state.Buffer[i + 1] == '\n')
                    {
                        if (i + 3 < state.BufferSize && state.Buffer[i + 2] == '\r' && state.Buffer[i + 3] == '\n')
                        {
                            state.HeaderBuilder.Append(Encoding.ASCII.GetString(state.Buffer, state.BufferPos, i - state.BufferPos + 2));
                            string headers = state.HeaderBuilder.ToString();
                            ParseContentLength(state, headers);
                            state.BufferPos = i + 4;
                            Finish(state);
                            return;
                        }
                    }
                    i++;
                }

                state.HeaderBuilder.Append(Encoding.ASCII.GetString(state.Buffer, state.BufferPos, state.BufferSize - state.BufferPos));
                state.BufferPos = state.BufferSize;
                return;
            }
        }

        private static void ParseContentLength(DownloadState state, string headers)
        {
            string[] lines = headers.Split(new[] { "\r\n" }, StringSplitOptions.None);
            foreach (string line in lines)
            {
                if (line.StartsWith("Content-Length:", StringComparison.OrdinalIgnoreCase))
                {
                    string lengthStr = line.Substring(15).Trim();
                    if (int.TryParse(lengthStr, out int length))
                    {
                        state.ContentLength = length;
                        Console.WriteLine($"[CallbackAction] Content-Length for {state.Url}: {state.ContentLength}");
                        return;
                    }
                }
            }
            Console.WriteLine($"[CallbackAction] Warning: Content-Length not found for {state.Url}");
        }

        private static void Finish(DownloadState state)
        {
            if (state.IsCompleted)
            {
                return;
            }

            state.IsCompleted = true;
            Console.WriteLine($"[CallbackAction] Download completed for {state.Url}");
            PrintHeadersWithSizes(state);
            state.Socket?.Close();
            state.OnComplete?.Invoke();
        }

        private static void Fail(DownloadState state)
        {
            if (state.IsCompleted)
            {
                return;
            }

            state.IsCompleted = true;
            Console.WriteLine($"[CallbackAction] Download failed for {state.Url}");
            state.Socket?.Close();
            state.OnComplete?.Invoke();
        }

        private static void PrintHeadersWithSizes(DownloadState state)
        {
            string headers = state.HeaderBuilder.ToString();
            Console.WriteLine($"  Total headers size: {headers.Length} bytes");
            string[] lines = headers.Split(new[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string line in lines)
            {
                Console.WriteLine($"    [{line.Length} bytes] {line}");
            }
        }
    }
}

