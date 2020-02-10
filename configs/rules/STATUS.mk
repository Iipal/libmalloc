.PHONY: STATUS_START
STATUS_START:
 ifneq (,$(OBJS))
	@$(ECHO) " | ---------------------- |"
	@$(ECHO) " | making: $(CLR_UNDERLINE)$(NAME)$(CLR_WHITE) ..."
	@$(ECHO) " | ---------------------- |"
 endif

.PHONY: STATUS_END
STATUS_END:
	@$(ECHO) "/ ----------------------- |"
 ifneq (,$(NAME))
  ifneq (,$(OBJS))
	@$(ECHO) "| compiled                : $(NAME) $(MSG_SUCCESS)"
  endif
 endif
 ifneq (,$(CC))
	@$(ECHO) "| compiler                : $(CC)[$(words $(SRCS)).c]"
 endif
 ifneq (,$(DEFINES))
	@$(ECHO) "| compiler custom defines : $(foreach dfns,$(DEFINES),$(CLR_INVERT)$(dfns)$(CLR_WHITE) )"
 endif
 ifneq (,$(CFLAGS))
	@$(ECHO) "| compiler          flags : $(CFLAGS)"
 endif
 ifneq (,$(CFLAGS_OPTIONAL))
	@$(ECHO) "| compiler optional flags : $(CLR_UNDERLINE)$(CFLAGS_OPTIONAL)$(CLR_WHITE)"
 endif
 ifneq (,$(ARFLAGS))
  ifneq (,$(OBJS))
   ifeq (.a,$(suffix $(NAME)))
	@$(ECHO) "| archiver                : $(CLR_UNDERLINE)$(AR)$(CLR_WHITE)"
	@$(ECHO) "| archiver          flags : $(CLR_UNDERLINE)$(ARFLAGS)$(CLR_WHITE)"
   else
	@$(ECHO) "| compiler shared   flags : $(CLR_UNDERLINE)$(CFLAGS_SHARED)$(CLR_WHITE)"
   endif
  endif
 endif
	@$(ECHO) "\\ ----------------------- |"
