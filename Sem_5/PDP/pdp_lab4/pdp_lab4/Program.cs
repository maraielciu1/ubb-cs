using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace pdp_lab4
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] urls = args.Length > 0 
                ? args 
                : new string[]
                {
                    "http://info.cern.ch/",
                    "http://example.com/"
                };

            Console.WriteLine("HTTP File Downloader - Three Implementations");
            Console.WriteLine("=============================================\n");

            try
            {
                Task callbackTask = CallbackBasedDownloader.DownloadFilesAsync(urls);
                callbackTask.Wait(); 

                // CallbackActionDownloader actionDownloader = new CallbackActionDownloader();
                // List<Task> callbackTasks = new List<Task>();

                // foreach (string url in urls)
                // {
                //     TaskCompletionSource<bool> tcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
                //     callbackTasks.Add(tcs.Task);
                //     actionDownloader.StartDownload(url, () => tcs.TrySetResult(true));
                // }

                // Task.WhenAll(callbackTasks).Wait();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in callback-based implementation: {ex.Message}");
            }

            try
            {
                Task task2 = TaskBasedDownloader.DownloadFilesAsync(urls);
                task2.Wait(); 
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in task-based implementation: {ex.Message}");
            }

            try
            {
                Task task3 = AsyncAwaitDownloader.DownloadFilesAsync(urls);
                task3.Wait(); 
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in async/await implementation: {ex.Message}");
            }

            Console.WriteLine("\nAll implementations completed!");
        }
    }
}