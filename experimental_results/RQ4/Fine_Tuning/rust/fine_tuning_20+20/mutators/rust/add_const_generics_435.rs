use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Add_Const_Generics_435;

impl Mutator for Add_Const_Generics_435 {
    fn name(&self) -> &str {
        "Add_Const_Generics_435"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "Copy" {
                                        predicate_type.bounds.push(parse_quote!(
                                            for<const NM: usize = { NP - 2 }>
                                        ));
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
        "Identify function signatures with const generics in the where clause, particularly those involving arithmetic expressions. Extend the where clause by adding a new const parameter that creates an interdependent relationship with existing consts, thereby increasing the complexity of const evaluation and testing the compiler's ability to handle intricate const generic scenarios."
    }
}