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

pub struct Alter_Const_Parameter_Width_383;

impl Mutator for Alter_Const_Parameter_Width_383 {
    fn name(&self) -> &str {
        "Alter_Const_Parameter_Width_383"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generic_param) = item_struct.generics.params.first_mut() {
                    if let GenericParam::Const(const_param) = generic_param {
                        if let Type::Array(type_array) = &const_param.ty {
                            if let Expr::Lit(expr_lit) = &type_array.len {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    if let Ok(original_size) = lit_int.base10_parse::<usize>() {
                                        let new_size = if original_size > 1 {
                                            original_size - 1
                                        } else {
                                            original_size + 1
                                        };
                                        let new_size_lit = syn::LitInt::new(
                                            &new_size.to_string(),
                                            lit_int.span(),
                                        );
                                        let new_expr_lit = Expr::Lit(ExprLit {
                                            attrs: vec![],
                                            lit: syn::Lit::Int(new_size_lit),
                                        });
                                        type_array.len = *Box::new(new_expr_lit);
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
        "The mutation operator identifies struct definitions using const generics with array types as const parameters. It alters the array size in the const parameter, either incrementing or decrementing the size by one. This change is made to expose potential issues in the Rust compiler's handling of const generic parameters with varying array sizes, which can lead to internal compiler errors if not managed correctly."
    }
}