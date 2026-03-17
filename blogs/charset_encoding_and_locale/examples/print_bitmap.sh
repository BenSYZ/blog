#!/bin/bash
convert_01=true
print_hex(){
    local bc_cmd="ibase=16;obase=2;"
    local hex_num _hex_num
    for hex_num in "$@";do
        _hex_num="${hex_num#0x}"; _hex_num="${_hex_num^^}"
        bc_cmd+="$_hex_num;"
    done

    local -a bin_num_arr=()
    while read -r bin_num; do
        bin_num_arr+=("$bin_num")
    done < <(echo "$bc_cmd" | bc)

    local w
    for (( w=0 ; w < ${#bin_num_arr[@]} ; ++w ));do
        printf "%0*d" 8 "${bin_num_arr[w]}"
    done | {
        if "$convert_01";then
            sed 's/0/ /g;s/1/█/g'
        else
            cat
        fi
    }
    printf "\n"
}
main(){
    local width height
    local str_B1 str_B2

    # https://github.com/zhmars/cjktty-patches/
    width=1
    bitmap_str="0x00,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,"

    width=1
    bitmap_str="0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x66,0x66,0x66,0x66,0xfc,0x00,0x00,0x00,0x00,"

    width=2
    str_B1="0x08,0x08,0x08,0x11,0x11,0x32,0x34,0x50,0x91,0x11,0x12,0x12,0x14,0x10,0x10,0x10,"
    str_B2="0x80,0x80,0x80,0xfe,0x02,0x04,0x20,0x20,0x28,0x24,0x24,0x22,0x22,0x20,0xa0,0x40,"
    bitmap_str="$str_B1$str_B2"

    width=1
    bitmap_str="0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x66,0x66,0x66,0x66,0xfc,0x00,0x00,0x00,0x00,"

    # https://github.com/torvalds/linux/blob/master/lib/fonts/font_6x8.c#L668
    width=1
    bitmap_str="0x78,0x24,0x24,0x38,0x24,0x24,0x78,0x00,"

    local -a bitmap_arr
    while read -r line; do
        bitmap_arr+=("$line");
    done < <(echo -n "$bitmap_str" | sed 's/,/\n/g')

    height="$((${#bitmap_arr[@]}/width))"
    #echo "height=$height"

    local i
    for (( i = 0; i < height ; ++i ));do
        local -a args=()
        local j
        for (( j = 0; j < width; ++j ));do
            args+=("${bitmap_arr[$((i+j*height))]}")
        done
        #echo "${args[*]}"
        print_hex "${args[@]}"
    done


    return 0
}
main "$@"

