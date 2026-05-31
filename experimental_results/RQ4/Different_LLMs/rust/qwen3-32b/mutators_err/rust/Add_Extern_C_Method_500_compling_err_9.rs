use syn::parse_quote;
use crate::mutator::Mutator;
use syn::ItemFn;

pub struct Add_Extern_C_Method_500;

impl Mutator for Add_Extern_C_Method_500 {
    fn name(&self) -> &str {
        "Add_Extern_C_Method_500"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let new_method: ItemFn = parse_quote! {
                    extern "C" fn mutated_extern_c_method(&self) -> i32 {
                        0
                    }
                };
                trait_item.items.push(syn::TraitItem::Fn(syn::TraitItemFn {
                    attrs: Vec::new(),
                    sig: new_method.sig,
                    default: Some(new_method.block.clone()),
                    semi_token: None,
                }));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}