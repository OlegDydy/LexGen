using System;
using System.Collections.Generic;
using System.Linq;

namespace LexGen
{
    public enum nodetype
    {
        None, Char, Charset, AnyChar,
        WhiteSpace, Digit, Word, Block, Repeat, Token
    };

    public class EIndeterminate : Exception
    {
        public Int32 id;
        public EIndeterminate(String Message, Int32 id):
            base(Message)
        {
            this.id = id;
        }
    };

    public class node_t
    {

        public Int32 id;
        public nodetype nt;
        public bool invert;// nodetype.AnyChar - include \r \n
        public bool greedy;
        public Int32 A;// nodetype.Char - char; nodetype.Repeat - minCount; 
        public Int32 B;// ; nodetype.Repeat - maxCount
        public node_t parent;
        public List<node_t> children;
        public SortedSet<char> set;

        public Int32 next_id()
        {
            Int32 result = A;
            A++;
            return result;
        }

        public node_t()
        {
            children = new List<node_t>();
            set = new SortedSet<char>();
            id = 0;
            nt = nodetype.None;
            greedy = false;
            invert = false;
            A = 0;
            B = 0;
            parent = null;
        }
    }

    class RegularGenerator
    {
        //private string NOT_IMPLEMENTED_STR = "NOT IMLEMENTED YET";
        private string INVALID_REGULAR_STR = "Invalid regular expression";
        private string INDETERMINATE_STR = "Indeterminate behavior : Two tokens have the same form.Please check tokens : ";


        public RegularGenerator()
        {
            root = new node_t();
            elements = new List<node_t>();
            elements.Add(root);
        }

        public node_t root;
        public List<node_t> elements;

        static public bool ws(char ch)
        {
            return ch == ' ' || ch == '\f' ||
              ch == '\t' || ch == '\v' || ch == '\n' || ch == '\r';
        }

        static public bool digit(char ch)
        {
            return '0' <= ch && ch <= '9';
        }

        static public bool word(char ch)
        {
            return 'a' <= ch && ch <= 'z' ||
              'A' <= ch && ch <= 'Z' ||
              '0' <= ch && ch <= '9' ||
              ch == '_';
        }

        // operand "."
        static public bool any(char ch, bool inlude_linebreak)
        {
            if (inlude_linebreak)
                return true;
            else
                return ch != '\r' && ch != '\n';
        }

        // вставка нового потомка
        private node_t insert_child(node_t parent, node_t child)
        {
            // начальный структурный блок
            if (parent.parent == root)
            {
                node_t node = insert_child(root, child);
                if (node != child)
                {
                    // Аналогичный узел уже есть
                    child = node;
                    parent.children.Add(child);
                    return child;
                }
            }
            node_t cur = parent;
            if (!(parent.children.Count == 0))
            {
                // Переход на выдачу токена должен быть если идти больше некуда
                if (child.nt == nodetype.Token)
                {
                    // выдавать мы можем только один токен иначе неопределенность
                    if (cur.children.Last().nt == nodetype.Token)
                        throw new EIndeterminate(INDETERMINATE_STR, cur.A);
                    cur.children.Add(child);
                }
                else
                {
                    int i = 0;
                    // вставка перед выдачей токена если он есть
                    for (; i < cur.children.Count; i++)
                    {
                        if (cur.children[i].nt == nodetype.Token)
                            break;
                        if (cur.children[i].nt == child.nt)
                        {
                            // проверим, а не пытаемся ли мы сунуть повторяющийся элемент
                            switch (child.nt)
                            {
                                case nodetype.Char:
                                    // если добавляем повторяющийся символ
                                    if (child.A == cur.children[i].A)
                                        return cur.children[i];
                                    break;
                                case nodetype.Charset:
                                    // если добавляем повторяющийся класс символов
                                    if (child.set == cur.children[i].set)
                                        return cur.children[i];
                                    break;
                                case nodetype.AnyChar:
                                case nodetype.WhiteSpace:
                                case nodetype.Digit: 
                                case nodetype.Word:
                                    // достаточно встретить
                                    if (cur.children[i].invert == child.invert)
                                        return cur.children[i];
                                    else
                                        throw new Exception("WTF!");// такого быть не должно по идее
                                case nodetype.Block:
                                    throw new Exception("WTF!");// такого быть не должно по идее
                                case nodetype.Repeat:
                                    if (cur.children[i].A == child.A && cur.children[i].B == child.B)
                                        return cur.children[i];
                                    break;
                            }
                        }
                    }
                    cur.children.Insert(i, child);
                }
            }
            else
            {
                parent.children.Add(child);
            }

            // всплывааем 
            // добавляем ссылки на блок родителю и переходим к нему
            if (parent.nt == nodetype.Repeat && parent.A == 0)
                insert_child(parent.parent, child);

            if (parent.parent == root && child.nt == nodetype.Token)
                throw new Exception("Regular expression has empty sequence");
            return child;
        }

