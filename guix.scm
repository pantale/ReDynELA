(use-modules (guix download)
             (guix packages)
             (guix gexp)
             (guix build-system gnu)
             (guix build-system cmake)
             ((guix licenses) #:prefix license:)
             (gnu packages)
             (gnu packages bison)
             (gnu packages m4))

;; Taken from an old Guix version
(define flex-2.5
  (package
    (name "flex")
    (version "2.5.37")
    (source (origin
              (method url-fetch)
              (uri (string-append "mirror://sourceforge/flex/flex-"
                                  version ".tar.bz2"))
              (sha256
               (base32
                "0ah5mi4j62b85a9rllv1004mzjb5cd0mn4glvz13p88rpx77pahp"))))
    (build-system gnu-build-system)
    ;; Running the tests requires a patch, so for simplicity the
    ;; tests are disabled.
    (arguments
     '(#:tests? #f))
    (inputs `(("bison" ,bison)))
    (propagated-inputs `(("m4" ,m4)))
    (home-page "http://flex.sourceforge.net/")
    (synopsis "A fast lexical analyser generator")
    (description
     "Flex is a tool for generating scanners. A scanner, sometimes
called a tokenizer, is a program which recognizes lexical patterns in
text. The flex program reads user-specified input files, or its standard
input if no file names are given, for a description of a scanner to
generate. The description is in the form of pairs of regular expressions
and C code, called rules. Flex generates a C source file named,
\"lex.yy.c\", which defines the function yylex(). The file \"lex.yy.c\"
can be compiled and linked to produce an executable. When the executable
is run, it analyzes its input for occurrences of text matching the
regular expressions for each rule. Whenever it finds a match, it
executes the corresponding C code.")
    (license "BSD")))

(package
  (name "ReDynELA")
  (version "1.0")
  (source (local-file (string-append (dirname (current-filename)) "/Sources")
                      #:recursive? #t))
  (build-system cmake-build-system)
  (arguments
   `(#:tests? #f
     #:phases
     (modify-phases %standard-phases
       (replace 'install
         (lambda* (#:key inputs outputs #:allow-other-keys)
           (let* ((out (assoc-ref outputs "out"))
                  (bin   (string-append out "/bin")))
             (mkdir-p bin)
             (install-file "bin/DynELA_solve" bin)))))))
  (native-inputs
   `(("flex" ,flex-2.5)
     ("bison" ,bison)))
  (synopsis "DynELA parallel v1.0 for ReScience paper")
  (description "Source code of the solver of DynELA v.1.0 parallel
version published in 2005 for the following paper:

Olivier Pantalé, Parallelization of an object-oriented FEM dynamics code: influence of the strategies on the Speedup. (2005) Advances in Engineering Software, 36 (6). 361-373.")
  (home-page "https://github.com/pantale/ReDynELA")
  (license license:bsd-3))
