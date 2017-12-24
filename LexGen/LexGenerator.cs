using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace LexGen
{
    class LexGenerator
    {

        private enum token_t
        {
            unknown, none,
            token, _operator, keyword, mute, sysyem,
            ident, expr, eol, eof
        };

        // Описание Компилируемого Токена (Имя и как его найти)
        struct token_description
        {
            public token_t type;
            public string name;
            public string expr;
        };

        // Здесь храним все определения
        static private List<token_description> token_descs = new List<token_description>();
        static public String ErrStr = "";

        static private int ItemCmp(token_description a, token_description b)
        {

            if (a.type < b.type)
                return -1;
            if (a.type > b.type)
                return 1;
            return a.name.CompareTo(b.name);
        }
        
        static public int Gen(String src, out String header, out String body, String UnitName)
        {
            RegularGenerator gen = new RegularGenerator();
            String tmp = "";
            bool first = true;
            StringBuilder ss = new StringBuilder();
            foreach (char ch in UnitName)
            {
                if ('a' <= ch && ch <= 'z' || 
                    'A' <= ch && ch <= 'Z' ||
                    '0' <= ch && ch <= '9' ||
                    ch == '_')
                {
                    if (first && '0' <= ch && ch <= '9')
                        ss.Append('_',1);
                    ss.Append(ch);
                }
                if (ch == ' ')
                    ss.Append('_');
                first = false;
            }
            if (ss.Length == 0)
                UnitName = "untitled";
            else
                UnitName = ss.ToString();

            init(ref gen);

            int i = 0;
            int j;
            int line = 1;
            int line_begin = -1;
            bool mute_expr = false;

            // обработка
            while (i < src.Length)
            {
                // ищем начало
                for (; i < src.Length && RegularGenerator.ws(src[i]); i++)
                {
                    if (src[i] == '\n')
                    {
                        // просто начало строки (вместо разделения точкой с запятой)
                        process_token(token_t.eol, "\n");
                        mute_expr = false;
                        line++;
                        line_begin = i;
                    }
                }
                if (i == src.Length) break;

                j = i;

                switch (src[i])
                {
                    case '#':
                        // комментарий
                        for (i++; i < src.Length && src[i] != '\n'; i++) ;
                        continue;
                    // начало выражения
                    case '(':
                        {
                            if (mute_expr)
                                break;
                            // начало выражения потом его в собственный компилятор загоним
                            int brack = 1;
                            bool screen = false;
                            for (i++; i < src.Length && brack > 0 && src[i] != '\n'; i++)
                            {
                                if (screen)
                                    // экранироване действует лишь на один символ
                                    // что конкретно экранируется сейчас не особо важно
                                    screen = false;
                                else
                                {
                                    // не экранированные символы участвуют в подсчете
                                    if (src[i] == '(') brack++;
                                    else if (src[i] == ')') brack--;
                                    else if (src[i] == '\\') screen = true;
                                }
                            }
                            tmp = src.Substring(j, i - j);

                            // очищаем от внешних скобок они больше не нужны
                            if (brack > 1)
                            {
                                // где-то в выражении есть незакрытые скобки
                                ErrStr += $"[ERROR] unbalanced brackets at {line}:{j - line_begin}";
                                header = "";
                                body = "";
                                return -1;
                            }
                            else if (brack == 1)
                            {
                                // одну скобку мы можем закрыть, так как точно знаем что это за скобка
                                // (с неё начинается любое регулярное выражение)
                                ErrStr += $"[WARNING] expression not closed at {line}:{j - line_begin}";
                            }
                            else
                                tmp = tmp.Remove(tmp.Length - 1);
                            tmp = tmp.Remove(0, 1);

                            // отправляем на дальнейшую обработку

                            try
                            {
                                process_token(token_t.expr, tmp);
                            }
                            catch (Exception e)
                            {
                                ErrStr += $"[ERROR] {e.Message} \"{tmp}\" at {line}:{i - line_begin - tmp.Length}";
                                header = "";
                                body = "";
                                return -1;
                            }
                            continue;
                        }
                }

                // найдем идентификатор
                for (; i < src.Length && !RegularGenerator.ws(src[i]); i++) ;
                tmp = src.Substring(j, i - j);
                // уточним найденный идентификатор (ключевое слово/идентификатор)
                token_t token = recognize(tmp);
                // отправка на обраотку
                try
                {
                    process_token(token, tmp);
                }
                catch (Exception e)
                {
                    ErrStr += $"[ERROR] {e.Message} \"{tmp}\" at {line}:{i - line_begin - tmp.Length}";
                    header = "";
                    body = "";
                    return -1;
                }
            }
            process_token(token_t.eof, tmp);
            // Компиляция включаемых файлов

            token_descs.Sort(ItemCmp);

            Int32 id = 1;
            foreach (token_description item in token_descs)
            {
                try
                {
                    // добавить ветвь в дерево состояний
                    if (item.type != token_t.sysyem)
                        if (item.type == token_t.mute)
                            gen.add_branch(item.expr, 0);
                        else
                            gen.add_branch(item.expr, id);
                    id++;
                }
                catch (EIndeterminate e)
                {
                    ErrStr = $"[ERROR] {e.Message} \"{tmp}\" at {line}:{i - line_begin - tmp.Length}";
                    header = "";
                    body = "";
                    return -1;
                }
                catch (Exception e)
                {
                    ErrStr = $"[ERROR] {e.Message}";
                    header = "";
                    body = "";
                    return -1;
                }
            }

            // Заголовочный файл
            write_header(out header,UnitName);
            // Конеченный автомат и вспомогательные функци
            write_source_file(out body, UnitName, ref gen);

            return 0;
        }

        static String PROGRAM_VERSION = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString(3);

        static void write_header(out String output, String UnitName)
        {
            // Шапка
            output =
              $"// Auto generated file by LexGen program ver: {PROGRAM_VERSION}\n" +
              "// Please don't edit manually\n" +
              "#pragma once\n" +
              $"#ifndef {UnitName}_h\n" +
              $"#define {UnitName}_h\n" +
              "\n";
            // вывод всех используемых токенов
            {
                String ss = "";
                int begin_string = 0;
                ss += "enum token_t { t_unexpected = -1,\n\t";
                foreach (token_description t in token_descs) {
                    if (ss.Length + t.name.Length + 2 - begin_string > 80)
                    {
                        ss += "\n\t";
                        begin_string = ss.Length - 1;
                    }
                    ss += $"{t.name} , ";
                }
                ss += "\n};\n\n";
                output += ss;
            }
            // заголовки экспортируемых функций
            output +=
              "#define lchar char\n" +
            "\n" +
            "struct selection_t {\n" +
            "\tsize_t begin, end;\n" +
            "};\n" +
            "\n" +
            "struct states_t {\n" +
            "\tsize_t length, capacity;\n" +
            "\tunsigned *state;\n" +
            "\tunsigned *substate;\n" +
            "};\n" +
            "size_t lex(const lchar *str, size_t str_lenght);\n\n" +
            "// Get string of processed token\n" +
            "selection_t get_token_str();\n\n" +
            "// Convert token to string\n" +
            "char* token_to_string(token_t token);\n\n" +
            "// Retrieves a token from the queue\n" +
            "token_t pop_token();\n\n" +
            $"#endif //{UnitName}_h";
        }

        static void write_source_file(out String output, String UnitName, ref RegularGenerator gen)
        {
            // Шапка файла
            output =
              $"// Auto generated file by LexGen program ver: {PROGRAM_VERSION}\n" +
            "// Please don't edit manually\n" +
            "#include <string.h>\n" +
            "#include <stdlib.h>\n" +
            $"#include \"{UnitName}.h\"\n" +
            "\n" +
            // Вспомогателные функции
            "size_t tokens_length = 0;\n" +
            "size_t tokens_capacity = 0;\n" +
            "selection_t *token_str = NULL;\n" +
            "token_t *tokens = NULL;\n" +
            "\n" +
            "void push_token(token_t token, size_t begin, size_t end) {\n" +
            "\tif (tokens_length == tokens_capacity) {\n" +
            "\t\ttokens_capacity += 32;\n" +
            "\t\ttokens = (token_t*)realloc(tokens, tokens_capacity * sizeof(tokens[0]));\n" +
            "\t\ttoken_str = (selection_t*)realloc(token_str, tokens_capacity * sizeof(token_str[0]));\n" +
            "\t}\n" +
            "\ttokens[tokens_length] = token;\n" +
            "\ttoken_str[tokens_length].begin = begin;\n" +
            "\ttoken_str[tokens_length].end = end;\n" +
            "\ttokens_length++;\n" +
            "}\n" +
            "\n" +
            "token_t pop_token() {\n" +
            "\ttokens_length--;\n" +
            "\treturn tokens[tokens_length];\n" +
            "}\n" +
            "\n" +
            "selection_t get_token_str() {\n" +
            "\treturn token_str[tokens_length];\n" +
            "}\n" +
            "\n" +
            "bool ws(char ch) {\n" +
            "\treturn ch == ' ' || ch == '\\f' ||\n" +
            "\t\tch == '\\t' || ch == '\\v' || ch == '\\n' || ch == '\\r';\n" +
            "}\n" +
            "\n" +
            "bool digit(char ch) {\n" +
            "\treturn '0' <= ch && ch <= '9';\n" +
            "}\n" +
            "\n" +
            "bool word(char ch) {\n" +
            "\treturn 'a' <= ch && ch <= 'z' ||\n" +
            "\t\t'A' <= ch && ch <= 'Z' ||\n" +
            "\t\t'0' <= ch && ch <= '9' ||\n" +
            "\t\tch == '_';\n" +
            "}\n" +
            "\n" +
            "// Save current state\n" +
            "void push_state(states_t *states, unsigned state, unsigned substate) {\n" +
            "\tif (states.length == states.capacity) {\n" +
            "\t\tstates.capacity += 32;\n" +
            "\t\tstates.state = (unsigned*)realloc(\n" +
            "\t\t\tstates.state, sizeof(states.state[0]) * states.capacity);\n" +
            "\t\tstates.substate = (unsigned*)realloc(\n" +
            "\t\t\tstates.substate, sizeof(states.substate[0]) * states.capacity);\n" +
            "\t}\n" +
            "\tstates.state[states.length] = state;\n" +
            "\tstates.substate[states.length] = substate;\n" +
            "\tstates.length++;\n" +
            "}\n" +
            "\n" +
            "// Load state\n" +
            "void pop_state(states_t *states, unsigned *state, unsigned *substate) {\n" +
            "\tif (states.length > 0) {\n" +
            "\t\tstates.length--;\n" +
            "\t\t*state = states.state[states.length];\n" +
            "\t\t*substate = states.substate[states.length] + 1;\n" +
            "\t}\n" +
            "\telse {\n" +
            "\t\t*state = 0;\n" +
            "\t\t*substate = 0;\n" +
            "\t}\n" +
            "}\n\n";

            // запись конечного автомата
            write_state_machine(ref output,ref gen.elements);

            // Функция для преобразования токена в текстовую форму
            output += "\n" +
            "char* token_to_string(token_t token){\n" +
            "\tswitch(token) {";
            foreach (token_description t in token_descs)

                output += $"\tcase {t.name}: return \"{t.name}\";\n";
            output += "\t}\n" +
              "\treturn NULL;\n" +
              "}\n\n";
        }

        static void write_term(ref String output, ref List<node_t> terminals, Int32 id) {
            String go_up =
              "\t\t\tdefault: // recursively go up\n" +
              "\t\t\t\tpop_state( &states, &state, &substate);\n" +
              "\t\t\t\tch = str[--i];\n" +
              "\t\t\t\tif (i < str_lenght) goto send_to_top;\n";
            bool no_error = false;
            int i = 0;
            // если переходов больше одного  
            if (terminals.Count > 1)
            {
                //этапы проверки
                output += "\t\t\tswitch (substate) {\n";
                for (i = 0; i < terminals.Count; i++)
                {
                    if (terminals[i].nt == nodetype.Token ||
                                      terminals[i].nt == nodetype.AnyChar && terminals[i].invert)
                        output += "\t\t\tdefault:\n";
                    else
                        output += $"\t\t\tcase {i}:";
                    no_error |= write_transition_condition(ref output, terminals[i], id, (UInt32)i);
                    if (terminals[i].nt != nodetype.Token && (terminals[i].nt != nodetype.AnyChar || !terminals[i].invert))
                        output += " break; }\n";
                }
                // если посылает ошибку
                if (id == 0)
                {
                    output +=
                      $"\t\t\tcase {i}:if (ws(ch)) {{ break; }}\n" +
                      "\t\t\tdefault:\n" +
                      "\t\t\t\tif (j == i) j++;\n" +
                      "\t\t\t\tpush_token(t_unexpected, i, j);\n" +
                      "\t\t\t\treturn tokens_length;\n";
                }
                else
                if (!no_error)
                    output += go_up;
                output += "\t\t\t}\n";
            }
            // если один переход
            else if (terminals.Count == 1)
            {
                if (terminals[0].nt == nodetype.Token ||
                    terminals[0].nt == nodetype.AnyChar && terminals[0].invert)
                    write_transition_condition(ref output, terminals[0], id, 0);
                else
                {
                    output +=
                      "\t\t\tswitch (substate) {\n" +
                      "\t\t\tcase 0:";
                    if (write_transition_condition(ref output, terminals[0], id, 0))
                        output += " break; }\n";
                    else
                        output +=
                        "break; }\n" +
                        go_up +
                        "\t\t\t}\n";
                }
            }
        }

        // запись конечного автомата
        static void write_state_machine(ref String output, ref List<node_t> nodes)
        {
            List<node_t> terminals = new List<node_t>();
            // начало фунцции
            output += "size_t lex(const lchar *str, size_t str_lenght) {\n" +
            "\tstates_t states;\n" +
            "\tunsigned depth = 0;\n" +
            "\tunsigned state = 0;\n" +
            "\tunsigned substate = 0;\n" +
            "\tsize_t str_begin = 0;\n" +
            "\tsize_t i;\n" +
            "\tchar ch;\n" +
            "\tstates.capacity = 0;\n" +
            "\tstates.length = 0;\n" +
            "\tstates.state = nullptr;\n" +
            "\tstates.substate = nullptr;\n" +
            "\tfor (size_t j = 0; j <= str_lenght; j++) {\n" +
            "\t\tif (j == str_lenght)\n" +
            "\t\t\tif (state == 0)\n" +
            "\t\t\t\tbreak;\n" +
            "\t\t\telse\n" +
            "\t\t\t\tsubstate = -1;\n" +
            "\t\tch = str[j];\n" +
            "\t\ti = j;\n" +
            "\tsend_to_top:\n" +
            "\t\tswitch (state) {\n" +
            "\t\tcase 0: {\n";

            // запись элементов начального состояния
            foreach (node_t item in nodes[0].children)
                if (item.nt != nodetype.None)
                    get_transition_condition(ref terminals, item);
            write_term(ref output, ref terminals, 0);
            output +=
              "\t\tstr_begin = i;\n" +
            "\t\t} break;\n";

            foreach (node_t node in nodes) {
                // не самостоятельный блок описывающий структуру
                // не выводим
                if (node.id == 0)
                    continue;
                // посылает ли этот узел исключения
                // узел "Все символы" и узлы 
                // возвращающие токены не посылают ошибок
                terminals.Clear();
                foreach (node_t item in node.children)
                    get_transition_condition(ref terminals, item);
                for (int i = terminals.Count - 1; i != -1; i--)
                {
                    if (terminals[i].nt == nodetype.AnyChar && terminals[i].invert)
                    {
                        node_t tmp = terminals[i];
                        for (int j = i + 1; j < terminals.Count; j++)
                        {
                            terminals[j - 1] = terminals[j];
                        }
                        if (terminals.Last().nt == nodetype.Token)
                            terminals.Capacity = terminals.Count - 1;
                        else
                            terminals[terminals.Count - 1] = tmp;
                    }
                }
                output += $"\t\tcase {node.id} : {{\n";
                write_term(ref output, ref terminals, node.id);
                output +=
                  "\t\t} break;\n";
            }
            output +=
              "\t\t}\n" +
            "\t\tsubstate = 0;\n" +
            "\t\tif (i < j) {\n" +
            "\t\t\tch = str[++i];\n" +
            "\t\t\tgoto send_to_top;\n" +
            "\t\t}\n" +
            "\t}\n" +
            "\tselection_t sel;\n" +
            "\ttoken_t token;\n" +
            "\tfor (size_t i = 0, j = tokens_length - 1; i < tokens_length / 2; i++, j--) {\n" +
            "\t\ttoken = tokens[i];\n" +
            "\t\ttokens[i] = tokens[j];\n" +
            "\t\ttokens[j] = token;\n" +
            "\t\tsel = token_str[i];\n" +
            "\t\ttoken_str[i] = token_str[j];\n" +
            "\t\ttoken_str[j] = sel;\n" +
            "\t}\n" +
            "\treturn tokens_length;\n" +
            "}\n";
        }

        // записать условия перехода в следующее состояние из текущего
        static void get_transition_condition(ref List<node_t> terminals, node_t node)
        {
            node_t node_repeat = null;
            bool unique = true;

            if (node.nt == nodetype.Block)
                throw new Exception("Block node in final syntax tree");

            // структурный блок - дальше список переходов
            if (node.nt == nodetype.None || node.nt == nodetype.Repeat)
            {
                if (node.nt == nodetype.Repeat)
                {
                    // жадный цикл - запишем условие повтора в начале блока
                    if (node.greedy)
                    {
                        // если максимум 1 то не возвращаемся к началу блока
                        if (node.B == 0 || node.B > 1)
                            foreach (node_t item in node.parent.children)
                            {
                                if (item.nt == nodetype.Token)
                                    break;
                                unique = true;
                                for (int i = 0; i < terminals.Count; i++)
                                {
                                    if (terminals[i] == item)
                                    {
                                        unique = false;
                                        break;
                                    }
                                }
                                if (unique)
                                {
                                    terminals.Add(item);
                                }
                            }
                    }
                    else
                    {
                        // обработать потом
                        node_repeat = node;
                    }
                }

                foreach (node_t item in node.children)
                {

                    if (item.nt == nodetype.Repeat)
                        // есть еще один внешний цикл
                        get_transition_condition(ref terminals, item);
                    else
                    {
                        unique = true;
                        for (int i = 0; i < terminals.Count; i++)
                        {
                            if (terminals[i] == item)
                            {
                                unique = false;
                                break;
                            }
                        }
                        if (unique)
                        {
                            terminals.Add(item);
                        }
                    }
                }

                // Не жадный цикл - условие повтора после всех проверок
                if (node_repeat != null)
                {
                    // если максимум 1 то не возвращаемся к началу блока
                    if (node.B == 0 || node.B > 1)
                    {
                        foreach (node_t item in node_repeat.parent.children)
                        {
                            if (item.nt == nodetype.Token)
                                break;
                            unique = true;
                            for (int i = 0; i < terminals.Count; i++)
                            {
                                if (terminals[i] == item)
                                {
                                    unique = false;
                                    break;
                                }
                            }
                            if (unique)
                            {
                                terminals.Add(item);
                            }
                        }
                    }
                }
            }
            else
            {
                unique = true;
                for (int i = 0; i < terminals.Count; i++)
                {
                    if (terminals[i] == node)
                    {
                        unique = false;
                        break;
                    }
                }
                if (unique)
                {
                    terminals.Add(node);
                }
            }
        }

        // экранирование спец. символов
        static string auto_screen(char c)
        {
            switch (c)
            {
                case '\a': return "'\\a'";
                case '\b': return "'\\b'";
                case '\r': return "'\\r'";
                case '\n': return "'\\n'";
                case '\t': return "'\\t'";
                case '\v': return "'\\v'";
                case '\f': return "'\\f'";
                case '\'': return "'\\''";
                case '\"': return "'\\\"'";
                case '\\': return "'\\\\'";
            }
            return $"\'{c}\'";
        }

        // Записать условие перехода в заданный блок (зависит от типа узла)
        static bool write_transition_condition(
          ref String output,
          node_t node,
          Int32 state,
          UInt32 substate)
        {
            bool no_error = false;
            if (!(node.nt == nodetype.AnyChar && node.invert == true) &&
              node.nt != nodetype.Token)
                output += "if (";
            switch (node.nt)
            {
                // один символ
                case nodetype.Char:
                    output += $" ch == {auto_screen((char)node.A)} ) {{ state = {node.id};";
                    break;
                // произвольный символьный класс
                case nodetype.Charset:
                    {
                        char prev = node.set.First();
                        char start = prev;
                        bool first = true;
                        output += ' ';
                        // максимально компактная запись всех значений символьного класса
                        foreach (char c in node.set) {
                            if (c - prev > 1)
                            {
                                // если не первый то добавляем условие или
                                if (first)
                                    first = false;
                                else
                                    output += " ||\n\t\t\t\t";
                                // если интервал содержит 1 или 2 значения то
                                // записываем как отдельные условия
                                // если больше то как интервал char 1 <= x <= char 2
                                switch (prev - start)
                                {
                                    case 0:
                                        output += $"ch == {auto_screen(prev)}";
                                        break;
                                    case 1:
                                        output += "ch == {start} ||\n\t\t\t\tch == {auto_screen(prev)}";
                                        break;
                                    default:
                                        output += $"{auto_screen(start)} <= ch && ch <= {auto_screen(prev)}";
                                        break;
                                }
                                start = c;
                            }
                            prev = c;
                        }
                        // обработка последнего интервала
                        if (first)
                            first = false;
                        else
                            output += " ||\n\t\t\t";
                        switch (prev - start)
                        {
                            case 0:
                                output += $"ch == {auto_screen(prev)}";
                                break;
                            case 1:
                                output += $"ch == {start} ||\n\t\t\tch == {auto_screen(prev)}";
                                break;
                            default:
                                output += $"{auto_screen(start)} <= ch && ch <= {auto_screen(prev)}";
                                break;
                        }
                        output += $") {{ state = {node.id};";
                    }
                    break;

                // встроенный класс "Все символы" + 
                case nodetype.AnyChar:
                    if (node.invert)
                    {
                        no_error = true;
                        output += $"\t\t\t\tstate = {node.id};\n";
                    }
                    else
                        output += $"ch != '\\n' && ch != '\\r') {{ state = {node.id};";
                    break;

                // Встроенный класс "Пробельные символы" + 
                case nodetype.WhiteSpace:
                    if (node.invert)
                        output += "!ws(ch)) {\n" +
                $"\t\t\tstate = {node.id};";
                    else
                        output += "ws(ch)) {\n" +
                          $"\t\t\tstate = {node.id};";
                    break;

                // встроенный класс "Цифры" + 
                case nodetype.Digit:
                    if (node.invert)
                        output += $"!digit(ch)) {{ state = {node.id};";
                    else
                        output += $"digit(ch)) {{ state = {node.id};";
                    break;

                // встроенный класс "Слова" + 
                case nodetype.Word:
                    if (node.invert)
                        output += $"!word(ch)) {{ state = {node.id};";
                    else
                        output += $"word(ch)) {{ state = {node.id};";
                    break;

                // Вернуть токен
                case nodetype.Token:
                    no_error = true;
                    if (node.A > 0)
                        output +=
                          $"\t\t\t\tpush_token({token_descs[node.A].name}, str_begin, i);\n";
                    output +=
                      "\t\t\t\tstate = 0; substate = 0; str_begin = i;\n" +
                "\t\t\t\tstates.length = 0;\n" +
                "\t\t\t\tif (i < str_lenght) goto send_to_top;\n";
                    break;
                default:
                    break;
            }
            // записываем хитрую конструкцию откатов
            if (!(node.nt == nodetype.AnyChar && node.invert == true) &&
              node.nt != nodetype.Token)
                output += $" push_state(&states, {state}, {substate});";
            return no_error;
        }

        // инициаизация
        static void init(ref RegularGenerator gen)
        {
            token_descs.Clear();
            gen.get_root().A = 1;
            token_description tmp;
            tmp.name = "t_none";
            tmp.expr = "";
            tmp.type = token_t.sysyem;
            token_descs.Add(tmp);
        }

        // распознавание
        static token_t recognize(String str)
        {
            if (str == "token")
                return token_t.token;
            if (str == "oper")
                return token_t._operator;
            if (str == "mute")
                return token_t.mute;
            if (str == "kword")
                return token_t.keyword;
            return token_t.ident;
        }

        // обработка токена
        static token_t state = token_t.unknown;
        static SortedSet<token_t> wait_next = new SortedSet<token_t>();
        static token_description descr;

        static void process_token(token_t t, String data)
        {

            if (t != token_t.eol && t != token_t.eof &&
                !(wait_next.Count == 0) && !wait_next.Contains(t))
            {
                throw new Exception("Unexpected token");
            }

            switch (t)
            {
                case token_t.unknown:
                    break;
                case token_t._operator:
                case token_t.mute:
                case token_t.keyword:
                case token_t.token:
                    state = t;
                    wait_next.Clear();
                    wait_next.Add(token_t.ident);
                    break;
                case token_t.ident:
                    // получен идентификатор
                    switch (state)
                    {
                        case token_t._operator:
                        case token_t.mute:
                        case token_t.keyword:
                        case token_t.token:
                            // Name must be unique
                            foreach (token_description item in token_descs)
                                if (item.name == data)
                                    throw new Exception("Token name must be unique");
                            // do some
                            descr.type = state;
                            descr.name = data;
                            wait_next.Clear();
                            wait_next.Add(token_t.expr);
                            break;
                    }
                    break;
                case token_t.expr:
                    // получено выражение
                    switch (state)
                    {
                        case token_t._operator:
                        case token_t.mute:
                        case token_t.keyword:
                        case token_t.token:
                            {
                                // do some
                                descr.expr = data;

                                token_descs.Add(descr);

                                wait_next.Clear();
                            }
                            break;
                    }
                    break;
                case token_t.eof:
                case token_t.eol:
                    state = token_t.unknown;
                    wait_next.Clear();
                    break;
            }
        }
        /* */
    }

}
