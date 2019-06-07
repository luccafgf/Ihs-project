	component pcihellocore is
		port (
			pcie_hard_ip_0_pcie_rstn_export                : in  std_logic                     := 'X';             -- export
			pcie_hard_ip_0_powerdown_pll_powerdown         : in  std_logic                     := 'X';             -- pll_powerdown
			pcie_hard_ip_0_powerdown_gxb_powerdown         : in  std_logic                     := 'X';             -- gxb_powerdown
			pcie_hard_ip_0_refclk_export                   : in  std_logic                     := 'X';             -- export
			pcie_hard_ip_0_rx_in_rx_datain_0               : in  std_logic                     := 'X';             -- rx_datain_0
			pcie_hard_ip_0_tx_out_tx_dataout_0             : out std_logic;                                        -- tx_dataout_0
			switchs_external_connection_export             : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			display_seven_left_external_connection_export  : out std_logic_vector(31 downto 0);                    -- export
			display_seven_right_external_connection_export : out std_logic_vector(31 downto 0);                    -- export
			ledgreen_external_connection_export            : out std_logic_vector(31 downto 0);                    -- export
			ledred_external_connection_export              : out std_logic_vector(31 downto 0);                    -- export
			buttons_external_connection_export             : in  std_logic_vector(31 downto 0) := (others => 'X')  -- export
		);
	end component pcihellocore;

	u0 : component pcihellocore
		port map (
			pcie_hard_ip_0_pcie_rstn_export                => CONNECTED_TO_pcie_hard_ip_0_pcie_rstn_export,                --                pcie_hard_ip_0_pcie_rstn.export
			pcie_hard_ip_0_powerdown_pll_powerdown         => CONNECTED_TO_pcie_hard_ip_0_powerdown_pll_powerdown,         --                pcie_hard_ip_0_powerdown.pll_powerdown
			pcie_hard_ip_0_powerdown_gxb_powerdown         => CONNECTED_TO_pcie_hard_ip_0_powerdown_gxb_powerdown,         --                                        .gxb_powerdown
			pcie_hard_ip_0_refclk_export                   => CONNECTED_TO_pcie_hard_ip_0_refclk_export,                   --                   pcie_hard_ip_0_refclk.export
			pcie_hard_ip_0_rx_in_rx_datain_0               => CONNECTED_TO_pcie_hard_ip_0_rx_in_rx_datain_0,               --                    pcie_hard_ip_0_rx_in.rx_datain_0
			pcie_hard_ip_0_tx_out_tx_dataout_0             => CONNECTED_TO_pcie_hard_ip_0_tx_out_tx_dataout_0,             --                   pcie_hard_ip_0_tx_out.tx_dataout_0
			switchs_external_connection_export             => CONNECTED_TO_switchs_external_connection_export,             --             switchs_external_connection.export
			display_seven_left_external_connection_export  => CONNECTED_TO_display_seven_left_external_connection_export,  --  display_seven_left_external_connection.export
			display_seven_right_external_connection_export => CONNECTED_TO_display_seven_right_external_connection_export, -- display_seven_right_external_connection.export
			ledgreen_external_connection_export            => CONNECTED_TO_ledgreen_external_connection_export,            --            ledgreen_external_connection.export
			ledred_external_connection_export              => CONNECTED_TO_ledred_external_connection_export,              --              ledred_external_connection.export
			buttons_external_connection_export             => CONNECTED_TO_buttons_external_connection_export              --             buttons_external_connection.export
		);

