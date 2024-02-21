# Specify the folder containing your resources
RESOURCE_FOLDER := Resources

# Get a list of all .glsl, .ttf, and .jpg files in the folder
RESOURCE_FILES := $(wildcard $(RESOURCE_FOLDER)/*.glsl) $(wildcard $(RESOURCE_FOLDER)/*.ttf) $(wildcard $(RESOURCE_FOLDER)/*.jpg)

# Generate a list of corresponding .cpp files
RESOURCE_CPP_FILES := $(patsubst $(RESOURCE_FOLDER)/%.glsl, $(RESOURCE_FOLDER)/%.glsl.cpp, $(filter %.glsl, $(RESOURCE_FILES))) \
                     $(patsubst $(RESOURCE_FOLDER)/%.ttf, $(RESOURCE_FOLDER)/%.ttf.cpp, $(filter %.ttf, $(RESOURCE_FILES))) \
                     $(patsubst $(RESOURCE_FOLDER)/%.jpg, $(RESOURCE_FOLDER)/%.jpg.cpp, $(filter %.jpg, $(RESOURCE_FILES)))

# Rule to generate .cpp files from .glsl, .ttf, and .jpg files
$(RESOURCE_FOLDER)/%.glsl.cpp: $(RESOURCE_FOLDER)/%.glsl
	xxd -i $< > $@

$(RESOURCE_FOLDER)/%.ttf.cpp: $(RESOURCE_FOLDER)/%.ttf
	xxd -i $< > $@

$(RESOURCE_FOLDER)/%.jpg.cpp: $(RESOURCE_FOLDER)/%.jpg
	xxd -i $< > $@

# Rule to combine all generated .cpp files into resource.cpp
resource.cpp: $(RESOURCE_CPP_FILES)
	cat $(RESOURCE_CPP_FILES) > $@

# Target to generate all .cpp files from resource files
generate_cpp_files: $(RESOURCE_CPP_FILES)

# Specify the default target
all: generate_cpp_files resource.cpp

.PHONY: all generate_cpp_files
