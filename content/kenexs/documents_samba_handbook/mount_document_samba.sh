#!/bin/bash
read -p "Username: " username
read -s -p "Password: " password
echo 

mount_dir=/media/samba_documents

sudo mkdir -p "$mount_dir" 2>/dev/null
sudo mount -t cifs -o "username=$username,password=$password,uid=$UID" //192.168.3.113/Documents "$mount_dir"

mount at "$mount_dir"
