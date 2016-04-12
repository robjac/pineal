#!/usr/bin/env hy
(import os)
(import re)

(defreader % [body] `(fn [%] ~body))


(defn read-file [file-name]
  (with [[f (open file-name)]]
    (.read f)))


(defn write-file [file-name body]
  (with [[f (open file-name "w")]]
    (.write f body)))


(defn join-rows [rows]
  (.join "\n" rows))


(defn join-blocks [rows]
  (.join "\n\n" rows))


(defn indent [s]
  (->> s
    (.splitlines)
    (map #%(if % (.format "\t{}" %) ""))
    join-rows))


(defn namespace [name body]
  (.format "namespace {}{{\n{}\n}}"
           name
           (indent body)))


(defn scope [ns name]
  (.format "{}::{}"
           ns name))


(defn boost-pydef [py_func c_func]
  (.format "py::def(\"{}\", &{});"
           py_func c_func))


(setv modules
  (->> (os.path.join "src" "dsl")
    os.listdir
    (filter #%(.endswith % ".h"))
    (map #%(let
             [[name    (->> % os.path.splitext first)]
              [body    (read-file (os.path.join "src" "dsl" %))]
              [defines (->> (re.findall "PINEAL\(\"\w+\"\)[\s\w]+\("
                                        body re.M)
                         (map #%{
                           :py_func (-> % (.split "\"") (get 1))
                           :c_func  (-> (re.split "[\s\(]+" %) (get -2))})

                         list)]]
             {:name name :body body :defines defines}))
    list))

(modules.sort)

(setv bodies
  (join-blocks (map #%(namespace (:name %)
                                 (:body %))
                 modules)))

(setv defines
  (->> modules
    (map (fn [m]
           (->> (:defines m)
             (map #%(boost-pydef (:py_func %)
                                 (scope (:name m) (:c_func %))))
             join-rows
             (indent))))
    join-blocks
    (.format "BOOST_PYTHON_MODULE(core){{
\tpy::class_<pEntity>(\"pEntity\")
\t    .def(py::init<py::object>())
\t;

\tpy::class_<pAction>(\"pAction\")
\t    .def(\"__call__\", &pAction::__call__)
\t;

{}}}")))

(setv code
  (join-blocks [(join-rows
                  ["// DO NOT EDIT THIS FILE"
                   "// generated using scripts/generate.hy and code in src/dsl/"
                   ""
                   "#pragma once"
                   "#define PINEAL(_)"])
                (namespace "dsl"
                           (join-blocks [bodies defines]))]))

(write-file (os.path.join "src" "dsl_wrapper.h")
            code)