(defmacro --header-- []
  "Things to do outside the --draw-- function"
  '(do
     (import [core [*]])

     {% for name in py_modules %}
     (import [{{ name }} [*]])
     {% endfor %}

     (import [math [*]])

     (def 2pi (* 2 pi))
     (value time (fn [] (get-osc-f "/time" 0.0)))
     (value amp (fn [] (get-osc-f "/amp" 0.0)))
     ))

; To use this file as imported module
(--header--)


(defmacro value [name x]
  "Define a function that return the value and optionally takes a scale and a
  offset factor"
  `(defn ~name [&rest args]
     (setv [scale offset] [1 0])

     (when (> (len args) 0)
       (setv scale (get args 0)))

     (when (> (len args) 1)
       (setv offset (get args 1)))

     (-> (~x) (* scale) (+ offset))))


(defn change [entity &rest actions] (change_c entity (list actions)))

(defmacro variadic [name name_c]
  `(defn ~name [&rest args] (~name_c (list args))))

(variadic group     group_c)
(variadic branch    branch_c)
(variadic rgb       rgb_c)
(variadic scale     scale_c)
(variadic translate translate_c)


(defmacro group-for [item-iterator &rest entities]
  (setv [item iterator] item-iterator)
  `(group_c (list (map (fn [~item]
                         (group_c [~@entities]))
                    ~iterator))))


(defmacro branch-for [item-iterator &rest actions]
  (setv [item iterator] item-iterator)
  `(branch_c (list (map (fn [~item]
                          (compose_c [~@actions]))
                     ~iterator))))


(defmacro get-osc-f [path default]
  `(get-osc-f_c (str ~path) ~default))


(defmacro text [font s]
  `(text_c (str ~font) (str ~s)))


;(defmacro beat [n entity pos dur]
;  `(~entity (at-beat [~n ~pos ~dur])))


;(defmacro seq [n &rest args]
;  `(do
;     (setv clips [])

;     (for [[pos e dur] [~@args]]
;       (setv a (at-beat [~n pos dur]))
;       (.append clips (change e a)))

;     (group_c clips)))

