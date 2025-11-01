use syn::{parse_quote, Item, GenericParam, Expr};

pub struct Modify_Trait_Alias_In_Const_Generics_64;

impl Mutator for Modify_Trait_Alias_In_Const_Generics_64 {
    fn name(&self) -> &str {
        "Modify_Trait_Alias_In_Const_Generics_64"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::TraitAlias(trait_alias) = item {
                if let Some(GenericParam::Const(const_param)) = trait_alias.generics.params.first_mut() {
                    if let Some(default) = &mut const_param.default {
                        if let Expr::Binary(_) = default.as_mut() {
                            let new_expr: Expr = parse_quote! { 6 / 2 };
                            *default = new_expr;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait aliases involving constant expressions in associated types. It identifies a trait alias declaration with a constant expression and modifies this expression to include a division operation, ensuring the divisor is non-zero. This alteration is intended to test the Rust compiler's handling of const generics, potentially revealing weaknesses in type inference and const evaluation logic."
    }
}