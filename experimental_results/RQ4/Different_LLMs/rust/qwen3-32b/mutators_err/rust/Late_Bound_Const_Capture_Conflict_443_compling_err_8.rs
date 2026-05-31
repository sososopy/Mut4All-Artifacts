use syn::parse_quote;
use syn::punctuated::Punctuated;
use syn::Meta::NestedMeta;
use crate::mutator::Mutator;

struct Late_Bound_Const_Capture_Conflict_443;

impl Mutator for Late_Bound_Const_Capture_Conflict_443 {
    fn name(&self) -> &str {
        "Late_Bound_Const_Capture_Conflict_443"
    }
    fn mutate(&self, file: &mut syn::File) {
        if !file.attrs.iter().any(|attr| {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    let nested = match list.parse_args_with(Punctuated::parse_terminated) {
                        Ok(n) => n,
                        Err(_) => return false,
                    };
                    nested.iter().any(|n| {
                        if let NestedMeta::Meta(meta) = n {
                            if let syn::Meta::NameValue(name_val) = meta {
                                if name_val.path.is_ident("non_lifetime_binders") {
                                    return true;
                                }
                            }
                        }
                        false
                    })
                } else {
                    false
                }
            } else {
                false
            }
        }) {
            let feature_attr = parse_quote! {
                #![feature(non_lifetime_binders)]
            };
            file.attrs.insert(0, feature_attr);
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in predicate_type.bounds.iter() {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let for_bound = parse_quote!(for<const C: usize> #trait_bound);
                                    new_bounds.push_value(for_bound);
                                    if !new_bounds.is_empty() {
                                        new_bounds.push_punct(Default::default());
                                    }
                                } else {
                                    new_bounds.push_value(bound.clone());
                                    if !new_bounds.is_empty() {
                                        new_bounds.push_punct(Default::default());
                                    }
                                }
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if let Some(where_clause) = &mut impl_item.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in predicate_type.bounds.iter() {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let for_bound = parse_quote!(for<const C: usize> #trait_bound);
                                    new_bounds.push_value(for_bound);
                                    if !new_bounds.is_empty() {
                                        new_bounds.push_punct(Default::default());
                                    }
                                } else {
                                    new_bounds.push_value(bound.clone());
                                    if !new_bounds.is_empty() {
                                        new_bounds.push_punct(Default::default());
                                    }
                                }
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in predicate_type.bounds.iter() {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            let for_bound = parse_quote!(for<const C: usize> #trait_bound);
                                            new_bounds.push_value(for_bound);
                                            if !new_bounds.is_empty() {
                                                new_bounds.push_punct(Default::default());
                                            }
                                        } else {
                                            new_bounds.push_value(bound.clone());
                                            if !new_bounds.is_empty() {
                                                new_bounds.push_punct(Default::default());
                                            }
                                        }
                                    }
                                    predicate_type.bounds = new_bounds;
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