        // добавить выходной узел
        void add_block_exit(node_t node, node_t block_end)
        {
            if (node.children.Count == 0)
            {
                if (node == block_end)
                    // пришли по другой ветке
                    return;
                node.children.Add(block_end);
            }
            else
                foreach(node_t item in node.children)
                {
                    if (item.nt == nodetype.Token)
                        break;
                    add_block_exit(item, block_end);
                }
        }

        // добавить структурный блок
        node_t add_child_block(node_t block_start)
        {
            node_t tmp = new node_t();
            tmp.parent = block_start;
            tmp.nt = nodetype.Block;
            elements.Add(tmp);

            // для всех выходов из блока добавим ссылку на закрывающий узел
            add_block_exit(block_start, tmp);

            // переходим в конец блока
            return tmp;
        }

        // добавить узел "один символ"
        node_t add_child_char(node_t parent, UInt32 ch)
        {
            if (parent.nt == nodetype.Block)
                parent.nt = nodetype.None;// структурный блок

            node_t child = new node_t();
            child.nt = nodetype.Char;
            child.parent = parent;
            child.A = (Int32)ch;

            node_t tmp = insert_child(parent, child);
            if (tmp != child)
            {
                //delete child;
                child = tmp;
            }
            else
            {
                child.id = root.next_id();
                elements.Add(child);
            }
            return child;
        }

        // добавить специфичного потомка (пресеты)
        node_t add_child(node_t parent, nodetype node_type)
        {
            if (node_type == nodetype.Repeat || node_type == nodetype.Char || node_type == nodetype.Block)
                throw new Exception("Use specialized function");

            if (parent.nt == nodetype.Block)
                parent.nt = nodetype.None;

            node_t child = new node_t();
            child.nt = node_type;
            child.parent = parent;

            node_t tmp = insert_child(parent, child);
            if (tmp != child)
            {
                // такой блок уже был
                //delete child;
                return tmp;
            }
            else
            {
                // У структурных блоков нет ID
                if (node_type != nodetype.None && node_type != nodetype.Token)
                    child.id = root.next_id();
                elements.Add(child);
            }

            return child;
        }

        // добавить потомка типа "цикл"
        node_t add_child_repeat(node_t parent, Int32 begin, Int32 end, bool greedy)
        {
            if (parent.nt == nodetype.Block)
            {
                // смена типа
                parent.nt = nodetype.Repeat;
                parent.A = begin;
                parent.B = end;
                parent.greedy = greedy;
            }
            else
            {
                // цикл как и блок не имеет собственного id поскольку является структурным
                node_t child = new node_t();
                child.nt = nodetype.Repeat;
                child.A = begin;
                child.B = end;
                child.greedy = greedy;
                // начало цикла должно быть выше на одну позицию
                child.parent = parent.parent;
                node_t tmp = insert_child(parent, child);
                if (tmp != child)
                {
                    // такой цикл уже был
                    //delete child;
                    child = tmp;
                }
                parent = child;
            }
            return parent;
        }

