package main

import (
	"bufio"
	"fmt"
	"math"
	"math/bits"
	"os"
	"strconv"
)

var (
	in  = bufio.NewReader(os.Stdin)
	out = bufio.NewWriter(os.Stdout)
)

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func divmod(x, y int) (int, int) {
	return x / y, x % y
}

func __gcd(a, b int) int {
	for b > 0 {
		a, b = b, a%b
	}
	return a
}

func gcd(nums ...int) int {
	r := 0
	for _, num := range nums {
		r = __gcd(r, num)
	}
	return r
}

func __lcm(a, b int) int {
	return a * b / __gcd(a, b)
}

func lcm(nums ...int) int {
	r := 1
	for _, num := range nums {
		r = __lcm(r, num)
	}
	return r
}

func acc(iter []int) []int {
	n := len(iter)
	r := make([]int, n+1)
	for i, num := range iter {
		r[i+1] = r[i] + num
	}
	return r
}

func sum(iter []int) (r int) {
	for _, num := range iter {
		r += num
	}
	return r
}

func pow(x, n, mod int) int {
	res := 1
	for n > 0 {
		if n&1 == 1 {
			res = res * x % mod
		}
		n >>= 1
		x = x * x % mod
	}
	return res
}

func isqrt(x int) int {
	rt := int(math.Sqrt(float64(x)))
	if rt*rt > x {
		rt--
	}
	return rt
}

func rs() string {
	var s string
	fmt.Fscan(in, &s)
	return s
}

func ri() int {
	var n int
	fmt.Fscan(in, &n)
	return n
}

func ri2() (int, int) {
	var n1, n2 int
	fmt.Fscan(in, &n1, &n2)
	return n1, n2
}

func ri3() (int, int, int) {
	var n1, n2, n3 int
	fmt.Fscan(in, &n1, &n2, &n3)
	return n1, n2, n3
}

func ra[T any](n int) []T {
	a := make([]T, n)
	for i := range n {
		fmt.Fscan(in, &a[i])
	}
	return a
}

func rai(n int) []int {
	return ra[int](n)
}

func toint(s string) int {
	r, _ := strconv.Atoi(s)
	return r
}

func tostr(n int) string {
	return strconv.Itoa(n)
}

func tolist(s string, cst byte) []int {
	r := make([]int, len(s))
	for i := range s {
		r[i] = int(s[i] - cst)
	}
	return r
}

func fill[T any](a []T, e T) {
	for i := range a {
		a[i] = e
	}
}

func wln[T any](e ...T) {
	n := len(e)
	for i := range n - 1 {
		fmt.Fprint(out, e[i], " ")
	}
	fmt.Fprintln(out, e[n-1])
}

func ws[T any](e T) {
	fmt.Fprint(out, e, " ")
}

func nln() {
	fmt.Fprintln(out)
}

var (
	d2 = [2][2]int{{1, 0}, {0, 1}}
	d4 = [4][2]int{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
	d8 = [8][2]int{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}}
)

const (
	YES  = "YES"
	NO   = "NO"
	MOD9 = 998244353
	MOD1 = 1000000007
)

func main() {
	defer out.Flush()
	n := ri()
	p := rai(n)
	for i := range n {
		p[i]--
	}
	g := make([][]int, n)
	for range n - 1 {
		u, v := ri2()
		u--
		v--
		g[u] = append(g[u], v)
		g[v] = append(g[v], u)
	}
	pa := make([]int, n)
	lca := make([][]int, n)
	for i := range n {
		lca[i] = make([]int, n)
	}
	var precomp func(int, int)
	precomp = func(u int, p int) {
		pa[u] = p
		for _, v := range g[u] {
			if v == p {
				continue
			}
			precomp(v, u)
		}
	}
	wln(pa...)
	precomp(0, -1)
	for u := range n {
		for v := u + 1; v < n; v++ {
			s := make(map[int]bool)
			t := u
			for t >= 0 {
				s[t] = true
				t = pa[t]
			}
			t = v
			for t >= 0 {
				if s[t] {
					lca[u][v] = t
					lca[v][u] = t
					break
				}
				t = pa[t]
			}
		}
	}
	for i := range n {
		wln(lca[i]...)
	}
	memo := make([][]int, 1<<n)
	for i := range 1 << n {
		memo[i] = make([]int, n)
		fill(memo[i], -1)
	}
	var dfs func(int, int) int
	dfs = func(mask, prev int) int {
		if mask+1 == 1<<n {
			return 0
		}
		if memo[mask][prev] != -1 {
			return memo[mask][prev]
		}
		ans := math.MinInt / 4
		for u := range n {
			if mask>>u&1 == 1 || lca[prev][u] != pa[u] {
				continue
			}
			f := 0
			if u == p[bits.OnesCount(uint(mask))] {
				f++
			}
			ans = max(ans, dfs(mask|(1<<u), u)+f)
		}
		memo[mask][prev] = ans
		return ans
	}
	res := []int{0}
	mask := 1
	prev := 0
	for range n - 1 {
		mx := dfs(mask, prev)
		for u := range n {
			if mask>>u&1 == 1 || lca[prev][u] != pa[u] {
				continue
			}
			f := 0
			if u == p[bits.OnesCount(uint(mask))] {
				f++
			}
			if dfs(mask|(1<<u), u)+f == mx {
				res = append(res, u)
				mask |= 1 << u
				prev = u
				break
			}
		}
	}
	for i := range res {
		res[i]++
	}
	wln(res...)
}
