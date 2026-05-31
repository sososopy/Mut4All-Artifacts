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

pub struct Modify_Generic_Const_Exprs_In_Struct_35;

impl Mutator for Modify_Generic_Const_Exprs_In_Struct_35 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Exprs_In_Struct_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut generics = item_struct.generics.clone();
                let mut rng = thread_rng();
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Path(expr_path) = &type_array.len {
                            let new_expr: Expr = match rng.gen_range(0..3) {
                                0 => parse_quote! { #expr_path + 1 },
                                1 => parse_quote! { #expr_path * 2 },
                                2 => {
                                    let new_generic: GenericParam = parse_quote! { const N: usize };
                                    if !generics.params.iter().any(|param| param == &new_generic) {
                                        generics.params.push(new_generic);
                                    }
                                    parse_quote! { #expr_path + N }
                                }
                                _ => unreachable!(),
                            };
                            type_array.len = new_expr;
                        }
                    }
                }
                item_struct.generics = generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}