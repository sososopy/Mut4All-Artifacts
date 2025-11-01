use crate::mutator::Mutator;

pub struct Modify_Generic_Constants_401;

impl Mutator for Modify_Generic_Constants_401 {
    fn name(&self) -> &str {
        "Modify_Generic_Constants_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(generic_argument) = trait_bound.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(ref mut args) = generic_argument.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(Expr::Lit(expr_lit)) = arg {
                                                    if let syn::Lit::Int(ref mut lit_int) = expr_lit.lit {
                                                        if let Ok(value) = lit_int.base10_parse::<usize>() {
                                                            let new_value = value + 2; // Modify constant value
                                                            *lit_int = syn::LitInt::new(&new_value.to_string(), lit_int.span());
                                                        }
                                                    }
                                                }
                                            }
                                        }
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
        "This mutator identifies function signatures with generic parameters and constant expressions in the where clause. It modifies these constant expressions by introducing a new constant value, ensuring the type and structure are maintained. This tests the compiler's ability to handle different constant expressions within generic contexts."
    }
}