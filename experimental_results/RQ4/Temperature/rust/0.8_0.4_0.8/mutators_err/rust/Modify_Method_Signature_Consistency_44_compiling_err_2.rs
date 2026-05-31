use std::collections::HashMap;

pub struct Modify_Method_Signature_Consistency_44;

impl Mutator for Modify_Method_Signature_Consistency_44 {
    fn name(&self) -> &str {
        "Modify_Method_Signature_Consistency_44"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_methods = extract_trait_methods(file);
        
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(trait_name) = path.segments.last() {
                        if let Some(methods) = trait_methods.get(&trait_name.ident.to_string()) {
                            for impl_item in &mut item_impl.items {
                                if let syn::ImplItem::Fn(method) = impl_item {
                                    if let Some(trait_method) = methods.get(&method.sig.ident.to_string()) {
                                        method.sig.inputs = trait_method.sig.inputs.clone();
                                        method.sig.output = trait_method.sig.output.clone();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies method signatures in trait implementations to ensure they match the corresponding method signatures in the trait definition. This involves aligning the number of parameters, their types, and the method names, which can help expose inconsistencies and trigger Rust compiler bugs related to specialization."
    }
}

fn extract_trait_methods(file: &syn::File) -> HashMap<String, HashMap<String, syn::TraitItemFn>> {
    let mut trait_methods = HashMap::new();
    
    for item in &file.items {
        if let syn::Item::Trait(item_trait) = item {
            let trait_name = item_trait.ident.to_string();
            let mut methods = HashMap::new();
            
            for item in &item_trait.items {
                if let syn::TraitItem::Fn(method) = item {
                    methods.insert(method.sig.ident.to_string(), method.clone());
                }
            }
            
            trait_methods.insert(trait_name, methods);
        }
    }
    
    trait_methods
}