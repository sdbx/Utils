TARGET = hhss

all: $(TARGET)

$(TARGET): hhss.rs Cargo.toml
	cargo build --release
	cp target/release/$(TARGET) ./$(TARGET)

clean:
	cargo clean
	rm -f $(TARGET)

.PHONY: all clean