## DO NOT EDIT!
# This file was provisioned by Terraform
# File origin: https://github.com/Arrow-air/tf-github/tree/main/src/templates/stm32-all/.make/st.mk
#
BUILD_IMAGE_NAME     ?= ghcr.io/arrow-air/tools/arrow-stm32
BUILD_IMAGE_TAG      ?= 1.0
OUTPUTS_PATH         ?= $(SOURCE_PATH)/out
ADDITIONAL_OPT       ?=

build_run = docker run \
	--name=$(DOCKER_NAME)-$@ \
	--rm \
	--privileged \
	--user `id -u`:`id -g` \
	--workdir=/usr/src/app/ \
	--env-file=$(SOURCE_PATH)/.env \
	-v /dev/bus/usb:/dev/bus/usb \
	$(ADDITIONAL_OPT) \
	-v "$(SOURCE_PATH)/:/usr/src/app/" \
	-t $(BUILD_IMAGE_NAME):$(BUILD_IMAGE_TAG) \
	/bin/sh -c "$(1)"

build-docker-pull:
	@echo docker pull -q $(BUILD_IMAGE_NAME):$(BUILD_IMAGE_TAG)

.help-st:
	@echo ""
	@echo "$(SMUL)$(BOLD)$(GREEN)STM32 Tools$(SGR0)"
	@echo "  $(BOLD)stm32-build$(SGR0)       -- Run 'cmake'"
	@echo "  $(BOLD)stm32-clean$(SGR0)       -- Removes the build/ directory"
	@echo "  $(BOLD)stm32-flash$(SGR0)       -- Run 'st-flash write $(PROJECT_NAME).bin 0x08000000'"
	@echo "  $(BOLD)stm32-probe$(SGR0)       -- Detects connected boards"
	@echo "  $(BOLD)stm32-test$(SGR0)        -- Runs tests"

.SILENT: build-docker-pull

stm32-build: build-docker-pull
	@echo "$(CYAN)Running cmake...$(SGR0)"
	mkdir -p build
	@$(call build_run,cmake -B build/)
	@$(call build_run,cd build && make $(PROJECT_NAME))

stm32-probe: build-docker-pull
	@echo "$(YELLOW)Detecting STM32 boards...$(SGR0)"
	@$(call build_run,cd build && st-info probe)

stm32-flash: build-docker-pull stm32-build
	@echo "$(YELLOW)Flashing...$(SGR0)"
	@$(call build_run,\
	cd build && \
	st-flash write $(PROJECT_NAME).bin 0x08000000 && \
	st-flash reset)

stm32-test:build-docker-pull
	@echo "$(YELLOW)Testing framework not implemented. Skipping.$(SGR0)"

stm32-clean: build-docker-pull
	@echo "$(CYAN)Removing build directory...$(SGR0)"
	@$(call build_run,rm -rf build/)
