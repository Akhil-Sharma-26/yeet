#!/bin/bash

echo "Enter your name:"
read name
echo "Enter your email:"
read email

echo "export YEET_AUTHOR_NAME=\"$name\"" >> ~/.bashrc
echo "export YEET_AUTHOR_EMAIL=\"$email\"" >> ~/.bashrc

# only append to ~/.zshrc if it exists
if [ -f "$HOME/.zshrc" ]; then
    echo "export YEET_AUTHOR_NAME=\"$name\"" >> "$HOME/.zshrc"
    echo "export YEET_AUTHOR_EMAIL=\"$email\"" >> "$HOME/.zshrc"
fi

echo "Environment variables added to ~/.bashrc ans ~/.zshrc"
echo "Run 'source ~/.bashrc' or according to your terminal do changes to script, it only supports bash and zsh"
