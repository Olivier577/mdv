import numpy as np


def i2xyz_(index: int, shape: tuple, coords: np.ndarray, n: int, product: int) -> np.ndarray:
    print(f'{n=}')
    #product = np.prod(shape[n:])
    if index - product + 1 <= 0:
        if n == len(shape) - 1:
            coords[n] = index
        else:
            # product = np.prod(shape[n + 1 :])
            product //= shape[n]
            coords[n] = index // product
            rest = index % product
            print("increment n and rerun")
            i2xyz_(rest, shape, coords, n + 1, product)
    else:
        print("decrement n and rerun")
        product *= shape[n]
        i2xyz_(index, shape, coords, n - 1, product)
    return coords


def i2xyz2(index: int, shape: tuple) -> np.ndarray:
    n = len(shape)
    coords = np.zeros(n, dtype=np.int64)
    n -= 1
    product = shape[n]
    incr = 1
    while incr != 0:
        if index - (product - 1) <= 0:
            if n == len(shape) - 1:
                coords[n] = index
                incr = 0
                print("exit while loop")
            else:
                product //= shape[n]
                coords[n] = index // product
                rest = index % product
                if rest == 0:
                    break
                index = rest
                incr = 1
                print("increment n and rerun")
        else:
            product *= shape[n-1]
            incr = -1
            print("decrement n and rerun")
        n += incr
    return coords




def i2xyz(index: int, shape: tuple):
    assert index < np.product(shape), "index is too large"
    n = len(shape)
    coords = np.zeros(n, dtype=np.int64)
    return i2xyz_(index=index, shape=shape, coords=coords, n=n-1, product=shape[n-1])


def traverse_right(shape: np.ndarray, coords: np.ndarray, dims: int):
    print(f'{coords=}')
    for i in range(dims - 1, -1, -1):
        if coords[i] < shape[i] -1:
            coords[i] += 1
            coords[i+1:] = 0
            traverse_right(shape, coords, dims)


def traverse_right2(shape: np.ndarray, coords: np.ndarray, dims: int, flag: bool = True):
    while flag:
        print(f'{coords=}')
        for i in range(dims - 1, -1, -1):
            if coords[i] < shape[i] - 1:
                coords[i] += 1
                coords[i+1:] = 0
                flag = True
                break
            flag = False





def traverse_left(shape: np.ndarray, coords: np.ndarray, dims: int):
    print(f'{coords=}')
    for i in range(0, dims):
        if coords[i] < shape[i] - 1:
            coords[i] += 1
            coords[:i] = 0
            traverse_left(shape, coords, dims)

dims = 3
shape = np.arange(dims) + 2
prod = np.product(shape)
coords = np.zeros(dims)
print(f'{traverse_right(shape, coords, dims)=}')
print(f'{shape=}')
print(f'{prod=}')

coords = np.zeros(dims)
print(f'{traverse_left(shape, coords, dims)=}')
print(f'{shape=}')
print(f'{prod=}')

coords = np.zeros(dims)
print(f'{traverse_right2(shape, coords, dims)=}')
print(f'{shape=}')
print(f'{prod=}')




shape=(2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2)
shape = (3, 4)
print(shape)
res = i2xyz2(index=5, shape=shape)

print(f"{res=}")
