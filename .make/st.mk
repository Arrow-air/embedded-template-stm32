## DO NOT EDIT!
# This file was provisioned by Terraform
# File origin: https://github.com/Arrow-air/tf-github/tree/main/src/templates/stm32-all/.make/st.mk
#
BUILD_IMAGE_NAME     ?= ghcr.io/arrow-air/tools/arrow-stm32
BUILD_IMAGE_TAG      ?= 1.0
OUTPUTS_PATH        ?= $(SOURCE_PATH)/out
ADDITIONAL_OPT      ?=

build_run = docker run \
	--name=$(BUILD_IMAGE_NAME)-$@ \
	--rm \
	--user `id -u`:`id -g` \
	--workdir=/usr/src/app/ \
	--env-file=$(SOURCE_PATH)/.env \
	$(ADDITIONAL_OPT) \
	-v "$(SOURCE_PATH)/:/usr/src/app/" \
	-t $(BUILD_IMAGE_NAME):$(BUILD_IMAGE_TAG) \
	/bin/bash -c "$(1)"

build-docker-pull:
	@echo docker pull -q $(BUILD_IMAGE_NAME):$(BUILD_IMAGE_TAG)

.help-st:
	@echo ""
	@echo "$(SMUL)$(BOLD)$(GREEN)Build Container$(SGR0)"
	@echo "  $(BOLD)build$(SGR0)       -- Run 'cmake'"

.SILENT: build-docker-pull

stm32-build: build-docker-pull
	@echo "$(CYAN)Running cmake...$(SGR0)"
	mkdir -p build
	@$(call build_run,cmake -B build/)
	@$(call build_run,cd build && make $(PROJECT_NAME))

stm32-flash: build-docker-pull build
	@echo "$(YELLOW)Flashing tool not implemented. Skipping.$(SGR0)"

stm32-test:build-docker-pull
	@echo "$(YELLOW)Testing framework not implemented. Skipping.$(SGR0)"

stm32-clean: build-docker-pull
	@echo "$(CYAN)Cleaning project...$(SGR0)"
	@$(call build_run,cd build && make clean)
