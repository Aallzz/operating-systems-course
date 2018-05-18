DIRS = $$(ls -d */)
PACKAGE = hw.zip

run-shell: 
	cd shell/ && $(MAKE) all run

clean: 
	rm -rf $(PACKAGE)
	for dir in $(DIRS) ; do \
	  echo $$dir && cd $$dir && $(MAKE) clean && cd ..; \
	done  

pack:
	for dir in $(DIRS) ; do \
	  echo $$dir && cd $$dir && $(MAKE) pack && cd ..; \
	done 
	zip $(PACKAGE) -r Makefile unpack.sh readme.txt
	for dir in $(DIRS); do \
	  zip -ur $(PACKAGE) $$dir*.zip; \
	done

unpack: 
	rm -rf $(PACKAGE)
	for dir in $(DIRS); do \
	  cd $$dir && unzip *.zip && rm -rf *.zip && cd ..; \
	done
	chmod 755 unpack.sh
	echo "unpacked"
