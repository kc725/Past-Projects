MODULES=objects terminalPrint infection controller demomap state news upgrade app
OBJECTS=$(MODULES:=.cmo) 
MLS=$(MODULES:=.ml)
MLIS=$(MODULES:=.mli)
TEST=test.byte
MAIN=app.byte
OCAMLBUILD=ocamlbuild -use-ocamlfind

default: build
	utop

build:
	$(OCAMLBUILD) $(OBJECTS)

test:
	$(OCAMLBUILD) -tag 'debug' $(TEST) && ./$(TEST)

play:
	$(OCAMLBUILD) $(MAIN) && ./$(MAIN)

check:
	bash checkenv.sh && bash checktypes.sh

finalcheck: check
	bash checkzip.sh
	bash finalcheck.sh

zip:
	zip blight.zip *.ml* _tags Makefile

docs: docs-public docs-private

docs-public: build
	mkdir -p doc.public
	ocamlfind ocamldoc -I _build -package ANSITerminal \
		-html -stars -d doc.public $(MLS)

docs-private: build
	mkdir -p doc.private
	ocamlfind ocamldoc -I _build -package ANSITerminal \
		-html -stars -d doc.private \
		-inv-merge-ml-mli -m A $(MLS)

clean:
	ocamlbuild -clean
	rm -rf doc.public doc.private 
