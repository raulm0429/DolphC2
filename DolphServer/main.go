package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"strings"

	"github.com/gorilla/handlers"
	"github.com/gorilla/mux"
)

type RegisteredInfo struct {
	RemoteAddress string
	DesktopName   string
}

type Task struct {
	Cmd string
}

func Tasks(resp http.ResponseWriter, r *http.Request) {
	//remoteAddr := r.RemoteAddr
	content, err := ioutil.ReadFile("Tasks/tasks.json")
	if err != nil {
		log.Fatal(err)
	}

	resp.Write(content)
	return
}

func reg(rw http.ResponseWriter, r *http.Request) {
	reqBody, _ := ioutil.ReadAll(r.Body)
	fmt.Println(string(reqBody))

	ip := strings.Split(r.RemoteAddr, ":")

	registeredInfo := RegisteredInfo{}
	json.Unmarshal([]byte(reqBody), &registeredInfo)
	registeredInfo.RemoteAddress = ip[0]
	content, err := json.Marshal(registeredInfo)
	if err != nil {
		fmt.Println(err)
	}
	err = ioutil.WriteFile("Data/userfile.json", content, 0644)
	if err != nil {
		log.Fatal(err)
	}
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/tasks", Tasks)
	r.HandleFunc("/reg", reg).Methods("POST")

	corsObj := handlers.AllowedOrigins([]string{"*"})

	log.Println("Listening ...")
	http.ListenAndServe("10.110.6.88:80", handlers.CORS(corsObj)(r))
}

