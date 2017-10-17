bin_dir=/opt/devkitpro/devkitARM/bin/
cc=arm-none-eabi-gcc
objcpy=arm-none-eabi-objcopy
ccparams=-mthumb-interwork -mthumb -O2
objcpyparams=-mthumb-interwork -mthumb -specs=gba.specs
gbafix=gbafix
objects=main.o video/screen.o

%.o: %.c
	$(bin_dir)$(cc) $(ccparams) -c -o $@ $<

alpha.elf: $(objects)
	$(bin_dir)$(cc) $< $(objcpyparams) -o $@

alpha.gba: alpha.elf
	$(bin_dir)$(objcpy) -v -O binary $< $@

run: alpha.gba
	vbam $<

.PHONY: clean
clean:
	rm alpha.gba alpha.elf $(objects) trace.log
