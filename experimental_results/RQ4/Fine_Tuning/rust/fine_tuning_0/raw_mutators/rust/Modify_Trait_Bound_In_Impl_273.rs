use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Bound_In_Impl_273;

impl Mutator for Modify_Trait_Bound_In_Impl_273 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Impl_273"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, ref mut where_clause)) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(TypePath { path, .. }) = &predicate_type.bounded_ty {
                                if let Some(segment) = path.segments.last() {
                                    if segment.ident == "Fg" {
                                        let new_bound: TypeParamBound = parse_quote!(Over<NewBg, NewFg>);
                                        predicate_type.bounds.clear();
                                        predicate_type.bounds.push(new_bound);
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
        "The mutation operator targets trait bounds in impl blocks, specifically altering the trait bounds in where clauses. By changing a trait bound like `Fg: Over<C, NewFg>` to `Fg: Over<NewBg, NewFg>`, it introduces potential type discrepancies. This transformation tests the compiler's ability to handle altered trait constraints and associated type resolution, potentially exposing bugs in trait selection logic."
    }
}