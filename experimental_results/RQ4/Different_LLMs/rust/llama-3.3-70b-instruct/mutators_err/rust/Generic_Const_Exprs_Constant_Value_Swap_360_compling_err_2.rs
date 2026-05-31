impl Mutator for Generic_Const_Exprs_Constant_Value_Swap_360 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Constant_Value_Swap_360"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::Generics::None = func.sig.generics {
                    continue;
                }
                let generics = func.sig.generics.get_mut().unwrap();
                let const_params: Vec<&mut GenericParam> = generics
                    .params
                    .iter_mut()
                    .filter(|param| {
                        if let GenericParam::Const(_) = param {
                            true
                        } else {
                            false
                        }
                    })
                    .collect();

                if const_params.len() > 1 {
                    let mut rng = thread_rng();
                    const_params.shuffle(&mut rng);

                    let new_params: Vec<GenericParam> = const_params
                        .iter()
                        .map(|param| param.clone())
                        .collect();

                    generics.params = new_params;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the constant values of generic const expression parameters in function definitions. This transformation aims to test the compiler's handling of generic const expressions and may lead to ICEs or inference failures in the const evaluation system."
    }
}