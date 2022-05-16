using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    public class Point<T>
    {
        public T X { get; set; }
        public T Y { get; set; }

        public Point() => (X, Y) = (default, default);
        public Point(T x, T y) => (X, Y) = (x, y);
    }
}