        String num = "0123456789";
        String wd = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        // получить корень дерева
        public node_t get_root()
        {
            return root;
        }

        // перевести из шестнадцатеричной системы в двоичную
        Int32 hex2int(char c)
        {
            if ('0' <= c && c <= '9')
                return c - '0';
            if ('a' <= c && c <= 'f')
                return 10 + c - 'a';
            if ('A' <= c && c <= 'F')
                return 10 + c - 'A';
            return -1;
        }

        //1. Cтроим конечный автомат в виде графа со ссылками

        void exclude_range(ref bool range, ref node_t cur)
        {
            if (range) { range = false; cur.set.Add('-'); }
        }

        void special_range(ref bool range, ref node_t cur, char start, ref char stop, char ch)
        {
            if (range)
            {
                range = false;
                stop = ch;
                if (stop < start)
                    throw new Exception(INVALID_REGULAR_STR);
                for (char i = start; i <= stop; i++)
                    cur.set.Add(i);
            }
            else
                cur.set.Add(ch);
        }

        // добавть новую ветвь автомата
        public void add_branch(String expr, Int32 token_id)
        {
            const int max_stack_len = 64;
            if (expr.Length == 0)
                return;

            char start = '\0', stop = '\0';
            node_t[] entrances = new node_t[max_stack_len];
            bool screen = false;
            int top = 0;
            bool charset = false;
            bool first = false;
            bool range = false;

            bool modifiers = false;
            bool coment = false;
            bool unset = false;
            bool any_symbol = false;
            bool any_case = false;

            node_t cur = add_child(root, nodetype.None);
            entrances[top] = cur;

            for (int i = 0; i < expr.Length; i++)
            {
                if (coment)
                {
                    if (expr[i] == ')')
                        coment = false;
                    continue;
                }
                if (modifiers)
                {
                    switch (expr[i])
                    {
                        case 's': any_symbol = !unset; break;
                        case 'i': any_case = !unset; break;
                        case '-':
                            if (unset)
                                throw new Exception(INVALID_REGULAR_STR);
                            unset = true;
                            break;
                        case ':': modifiers = false; break;
                        case ')': modifiers = false; top--; break;
                        case '#':
                            if (expr[i - 1] == '?')
                            {
                                modifiers = false;
                                coment = true;
                                break;
                            }
                            else
                                throw new Exception(INVALID_REGULAR_STR);
                        default:
                            throw new Exception(INVALID_REGULAR_STR);
                    }
                    continue;
                }
                // экранирование
                if (screen)
                {
                    screen = false;
                    // различные спецсимволы полученные из обычных
                    switch (expr[i])
                    {
                        case 'x': // ANSI
                        case 'u':
                            {// UTF-16
                                char ch = '\0';
                                Int32 t;
                                int j;
                                int len = (expr[i] == 'x') ? 1 : 3;// count - 1

                                // Достаточно ли в строке элементов?
                                if (i + len + 1 >= expr.Length)
                                {
                                    // если нет то игнорируем специальное значение
                                    if (charset)
                                        special_range(ref range, ref cur, start, ref stop, expr[i]);
                                    else
                                    {
                                        cur = add_child_char(cur, expr[i]);
                                    }
                                }
                                else
                                {
                                    // если Достаточно, то преобразуем в символ
                                    for (j = 0; j <= len; j++)
                                    {
                                        t = hex2int(expr[i + j + 1]);
                                        if (t < 16)
                                            // помещаем 16-ричную цифру в символ
                                            ch |= (char)(t << (4 * (len - j)));
                                        else
                                        {
                                            // встретили символ не являющийся 16-ричным числом
                                            // игнорируем специальное значение
                                            if (charset)
                                                special_range(ref range, ref cur,start, ref stop, expr[i]);
                                            else
                                                cur = add_child_char(cur, expr[i]);
                                            break;
                                        }
                                    }
                                    if (j > len)
                                    {
                                        // не игнорируем специальное значение
                                        cur = add_child_char(cur, ch);
                                        i += j;
                                    }
                                }
                            }
                            break;
                        case 'r':
                            { /*возврат каретки*/
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, '\r');
                                else
                                {
                                    cur = add_child_char(cur, '\r');
                                }
                                break;
                            }
                        case 'n':
                            { /*перевод строки*/
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, '\n');
                                else
                                {
                                    cur = add_child_char(cur, '\n');
                                }
                                break;
                            }
                        case 't':
                            { /*табуляция*/
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, '\t');
                                else
                                {
                                    cur = add_child_char(cur, '\t');
                                }
                                break;
                            }
                        case 'v':
                            { /*вертикальная табуляция*/
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, '\v');
                                else
                                {
                                    cur = add_child_char(cur, '\v');
                                }
                                break;
                            }
                        case 'f':
                            { /*разрыв страницы*/
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, '\f');
                                else
                                {
                                    cur = add_child_char(cur, '\v');
                                }
                                break;
                            }
                        // Встроенные классы символов
                        case 'd':
                            { /*Цифры*/
                                if (charset)
                                {
                                    exclude_range(ref range, ref cur);
                                    for (int j = 0; j < num.Length; cur.set.Add(num[j]), j++) ;
                                }
                                else
                                    cur = add_child(cur, nodetype.Digit);
                                break;
                            }
                        case 'D':
                            { /*Не Цифры*/
                                if (charset)
                                {
                                    exclude_range(ref range, ref cur);
                                    for (int j = '\0'; j <= '\xffff'; j++)
                                        if (j < '0' || j > '9')
                                            cur.set.Add((char)j);
                                }
                                else
                                {
                                    cur = add_child(cur, nodetype.Digit);
                                    cur.invert = true;
                                }
                                break;
                            }
                        case 's':
                            { /*Пробельные символы*/
                                if (charset)
                                {
                                    exclude_range(ref range, ref cur);
                                    cur.set.Add(' ');
                                    cur.set.Add('\v');
                                    cur.set.Add('\t');
                                    cur.set.Add('\r');
                                    cur.set.Add('\n');
                                    cur.set.Add('\f');
                                }
                                else
                                    cur = add_child(cur, nodetype.WhiteSpace);
                                break;
                            }
                        case 'S':
                            { /*Непробельные символы*/
                                if (charset)
                                {
                                    exclude_range(ref range, ref cur);
                                    for (int j = '\0'; j <= '\xffff'; j++)
                                        if (!ws((char)j))
                                            cur.set.Add((char)i);
                                }
                                else
                                {
                                    cur = add_child(cur, nodetype.WhiteSpace);
                                    cur.invert = true;
                                }
                                break;
                            }
                        case 'w':
                            /*Буквенный или цифровой или подчеркивание*/
                            if (charset)
                            {
                                exclude_range(ref range, ref cur);
                                for (int j = 0; j < wd.Length; j++)
                                    cur.set.Add(wd[j]);
                                for (int j = 0; j < num.Length; j++)
                                    cur.set.Add(num[j]);
                                cur.set.Add('_');
                            }
                            else
                                cur = add_child(cur, nodetype.Word);
                            break;

