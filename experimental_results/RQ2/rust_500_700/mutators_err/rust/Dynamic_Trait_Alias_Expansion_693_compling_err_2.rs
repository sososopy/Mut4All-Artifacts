use std::collections::HashMap;

pub struct Dynamic_Trait_Alias_Expansion_693;

impl Mutator for Dynamic_Trait_Alias_Expansion_693 {
    fn name(&self) -> &str {
        "Dynamic_Trait_Alias_Expansion_693"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alias_map = HashMap::new();

        // Collect all type aliases for dynamic traits
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::TraitObject(type_trait_object) = &*item_type.ty {
                    alias_map.insert(item_type.ident.clone(), type_trait_object.clone());
                }
            }
        }

        // Expand type aliases in function signatures
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(type_trait_object) = alias_map.get(&type_path.path.segments[0].ident) {
                                pat_type.ty = Box::new(Type::TraitObject(type_trait_object.clone()));
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(type_trait_object) = alias_map.get(&type_path.path.segments[0].ident) {
                            *return_type = Box::new(Type::TraitObject(type_trait_object.clone()));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}