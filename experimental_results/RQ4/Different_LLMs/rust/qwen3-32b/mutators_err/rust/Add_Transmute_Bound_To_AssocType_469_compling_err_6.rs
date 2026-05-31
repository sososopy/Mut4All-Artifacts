use syn::parse_quote;
use crate::mutator::Mutator;
use std::collections::HashMap;
use syn::punctuated::Punctuated;

pub struct Add_Transmute_Bound_To_AssocType_469;

impl Mutator for Add_Transmute_Bound_To_AssocType_469 {
    fn name(&self) -> &str {
        "Add_Transmute_Bound_To_AssocType_469"
    }
    fn mutate(&self, file: &mut syn::File) {
        use syn::TraitItem::*;
        use syn::Type::Path;

        let mut modified_traits = HashMap::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if !trait_item.items.is_empty() {
                    for item in &mut trait_item.items {
                        if let Type(ty) = item {
                            let bound = parse_quote!(+ for<'a> std::mem::TransmuteFrom<(), ()>);
                            ty.bounds.push(bound);
                            modified_traits.insert(trait_item.ident.clone(), ty.ident.clone());
                            break;
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    if let Some(trait_ident) = trait_path.get_ident() {
                        if let Some(assoc_name) = modified_traits.get(trait_ident) {
                            let self_type = impl_item.self_ty.clone();
                            let self_type_name = if let Path(type_path) = *self_type {
                                if let Some(segment) = type_path.path.segments.last() {
                                    segment.ident.to_string()
                                } else {
                                    continue;
                                }
                            } else {
                                continue;
                            };

                            let where_clause = impl_item.generics.where_clause.get_or_insert_with(|| {
                                syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: Punctuated::new(),
                                }
                            });

                            let predicate = parse_quote!(#self_type_name::#assoc_name: + for<'a> std::mem::TransmuteFrom<(), ()>);
                            where_clause.predicates.push_value(predicate);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a higher-ranked transmutability bound (`for<'a> std::mem::TransmuteFrom<(), ()>`) on an associated type within a trait's definition and its implementing `impl` blocks. This forces the compiler to normalize the associated type under a complex trait constraint, potentially exposing normalization failures or ICEs during trait resolution and type inference."
    }
}