
module pcihellocore (
	pcie_hard_ip_0_pcie_rstn_export,
	pcie_hard_ip_0_powerdown_pll_powerdown,
	pcie_hard_ip_0_powerdown_gxb_powerdown,
	pcie_hard_ip_0_refclk_export,
	pcie_hard_ip_0_rx_in_rx_datain_0,
	pcie_hard_ip_0_tx_out_tx_dataout_0,
	switchs_external_connection_export,
	display_seven_left_external_connection_export,
	display_seven_right_external_connection_export,
	ledgreen_external_connection_export,
	ledred_external_connection_export,
	buttons_external_connection_export);	

	input		pcie_hard_ip_0_pcie_rstn_export;
	input		pcie_hard_ip_0_powerdown_pll_powerdown;
	input		pcie_hard_ip_0_powerdown_gxb_powerdown;
	input		pcie_hard_ip_0_refclk_export;
	input		pcie_hard_ip_0_rx_in_rx_datain_0;
	output		pcie_hard_ip_0_tx_out_tx_dataout_0;
	input	[31:0]	switchs_external_connection_export;
	output	[31:0]	display_seven_left_external_connection_export;
	output	[31:0]	display_seven_right_external_connection_export;
	output	[31:0]	ledgreen_external_connection_export;
	output	[31:0]	ledred_external_connection_export;
	input	[31:0]	buttons_external_connection_export;
endmodule
