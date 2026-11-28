# Debugging the owning list

Investigate a copy of the intentionally faulty starter. From this directory:

```bash
list_work_dir=$(mktemp -d)
cp ../../hints/ex1/*.cpp ../../hints/ex1/*.hpp "$list_work_dir/"
cd "$list_work_dir"
c++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 ex1.cpp list.cpp -o ex1
```

## 1. Resolve the compilation errors

- In `find`, `current->get_data() = value` attempts assignment to a returned
  integer. Use `==` to compare values.
- In `print`, declare a traversal pointer: `Node* current = head`.
- Change `if (current = nullptr)` to `if (current == nullptr)`. Assignment
  discards the traversal pointer instead of checking whether the list is empty.

Recompile, then inspect a search:

```bash
gdb ./ex1
```

```text
(gdb) break 'List::find(int) const'
(gdb) run
(gdb) print value
(gdb) next
(gdb) print current
(gdb) continue
```

At entry, use `next` to execute local-variable initialization before inspecting
`current`. Follow its `next` links and compare the visited values.

## 2. Inspect removal of the head

Restart with a breakpoint on the pointer overload; confirm GDB's prompts:

```text
(gdb) delete breakpoints
(gdb) break 'List::erase(Node*)'
(gdb) run
(gdb) print head->data
(gdb) print node->previous
(gdb) print node->next
(gdb) finish
(gdb) print head->data
```

The first removal targets 5. Its previous pointer is null, so the starter skips
updating a previous node but never advances `head`. Detaching 5 leaves the list
starting at that value and loses access to its successors.

Update either the previous node's `next` or, for head removal,
`head = node->get_next()`. Update the next node's `previous` as well.
The remaining values must be `10, 15`.

## 3. Trace the null-pointer failure

With only the compilation repairs, the next search can no longer reach 10.
It returns null, which the original `erase(Node*)` dereferences:

```text
(gdb) continue
(gdb) backtrace
```

Select the `List::erase(Node*)` frame using `frame N` with the number in your
backtrace, then run `print node`. Addresses and line numbers vary by build.

Return immediately when `node == nullptr`. In `erase(int)`, report missing
values and leave the list unchanged. After repairing the head, add
`list.erase(20)` and `list.erase(nullptr)` to exercise these cases directly.

## 4. Release the erased node

Unlinking does not free an allocation. Update the neighbors and head, clear the
removed node's links, then delete it. Clearing `next` matters:
`Node::~Node` deletes the chain reached through that pointer. Deleting a
still-linked node would destroy successors and leave dangling pointers.

```bash
valgrind --tool=memcheck --leak-check=full --track-origins=yes ./ex1
```

Unreachable chains and detached nodes that were never deleted explain the
leaks. A null check alone fixes neither problem.

## 5. Check the completed solution

Build this solution with `make` and run `./ex1` under Memcheck. Check head,
middle and tail removal, an empty list, duplicates, missing values and null.
The example must progress from `5, 10, 15` to `10, 15`, then `15`, then empty.

`Node()` initializes `data` to zero. Otherwise, appending `new Node()` and
printing it reads an uninitialized value, detectable by Memcheck.

`List` and `Node` disable copying and copy assignment: shallow copies would
create two owners of the same nodes and cause access or deletion after free.
Attempting `List copy = list;` is a separate intentional compilation failure.

`find` lends a pointer, invalidated by erasing its node or destroying the list.
`append(Node*)` takes ownership of a non-null, unlinked heap node;
`erase(Node*)` requires a node of this list or null.
