function defnamen
    set -l cmd (which defnamen | string collect)
    set -l output ("$cmd" $argv | string collect)
    if [ "$output" ]
        set -x namen "$output"
    end
end
