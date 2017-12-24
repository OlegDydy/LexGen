#include "SyntaxTree.h"
#include <exception>

const char NOT_IMPLEMENTED_STR[] = "NOT IMLEMENTED YET";
const char INVALID_REGULAR_STR[] = "Invalid regular expression";
const char INDETERMINATE_STR[] = "Indeterminate behavior : Two tokens have the same form.Please check tokens : ";

node_t root;
std::vector<node_t*> elements = { &root };

std::string err_string;

bool ws(char ch) {
  return ch == ' ' || ch == '\f' ||
    ch == '\t' || ch == '\v' || ch == '\n' || ch == '\r';
}

bool digit(char ch) {
  return '0' <= ch && ch <= '9';
}

bool word(char ch) {
  return 'a' <= ch && ch <= 'z' ||
    'A' <= ch && ch <= 'Z' ||
    '0' <= ch && ch <= '9' ||
    ch == '_';
}

// operand "."
bool any(char ch,bool inlude_linebreak) {
  if (inlude_linebreak)
    return true;
  else
    return ch != '\r' && ch != '\n';
}

// вставка нового потомка
node_t* insert_child(node_t* parent, node_t* child) {
  // начальный структурный блок
  if (parent->parent == &root) {
    node_t* node = insert_child(&root, child);
    if (node != child) {
      // Аналогичный узел уже есть
      child = node;
      parent->children.push_back(child);
      return child;
    }
  }
  node_t* cur = parent;
  if (!parent->children.empty()) {
    // Переход на выдачу токена должен быть если идти больше некуда
    if (child->nt == ntToken) {
      // выдавать мы можем только один токен иначе неопределенность
      if (cur->children.back()->nt == ntToken)
        throw indeterminate_e(INDETERMINATE_STR, cur->A);
      cur->children.push_back(child);
    }
    else
    {
      std::vector<node_t*>::iterator item;
      // вставка перед выдачей токена если он есть
      for (item = cur->children.begin(); item != cur->children.end(); item++) {
        if ((*item)->nt == ntToken)
          break;
        if ((*item)->nt == child->nt) {
          // проверим, а не пытаемся ли мы сунуть повторяющийся элемент
          switch (child->nt) {
          case ntChar:
            // если добавляем повторяющийся символ
            if (child->A == (*item)->A)
              return (*item);
            break;
          case ntCharset:
            // если добавляем повторяющийся класс символов
            if (child->set == (*item)->set)
              return (*item);
            break;
          case ntAnyChar:
          case ntWhiteSpace:
          case ntDigit:
          case ntWord:
            // достаточно встретить
            if ((*item)->invert == child->invert)
              return (*item);
          case ntBlock:
            throw std::exception("WTF!");// такого быть не должно по идее
          case ntRepeat:
            if ((*item)->A == child->A && (*item)->B == child->B)
              return (*item);
            break;
          }
        }
      }
      cur->children.insert(item, child);
    }
  }
  else {
    parent->children.push_back(child);
  }

  // всплывааем 
  // добавляем ссылки на блок родителю и переходим к нему
  if (parent->nt == ntRepeat && parent->A == 0)
    insert_child(parent->parent, child);

  if (parent->parent == &root && child->nt == ntToken)
    throw std::exception("Regular expression has empty sequence");
  return child;
}

// добавить выходной узел
void add_block_exit(node_t *node, node_t *block_end) {
  if (node->children.empty()) {
    if (node == block_end)
      // пришли по другой ветке
      return;
    node->children.push_back(block_end);
  }
  else
  for (auto item = node->children.begin(); item != node->children.end(); item++) {
    if ((*item)->nt == ntToken) 
      break;
    add_block_exit((*item),block_end);
  }
}

// добавить структурный блок
node_t* add_child_block(node_t *block_start) {
  node_t *tmp = new node_t;
  tmp->parent = block_start;
  tmp->nt = ntBlock;
  elements.push_back(tmp);

  // для всех выходов из блока добавим ссылку на закрывающий узел
  add_block_exit(block_start, tmp);

  // переходим в конец блока
  return tmp;
}

// добавить узел "один символ"
node_t* add_child_char(node_t* parent, uint32_t ch) {
  if (parent->nt == ntBlock)
    parent->nt = ntNone;// структурный блок
  
  node_t* child = new node_t;
  child->nt = ntChar;
  child->parent = parent;
  child->A = ch;

  node_t* tmp = insert_child(parent, child);
  if (tmp != child) {
     delete child;
     child = tmp;
  }
  else {
    child->id = root.next_id();
    elements.push_back(child);
  }
  return child;
}

