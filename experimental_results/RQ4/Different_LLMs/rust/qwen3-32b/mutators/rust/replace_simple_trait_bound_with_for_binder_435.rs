use syn::parse_quote;
use syn::punctuated::Punctuated;
use syn::token;
use crate::mutator::Mutator;

pub struct Replace_Simple_Trait_Bound_With_For_Binder_435;

impl Mutator for Replace_Simple_Trait_Bound_With_For_Binder_435 {
    fn name(&self) -> &str {
        "Replace_Simple_Trait_Bound_With_For_Binder_435"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                if type_path.path.segments.len() == 1 {
                                    let segment = &type_path.path.segments[0];
                                    if segment.ident != "Self" && !segment.ident.to_string().starts_with("__") {
                                        if type_pred.bounds.len() == 1 {
                                            if let syn::TypeParamBound::Trait(trait_bound) = &type_pred.bounds[0] {
                                                let mut lifetimes: Punctuated<syn::Lifetime, token::Comma> = Punctuated::new();
                                                lifetimes.push(parse_quote!('a));
                                                let new_predicate = parse_quote!(for <#lifetimes> #type_pred);
                                                *predicate = new_predicate;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(type_pred) = predicate {
                                    if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                        if type_path.path.segments.len() == 1 {
                                            let segment = &type_path.path.segments[0];
                                            if segment.ident != "Self" && !segment.ident.to_string().starts_with("__") {
                                                if type_pred.bounds.len() == 1 {
                                                    if let syn::TypeParamBound::Trait(trait_bound) = &type_pred.bounds[0] {
                                                        let mut lifetimes: Punctuated<syn::Lifetime, token::Comma> = Punctuated::new();
                                                        lifetimes.push(parse_quote!('a));
                                                        let new_predicate = parse_quote!(for <#lifetimes> #type_pred);
                                                        *predicate = new_predicate;
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
        ""
    }
}