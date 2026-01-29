# Def

Offline dictionary application

## Install

    ./setup.sh

## Remove

    ./setup.sh remove

---

## Examples

- Add a word/definition to the database
  
  ```sh
  $ def -a -w "my new word" -p "my part of speech" -d "my definition!"
  ```

- Show the definitions for a word (use `-n` to avoid ansi escape sequences for
  bold text)

  ```sh
  $ def -n "my new word"                                                
  ┌──────────────────────┐
  │     my new word      │
  ├──────────────────────┤
  │ 1. my part of speech │
  │    my definition!    │
  └──────────────────────┘
  ```

- Remove a word/definition from the database

  ```sh
  $ def -r 1 "my new word"
  ```

- Learn a random word!

  ```sh
  $ def "$(def -R)"
  ```

- Edit a definition

  ```sh
  $ def example     
  ┌──────────────────────────────────────────────────────────────────────────────────┐
  │                                     example                                      │
  ├──────────────────────────────────────────────────────────────────────────────────┤
  │ 1. noun                                                                          │
  │    an item of information that is typical of a class or group                    │
  │                                                                                  │
  │ 2. noun                                                                          │
  │    a representative form or pattern                                              │
  │                                                                                  │
  │ 3. noun                                                                          │
  │    something to be imitated                                                      │
  │                                                                                  │
  │ 4. noun                                                                          │
  │    punishment intended as a warning to others                                    │
  │                                                                                  │
  │ 5. noun                                                                          │
  │    an occurrence of something                                                    │
  │                                                                                  │
  │ 6. noun                                                                          │
  │    a task performed or problem solved in order to develop skill or               │
  │    understanding                                                                 │
  └──────────────────────────────────────────────────────────────────────────────────┘

  $ def -e 5 example      
  Old part of speech: noun
  New part of speech (Leave empty to keep): new part of speech
  
  Old definition: an occurrence of something
  New definition (Leave empty to keep): here is a new definition

  $ def example     
  ┌──────────────────────────────────────────────────────────────────────────────────┐
  │                                     example                                      │
  ├──────────────────────────────────────────────────────────────────────────────────┤
  │ 1. noun                                                                          │
  │    an item of information that is typical of a class or group                    │
  │                                                                                  │
  │ 2. noun                                                                          │
  │    a representative form or pattern                                              │
  │                                                                                  │
  │ 3. noun                                                                          │
  │    something to be imitated                                                      │
  │                                                                                  │
  │ 4. noun                                                                          │
  │    punishment intended as a warning to others                                    │
  │                                                                                  │
  │ 5. new part of speech                                                            │
  │    here is a new definition                                                      │
  │                                                                                  │
  │ 6. noun                                                                          │
  │    a task performed or problem solved in order to develop skill or               │
  │    understanding                                                                 │
  └──────────────────────────────────────────────────────────────────────────────────┘
  ```
