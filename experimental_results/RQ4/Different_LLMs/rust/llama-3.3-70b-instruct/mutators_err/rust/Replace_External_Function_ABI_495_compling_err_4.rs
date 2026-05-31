impl Mutator for Replace_External_Function_ABI_495 {
    fn name(&self) -> &str {
        "Replace_External_Function_ABI_495"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(abi) = &func.sig.abi {
                    let new_abi = match abi.name {
                        Some(ref name) => match name.value().as_str() {
                            "C" => "rust-call",
                            "rust-call" => "C",
                            _ => &name.value(),
                        },
                        None => "C",
                    };
                    func.sig.abi = Some(Abi {
                        name: Some(LitStr::new(new_abi, abi.name.as_ref().unwrap().span())),
                        ..abi.clone()
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the ABI of a function with an external ABI, for example, replace 'extern rust-call' with 'extern c' or vice versa. This transformation aims to test the compiler's handling of different external ABIs, which may trigger bugs like the one reported in the issue."
    }
}