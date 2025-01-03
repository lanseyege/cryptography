package main

import (
	"fmt"
	"github.com/Nik-U/pbc"
)

func main() {
	fmt.Println("vim-go")
	params := pbc.GenerateA(160, 512)
	pairing := params.NewPairing()
	g := pairing.NewG1()
	g.Rand()
	fmt.Printf("g = %s\n", g)
	fmt.Println(g)
}
