# enable color support of ls
alias ls='ls --color=auto'

# enable git autocompletion
if [ -f /usr/share/bash-completion/completions/git ]; then
    source /usr/share/bash-completion/completions/git
fi

# install pre-commit hooks
pre-commit install