                        case 'W': /*не Буквенный не цифровой и не подчеркивание*/
                            if (charset) {
                                exclude_range(ref range, ref cur);
                                for (int j = '\0'; j <= '\xffff'; j++)
                                    if (!word((char)j))
                                        cur.set.Add(num[i]);
                            }
                            else {
                                cur = add_child(cur, nodetype.Word);
                                cur.invert = true;
                            }
                            break;
                        // Все символы не являющиеся управляющими будут записаны как обычно
                        default: {
                                if (charset)
                                    special_range(ref range, ref cur,start, ref stop, expr[i]);
                                else
                                    cur = add_child_char(cur, expr[i]);
                                break; }
                    }
                    continue;
                }

                // Обычный режим
                switch (expr[i])
                {
                    // экранировать символ
                    case '\\': {
                            screen = true;
                            break; }
                    // начало символьного класса
                    case '[':
                        {
                            if (charset)
                            {
                                cur.set.Add('[');
                                break;
                            }
                            else
                            {
                                cur = add_child(cur, nodetype.Charset);
                                first = true;
                                charset = true;
                                continue;
                            }
                        }
                    // конец символьного класса
                    case ']':
                        charset = false;
                        if (range)
                            cur.set.Add('-');
                        if (cur.set.Count == 0)
                            throw new Exception(INVALID_REGULAR_STR);
                        break;
                    // начало блока
                    case '(':
                        // запоминаем точку возврата
                        if (charset)
                            special_range(ref range, ref cur,start, ref stop, '(');
                        else
                        if (top < max_stack_len) {
                            entrances[++top] = cur;
                        }
                        else
                            throw new Exception("The maximum depth of nesting is reached (64)");
                        break;
                    // Конец блока
                    case ')':
                        {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, ')');
                            else
                                cur = add_child_block(entrances[top--]);
                        } break;
                    // оператор "или"
                    case '|':
                        {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, '|');
                            else
                                cur = entrances[top];
                        } break;
                    // интервал
                    case '-':
                        if (charset)
                        {
                            if (first)
                                special_range(ref range, ref cur,start, ref stop, '-');
                            else {
                                start = expr[i - 1];
                                range = true;
                            }
                        }
                        else {
                            cur = add_child_char(cur, '-');
                        }
                        break;
                    // Любой символ
                    case '.': {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, '.');
                            else {
                                cur = add_child(cur, nodetype.AnyChar);
                                if (any_symbol)
                                    cur.invert = true;
                            }
                        } break;
                    // повторить 0+ раз (n >= 0)
                    case '*': {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, '*');
                            else {
                                if (cur.nt == nodetype.Repeat)
                                    // из цикла не может идти еще один цикл
                                    cur = add_child_char(cur, '*');
                                else {
                                    // позитивный просмотр вперед
                                    if (i + 1 < expr.Length && expr[i + 1] == '?') {
                                        cur = add_child_repeat(cur, 0, 0, false);
                                        i++;
                                    }
                                    else
                                        cur = add_child_repeat(cur, 0, 0, true);
                                }
                            }
                        } break;
                    // повторить 1+ раз (n > 0)
                    case '+': {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, '+');
                            else {
                                if (cur.nt == nodetype.Repeat)
                                    // из цикла не может идти еще один цикл
                                    cur = add_child_char(cur, '+');
                                else
                                  // позитивный просмотр вперед
                                  if (i + 1 < expr.Length && expr[i + 1] == '?') {
                                    cur = add_child_repeat(cur, 1, 0, false);
                                    i++;
                                }
                                else
                                    cur = add_child_repeat(cur, 1, 0, true);
                            }
                        } break;
                    // повторить 0 или 1 раз
                    case '?': {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, '?');
                            else {
                                if (cur == entrances[top]) {
                                    // начало особой секции
                                    modifiers = true;
                                } else
                                if (cur.nt == nodetype.Repeat) {
                                    // из цикла не может идти еще один цикл
                                    cur = add_child_char(cur, '?');
                                }
                                else
                                  // позитивный просмотр вперед
                                  if (i + 1 < expr.Length && expr[i + 1] == '?') {
                                    cur = add_child_repeat(cur, 0, 1, false);
                                    i++;
                                }
                                else
                                    cur = add_child_repeat(cur, 0, 1, true);
                            }
                        } break;
                    // все не специфичные символы
                    default: {
                            if (charset)
                                special_range(ref range, ref cur,start, ref stop, expr[i]);
                            else
                                cur = add_child_char(cur, expr[i]);
                        } break;
                }
                if (charset)
                    first = false;
            }

            if (coment || modifiers)
                throw new Exception(INVALID_REGULAR_STR);

            // выходной узел
            cur = add_child(cur, nodetype.Token);
            cur.A = token_id;

        }
    }
}
