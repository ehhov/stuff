function psc
    set -l output (pgrep $argv)
    [ "$output" ]; and ps -o 'pid=,cmd=' -p $output
end
