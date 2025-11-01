use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;
use syn::GenericParam;

pub struct Modify_Const_Generic_Params_114;

impl Mutator for Modify_Const_Generic_Params_114 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Params_114"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if let Some(GenericParam::Const(const_param)) = trait_item.generics.params.first_mut() {
                    const_param.ty = parse_quote!(i32);
                    const_param.default = Some(parse_quote!(-1));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies trait definitions with constant generic parameters and modifies the type and value of these parameters. Specifically, it changes the type to `i32` and the value to `-1`, ensuring compatibility with the trait's existing functions or associated items."
    }
}