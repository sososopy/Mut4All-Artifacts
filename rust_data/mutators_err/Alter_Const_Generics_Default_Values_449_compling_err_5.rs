use syn::parse_quote;
use syn::Item;
use syn::GenericParam;
use crate::mutator::Mutator;

pub struct Alter_Const_Generics_Default_Values_449;

impl Mutator for Alter_Const_Generics_Default_Values_449 {
    fn name(&self) -> &str {
        "Alter_Const_Generics_Default_Values_449"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(GenericParam::Const(const_param)) = item_struct.generics.params.first_mut() {
                    if let Some(default) = &mut const_param.default {
                        *default = parse_quote!({ let x: #default = Default::default(); x });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with const generics having default values. It alters the default value of the const generic parameter to an expression that introduces additional complexity, such as using a `let` binding. This mutation challenges the compiler's handling of const generics and default values, potentially exposing issues in const evaluation paths."
    }
}