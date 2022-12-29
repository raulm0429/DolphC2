package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"

	"github.com/gorilla/mux"
)

func Tasks(resp http.ResponseWriter, _ *http.Request) {
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

func Post(rw http.ResponseWriter, r *http.Request) {
	reqBody, _ := ioutil.ReadAll(r.Body)
	fmt.Println(string(reqBody))
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/tasks", Tasks)
	r.HandleFunc("/post", Post).Methods("POST")

	log.Println("Listening ...")
	http.ListenAndServe("10.110.6.88:8080", r)
}
