use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Add_Const_Generics_92;

impl Mutator for Add_Const_Generics_92 {
    fn name(&self) -> &str {
        "Add_Const_Generics_92"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(where_clause) = &mut item_struct.generics.where_clause {
                    let mut has_const_generic = false;
                    for param in &item_struct.generics.params {
                        if let GenericParam::Const(_) = param {
                            has_const_generic = true;
                            break;
                        }
                    }
                    if has_const_generic {
                        item_struct.generics.params.push(parse_quote!(const N1: usize));
                        item_struct.generics.params.push(parse_quote!(const N2: usize));

                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                if let Type::Array(type_array) = &predicate_type.bounded_ty {
                                    if let Expr::Path(expr_path) = &*type_array.len {
                                        if let Some(segment) = expr_path.path.segments.last() {
                                            if segment.ident == "N" {
                                                predicate_type.bounded_ty = parse_quote!([(); N1 + N2]);
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
        "The mutation operator locates struct definitions using const generics and introduces two additional const generic parameters. It updates the where clause to use a combination of these new parameters, transforming constraints like `[(); S::N]: Sized` to `[(); N1 + N2]: Sized`. This tests the compiler's handling of const generics in struct definitions and ensures compatibility with the new signature."
    }
}