use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Expr, ExprLit, File, Item, ItemImpl, Lit, Path as SynPath, Type, WherePredicate, parse_quote,
    punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Mutate_Generic_Const_Expressions_37;

impl Mutator for Mutate_Generic_Const_Expressions_37 {
    fn name(&self) -> &str {
        "Mutate_Generic_Const_Expressions_37"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if trait_path.segments.last().unwrap().ident == "Add" {
                        if let Some(where_clause) = &mut item_impl.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Array(array_type) = &predicate_type.bounded_ty {
                                        if let Expr::Lit(ExprLit { lit: Lit::Int(lit_int), .. }) = &array_type.len {
                                            let original_value = lit_int.base10_parse::<usize>().unwrap();
                                            let new_value = format!("{}", original_value * 2);
                                            array_type.len = parse_quote! { #new_value };
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
        "The mutation operator targets trait implementations with const generic parameters and modifies the constant expressions in the `where` clause. It specifically looks for arithmetic operations and alters them by changing the values or operations to introduce variation. This change stresses the compiler's const generic handling and aims to expose potential issues in constraint resolution."
    }
}