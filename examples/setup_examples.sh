#!/bin/bash

# Create the example directory structure
mkdir -p examples/data

# Create the sample orders file
cat > examples/data/sample_orders.txt << 'EOF'
35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|
35=D|49=SENDER|56=TARGET|11=ORDER124|55=GOOGL|54=1|44=2750.00|38=50|40=2|
35=D|49=SENDER|56=TARGET|11=ORDER125|55=MSFT|54=1|44=330.25|38=75|40=2|
35=D|49=SENDER|56=TARGET|11=ORDER126|55=AMZN|54=1|44=3400.00|38=25|40=2|
35=D|49=SENDER|56=TARGET|11=ORDER127|55=META|54=2|44=320.75|38=150|40=2|
EOF

# Make the script executable
chmod +x examples/fix_client.cpp

echo "Example environment set up successfully!"
