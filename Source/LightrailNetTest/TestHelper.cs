using System.Collections.Generic;
using System.IO;

namespace LightrailNetTest
{
    internal static class TestHelper
    {
        public static readonly string TestFilePath = @"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\c1xx.dll";

        public static bool CompareStringArrayUnstable(string[] arr1, string[] arr2)
        {
            if (arr1.Length != arr2.Length)
            {
                return false;
            }

            List<string> arr2list = new List<string>(arr2);
            for (int i = 0; i < arr1.Length; i++)
            {
                if (!arr2list.Contains(arr1[i]))
                {
                    return false;
                }
            }

            return true;
        }

        public static bool CompareStringArray(string[] arr1, string[] arr2)
        {
            if (arr1.Length != arr2.Length)
            {
                return false;
            }

            for (int i = 0; i < arr1.Length; i++)
            {
                if (arr1[i] != arr2[i])
                {
                    return false;
                }
            }

            return true;
        }

        public static bool CompareByteArray(byte[] arr1, byte[] arr2)
        {
            if (arr1.Length != arr2.Length)
            {
                return false;
            }

            for (int i = 0; i < arr1.Length; i++)
            {
                if (arr1[i] != arr2[i])
                {
                    return false;
                }
            }

            return true;
        }

        public static byte[] ReadTestFile()
        {
            FileInfo file = new FileInfo(TestFilePath);
            FileStream stream = file.OpenRead();

            List<byte> filedata = new List<byte>();

            byte[] buffer = new byte[10240];
            int read = stream.Read(buffer, 0, 10240);
            while (read > 0)
            {
                for (int i = 0; i < read; i++)
                {
                    filedata.Add(buffer[i]);
                }

                read = stream.Read(buffer, 0, 10240);
            }

            return filedata.ToArray();
        }
    }
}