// добавить специфичного потомка (пресеты)
node_t* add_child(node_t* parent, nodetype node_type) {
  if (node_type == ntRepeat || node_type == ntChar || node_type == ntBlock)
    throw std::exception("Use specialized function");

  if (parent->nt == ntBlock) 
    parent->nt = ntNone;

  node_t *child = new node_t;
  child->nt = node_type;
  child->parent = parent;

  node_t * tmp = insert_child(parent, child);
  if (tmp != child) {
    // такой блок уже был
    delete child;
    return tmp;
  }
  else {
    // У структурных блоков нет ID
    if (node_type != ntNone && node_type != ntToken)
      child->id = root.next_id();
    elements.push_back(child);
  }

  return child;
}

// добавить потомка типа "цикл"
node_t* add_child_repeat(node_t* parent, int begin, int end, bool greedy) {
  if (parent->nt == ntBlock) {
    // смена типа
    parent->nt = ntRepeat;
    parent->A = begin;
    parent->B = end;
    parent->greedy = greedy;
  }
  else {
    // цикл как и блок не имеет собственного id поскольку является структурным
    node_t* child = new node_t;
    child->nt = ntRepeat;
    child->A = begin;
    child->B = end;
    child->greedy = greedy;
    // начало цикла должно быть выше на одну позицию
    child->parent = parent->parent;
    node_t* tmp = insert_child(parent, child);
    if (tmp != child) {
      // такой цикл уже был
      delete child;
      child = tmp;
    }
    parent = child;
  }
  return parent;
}

const char *num = "0123456789";
const char *wd = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// получить корень дерева
node_t * get_root()
{
  return &root;
}

// получить список всех созданных узлов
const std::vector<node_t*>* get_elements()
{
  return &elements;
}

// перевести из шестнадцатеричной системы в двоичную
uint32_t hex2int(char c) {
  if ('0' <= c && c <= '9')
    return c - '0';
  if ('a' <= c && c <= 'f')
    return 10 + c - 'a';
  if ('A' <= c && c <= 'F')
    return 10 + c - 'A';
  return -1;
}

//1. Cтроим конечный автомат в виде графа со ссылками
#define exclude_range if (range) {range = false; cur->set.insert('-');}
#define special_range(a) {\
if (range) {\
  range = false;\
  stop = a;\
  if (stop < start)\
    throw std::exception(INVALID_REGULAR_STR);\
  for (int i = start; i <= stop; i++)\
    cur->set.insert(i);\
}else\
  cur->set.insert(a);}

