#!/usr/bin/python2
import os

class Generator:

	bridge_network = "192.168.1."
	vagrant_box = "terrywang/archlinux"
	num_clients = 3
	max_ram = 7000000
	client_ram = 768

	base_config = """Vagrant.configure(2) do |config|
    servers.each do |machine|
        config.vm.define machine[:hostname] do |node|
            node.vm.box = "{}"
            node.vm.usable_port_range = (2200..2250)
            node.vm.hostname = machine[:hostname]
            node.vm.network "public_network", bridge: "br0"
            node.ssh.password = "vagrant"
            node.vm.provider "virtualbox" do |vb|
                vb.customize ["modifyvm", :id, "--memory", machine[:ram]]
                vb.name = machine[:hostname]
                #if (!machine[:hdd_name].nil?)
                #    unless File.exist?(machine[:hdd_name])
                #        vb.customize ["createhd", "--filename", machine[:hdd_name], "--size", machine[:hdd_size]]
                #    end
                #    vb.customize ["storageattach", :id, "--storagectl", "SATA", "--port", 1, "--device", 0, "--type", "hdd", "--medium", machine[:hdd_name]]
                #end
            end
            node.vm.provision :shell, path: "temp_setup/client-bootstrap.sh"
        end
    end
end"""


	def main(self):
		self.write_header()
		self.write_client_array()
		self.write_line(self.base_config.format(self.vagrant_box))

	def write_header(self):
		header = """# -*- mode: ruby -*-
# vi: set ft=ruby :"""
		self.write_line(header)

	def write_client_array(self):
		array_structure = '[\n'
		for i in range(0, self.num_clients):
			array_structure += '\t{\n'
			array_structure += '\t\t:hostname => \"client' + str(i) + '\",\n'
			array_structure += '\t\t:ip => \"' + str(self.bridge_network) + str(100 + i) + '\",\n'
			array_structure += '\t\t:ram => ' + str(self.client_ram) + '\n'
			array_structure += '\t}'
			if i != self.num_clients - 1:
				array_structure += ','
			array_structure += '\n'
		array_structure += ']\n'
		self.write_variable('servers', array_structure)

	def write_variable(self, name, value):
		self.write_line(name + ' = ' + value)

	def write_line(self, line):
		with open('Vagrantfile', 'ab') as vagrantfile:
			vagrantfile.write(line + '\n')

	def __init__(self):
		with open('Vagrantfile', 'wb') as vagrantfile:
			pass

gen = Generator()
gen.main()