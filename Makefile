## DO NOT EDIT!
# This file was provisioned by Terraform
# File origin: https://github.com/Arrow-air/tf-github/tree/main/src/templates/stm32-all/Makefile

include .make/env.mk
export

help: .help-base .help-cspell .help-markdown .help-editorconfig .help-commitlint
build: stm32-build
clean: stm32-clean
test: stm32-test cspell-test md-test-links editorconfig-test
tidy: editorconfig-tidy

include .make/st.mk
include .make/base.mk
include .make/cspell.mk
include .make/markdown.mk
include .make/editorconfig.mk
include .make/commitlint.mk
