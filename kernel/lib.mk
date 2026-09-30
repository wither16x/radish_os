# Various utilities for other Makefiles

ESCAPE := $(shell printf '\033')
GREEN := $(ESCAPE)[1;32m
CYAN := $(ESCAPE)[1;36m
RED := $(ESCAPE)[1;31m
BRIGHT_BLACK := $(ESCAPE)[90m
RESET := $(ESCAPE)[0m

define log_info
@printf '%b' '$(BRIGHT_BLACK)[-] $(1)$(RESET)'
endef

define log_error
@printf '%b' '$(RED)ERROR: $(1)$(RESET)\n'
endef

define log_success
@printf '%b' '$(GREEN)SUCCESS: $(1)$(RESET)\n'
endef

define log_note
@printf '%b' '$(CYAN)$(1)$(RESET)\n'
endef

OK := @printf '%b' '$(GREEN) OK$(RESET)\n'
ERR := @printf '%b' '$(RED) ERR$(RESET)\n'