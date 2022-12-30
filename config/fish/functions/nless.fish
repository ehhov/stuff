function nless
    if [ -z "$argv" ]
        set -f argv -
    end
    nvim -c 'runtime ftplugin/nless.vim' $argv
end
