use syn::parse_quote;
use crate::mutator::Mutator;
use syn::GenericParam;
use syn::punctuated::Punctuated;

pub struct Add_HigherRanked_TraitBound_30;

impl Mutator for Add_HigherRanked_TraitBound_30 {
    fn name(&self) -> &str {
        "Add_HigherRanked_TraitBound_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                if let Some(generics) = s.generics.as_mut() {
                    if let Some(first_param) = generics.params.iter().next() {
                        if let GenericParam::Type(ty) = first_param {
                            let param_name = &ty.ident;
                            let predicate = parse_quote! {
                                for<'a> #param_name::Assoc<'a>: Send
                            };
                            let where_clause = generics.where_clause.get_or_insert_with(|| {
                                syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: Punctuated::new(),
                                }
                            });
                            where_clause.predicates.push(predicate);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = impl_item.generics.as_mut() {
                    if let Some(first_param) = generics.params.iter().next() {
                        if let GenericParam::Type(ty) = first_param {
                            let param_name = &ty.ident;
                            let predicate = parse_quote! {
                                for<'a> #param_name::Assoc<'a>: Send
                            };
                            let where_clause = generics.where_clause.get_or_insert_with(|| {
                                syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: Punctuated::new(),
                                }
                            });
                            where_clause.predicates.push(predicate);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a higher-ranked trait bound with a named lifetime in the where clause of structs and impl blocks that have generic type parameters. This transformation forces the compiler to handle universal regions and associated type lifetimes, potentially exposing issues in region inference and trait resolution for higher-ranked bounds."
    }
}