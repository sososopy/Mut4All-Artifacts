use crate::mutator::Mutator;

pub struct Modify_Trait_Bounds_54;

impl Mutator for Modify_Trait_Bounds_54 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_54"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut new_items = Vec::new();
                for item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = item {
                        if let Some(where_clause) = &method.sig.generics.where_clause {
                            let new_trait_bound: TypeParamBound = parse_quote!(+ AnotherTrait<M>);
                            let mut new_where_clause = where_clause.clone();
                            for predicate in &mut new_where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    predicate_type.bounds.push(new_trait_bound.clone());
                                }
                            }
                            let new_method = syn::ImplItem::Fn(syn::ImplItemFn {
                                attrs: method.attrs.clone(),
                                vis: method.vis.clone(),
                                defaultness: method.defaultness.clone(),
                                sig: syn::Signature {
                                    ident: Ident::new(
                                        &format!("{}_complex", method.sig.ident),
                                        method.sig.ident.span(),
                                    ),
                                    generics: syn::Generics {
                                        where_clause: Some(new_where_clause),
                                        ..method.sig.generics.clone()
                                    },
                                    ..method.sig.clone()
                                },
                                block: method.block.clone(),
                            });
                            new_items.push(syn::ImplItem::Fn(new_method));
                        }
                    }
                }
                item_impl.items.extend(new_items.into_iter().map(|item| syn::ImplItem::Fn(item)));
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on modifying trait bounds in function definitions within implementation blocks. It identifies functions with a `where` clause and introduces a new function with additional complex trait bounds. This is achieved by cloning the original function and appending a new trait bound that is more restrictive or potentially conflicting, thus increasing the complexity of trait satisfaction and exploring edge cases in the Rust compiler."
    }
}