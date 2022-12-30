package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"

	"github.com/gorilla/handlers"
	"github.com/gorilla/mux"
)

func Tasks(resp http.ResponseWriter, r *http.Request) {
	//remoteAddr := r.RemoteAddr
	resp.WriteHeader(http.StatusCreated)
	resp.Header().Set("Content-Type", "application/json")
	response := make(map[string]string)
	response["cmd"] = "calc"
	jsonResp, err := json.Marshal(response)
	if err != nil {
		log.Fatalf("Error happened in JSON marshal. Err: %s", err)
	}
	resp.Write(jsonResp)
	return
}

func reg(rw http.ResponseWriter, r *http.Request) {
	reqBody, _ := ioutil.ReadAll(r.Body)
	fmt.Println(string(reqBody))
	fmt.Println(r.RemoteAddr)
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/tasks", Tasks)
	r.HandleFunc("/reg", reg).Methods("POST")

	log.Println("Listening ...")
	http.ListenAndServe("192.168.56.1:8080", handlers.CORS(handlers.AllowedOrigins([]string{"*"}))(r))
}
