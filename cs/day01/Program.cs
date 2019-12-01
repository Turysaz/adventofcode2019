using System;
using System.IO;
using System.Linq;

namespace day01
{
    class Program
    {
        static void Main(string[] args)
        {
            int Fuel(int mass, bool recursive = false)
            {
                var f = (int)(Math.Floor(mass/3.0)) - 2;

                if (recursive == false)
                {
                    return f;
                }
                
                return f < 0 ? 0 : f + Fuel(f, true);
            }

            var modules = File.ReadAllLines("../../inputs/day01.txt")
                .Select(l => Int32.Parse(l));

            // Part 1
            Console.WriteLine(modules.Select(module => Fuel(module)).Sum());

            // Part 2
            Console.WriteLine(modules.Select(
                    module => Fuel(module, true))
                .Sum());
        }
    }
}
