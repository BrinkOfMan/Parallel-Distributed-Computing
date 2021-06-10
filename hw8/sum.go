package main

import "fmt"

func plus(slice []float64) float64 {

	var sum float64 = 0

	for i := 0; i < 10; i++ {
		sum += slice[i]
	}

	return sum
}

func main() {

	var ret float64

	aSlice := []float64{1, 2, 3, 4, 5, 6, 7, 8, 9, 10} // a slice of type float64
	ret = plus(aSlice)

	fmt.Println(ret)
}