// добавть новую ветвь автомата
void add_branch(const std::string &expr, uint32_t token_id) {
  const int max_stack_len = 64;
  if (expr.empty())
    return;

  char start, stop;
  node_t* entrances[max_stack_len];
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

  node_t *cur = add_child(&root,ntNone);
  entrances[top] = cur;

  for (size_t i = 0; i < expr.length(); i++) {
    if (coment) {
      if (expr[i] == ')')
        coment = false;
      continue;
    }
    if (modifiers) {
      switch (expr[i]) {
      case 's': any_symbol = !unset; break;
      case 'i': any_case = !unset; break;
      case '-':
        if (unset)
          throw std::exception(INVALID_REGULAR_STR);
        unset = true;
        break;
      case ':':modifiers = false; break;
      case ')':modifiers = false; top--; break;
      case '#':
        if (expr[i - 1] == '?') {
          modifiers = false;
          coment = true;
          break;
        }
      default:;
        throw std::exception(INVALID_REGULAR_STR);
      }
      continue;
    }
    // экранирование
    if (screen) {
      screen = false;
      // различные спецсимволы полученные из обычных
      switch (expr[i]) {
      case 'x': // ANSI
      case 'u': {// UTF-16
        wchar_t ch = L'\0';
        uint32_t t;
        size_t j;
        size_t len = (expr[i] == 'x') ? 1 : 3;// count - 1

        // Достаточно ли в строке элементов?
        if (i + len + 1 >= expr.length()) {
          // если нет то игнорируем специальное значение
          if (charset)
            special_range(expr[i])
          else {
            cur = add_child_char(cur, expr[i]);
          }
        }
        else {
          // если Достаточно, то преобразуем в символ
          for (j = 0; j <= len; j++) {
            t = hex2int(expr[i + j + 1]);
            if (t < 16)
              // помещаем 16-ричную цифру в символ
              ch |= t << (4 * (len - j)); 
            else {
              // встретили символ не являющийся 16-ричным числом
              // игнорируем специальное значение
              if (charset)
                special_range(expr[i])
              else
                cur = add_child_char(cur, expr[i]);
              break;
            }
          }
          if (j > len) {
            // не игнорируем специальное значение
            cur = add_child_char(cur, ch);
            i += j;
          }
        }
      }break;
      case 'r': { /*возврат каретки*/
        if (charset)
          special_range('\r')
        else {
          cur = add_child_char(cur, '\r');
        }
        break; }
      case 'n': { /*перевод строки*/
        if (charset)
          special_range('\n')
        else {
          cur = add_child_char(cur, '\n');
        }
        break; }
      case 't': { /*табуляция*/
        if (charset)
          special_range('\t')
        else {
          cur = add_child_char(cur, '\t');
        }
        break; }
      case 'v': { /*вертикальная табуляция*/
        if (charset)
          special_range('\v')
        else {
          cur = add_child_char(cur, '\v');
        }
        break; }
      case 'f': { /*разрыв страницы*/
        if (charset)
          special_range('\f')
        else {
          cur = add_child_char(cur, '\v');
        }
        break; }
                // Встроенные классы символов
      case 'd': { /*Цифры*/
        if (charset) {
          exclude_range;
          for (int i = 0; num[i] != '\0'; cur->set.insert(num[i]), i++);
        }
        else
          cur = add_child(cur, ntDigit);
        break; }
      case 'D': { /*Не Цифры*/
        if (charset) {
          exclude_range;
          for (int i = 0; i <= 255; i++)
            if (i < '0' || i > '9')
              cur->set.insert(i);
        }
        else {
          cur = add_child(cur, ntDigit);
          cur->invert = true;
        }
        break; }
      case 's': { /*Пробельные символы*/
        if (charset) {
          exclude_range;
          cur->set.insert({ ' ', '\v', '\t', '\r', '\n', '\f' });
        }
        else
          cur = add_child(cur, ntWhiteSpace);
        break; }
      case 'S': { /*Непробельные символы*/
        if (charset) {
          exclude_range;
          for (int i = 0; i <= 255; i++)
            if (!ws(i))
              cur->set.insert(i);
        }
        else {
          cur = add_child(cur, ntWhiteSpace);
          cur->invert = true;
        }
        break; }
      case 'w': { /*Буквенный или цифровой или подчеркивание*/
        if (charset) {
          exclude_range;
          for (int i = 0; wd[i] != 0; i++)
            cur->set.insert(wd[i]);
          for (int i = 0; num[i] != 0; i++)
            cur->set.insert(num[i]);
          cur->set.insert('_');
        }
        else
          cur = add_child(cur, ntWord);
        break;
      case 'W': /*не Буквенный не цифровой и не подчеркивание*/
        if (charset) {
          exclude_range;
          for (int i = 0; i <= 255; i++)
            if (!word(i))
              cur->set.insert(num[i]);
        }
        else {
          cur = add_child(cur, ntWord);
          cur->invert;
        }
      } break;
      // Все символы не являющиеся управляющими будут записаны как обычно
      default: {
        if (charset)
          special_range(expr[i])
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
    case '[': {
      if (charset) {
        cur->set.insert('[');
        break;
      }
      else {
        cur = add_child(cur, ntCharset);
        first = true;
        charset = true;
        continue;
      }
      // конец символьного класса
    case ']':
      charset = false;
      if (range)
        cur->set.insert('-');
      if (cur->set.empty())
        throw std::exception(INVALID_REGULAR_STR);
      break;
      // начало блока
    case '(':
      // запоминаем точку возврата
      if (charset)
        special_range('(')
      else
      if (top < max_stack_len) {
        entrances[++top] = cur;
      }
      else
        throw std::exception("The maximum depth of nesting is reached (64)");
    } break;
      // Конец блока
    case ')':
    {
      if (charset)
        special_range(')')
      else
        cur = add_child_block(entrances[top--]);
    } break;
    // оператор "или"
    case '|':
    {
      if (charset)
        special_range('|')
      else
        cur = entrances[top];
    } break;
    // интервал
    case '-':
      if (charset)
      {
        if (first)
          special_range('-')
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
        special_range('.')
      else {
        cur = add_child(cur, ntAnyChar);
        if (any_symbol)
          cur->invert = true;
      }
    }break;
      // повторить 0+ раз (n >= 0)
    case '*': {
      if (charset)
        special_range('*')
      else {
        if (cur->nt == ntRepeat)
          // из цикла не может идти еще один цикл
          cur = add_child_char(cur, '*');
        else {
          // позитивный просмотр вперед
          if (i + 1 < expr.length() && expr[i + 1] == '?') {
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
        special_range('+')
      else {
        if (cur->nt == ntRepeat)
          // из цикла не может идти еще один цикл
          cur = add_child_char(cur, '+');
        else
          // позитивный просмотр вперед
          if (i + 1 < expr.length() && expr[i + 1] == '?') {
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
        special_range('?')
      else {
        if (cur == entrances[top]) {
          // начало особой секции
          modifiers = true;
        }else
        if (cur->nt == ntRepeat) {
          // из цикла не может идти еще один цикл
          cur = add_child_char(cur, '?');
        }
        else
          // позитивный просмотр вперед
          if (i + 1 < expr.length() && expr[i + 1] == '?') {
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
        special_range(expr[i])
      else
        cur = add_child_char(cur, expr[i]);
    } break;
    }
    if (charset)
      first = false;
  }

  if (coment || modifiers)
    throw std::exception(INVALID_REGULAR_STR);

  // выходной узел
  cur = add_child(cur, ntToken);
  cur->A = token_id;

}
