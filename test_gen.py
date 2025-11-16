import random

def generate_test_input(n=10, m=20, value_range=(-20, 20)):
    
    n = random.randint(100, 200)
    # n: number of initial elements
    # m: number of operations
    # value_range: allowed range of integer values (inclusive)
    
    # Step 1: Initial array
    arr = [random.randint(*value_range) for _ in range(n)]
    initial_arr = arr.copy()  # Keep the original for output

    # Step 2: Generate operations
    ops = ['I', 'R', 'S', 'L']
    operations = []

    for _ in range(m):
        op = random.choice(ops)

        # Insert operation
        if op == 'I':
            v = random.randint(*value_range)
            arr.append(v)
            operations.append(f"I {v}")

        # Remove operation — remove only if exists
        elif op == 'R':
            # Randomly choose a value: sometimes from array, sometimes random
            if arr and random.random() < 0.7:
                v = random.choice(arr)
                arr.remove(v)  # remove one copy
            else:
                v = random.randint(*value_range)
                if v in arr:
                    arr.remove(v)
            operations.append(f"R {v}")

        # S operation — find kth smallest
        elif op == 'S':
            # Ensure array not empty
            if not arr:
                # If empty, insert a value first
                v = random.randint(*value_range)
                arr.append(v)
                operations.append(f"I {v}")
            k = random.randint(1, len(arr))
            operations.append(f"S {k}")

        # L operation — lookup index in sorted array
        elif op == 'L':
            v = random.randint(*value_range)
            operations.append(f"L {v}")

    # Step 3: Output format
    print(f"{n} {m}")
    print(" ".join(map(str, initial_arr)))
    for op in operations:
        print(op)


# Example usage
generate_test_input(10, 10)
