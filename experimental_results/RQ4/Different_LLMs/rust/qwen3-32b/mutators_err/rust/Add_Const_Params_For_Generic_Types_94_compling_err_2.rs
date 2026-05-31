pub struct Add_Const_Params_For_Generic_Types_94;

impl Mutator for Add_Const_Params_For_Generic_Types_94 {
    fn name(&self) -> &str {
        "Add_Const_Params_For_Generic_Types_94"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_params = Punctuated::new();
                for param in &func.sig.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: usize);
                        new_params.push(new_const);
                    }
                }
                func.sig.generics.params = new_params;
            } else if let syn::Item::Struct(struct_item) = item {
                let mut new_params = Punctuated::new();
                for param in &struct_item.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: usize);
                        new_params.push(new_const);
                    }
                }
                struct_item.generics.params = new_params;
            } else if let syn::Item::Trait(trait_item) = item {
                let mut new_params = Punctuated::new();
                for param in &trait_item.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: usize);
                        new_params.push(new_const);
                    }
                }
                trait_item.generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}