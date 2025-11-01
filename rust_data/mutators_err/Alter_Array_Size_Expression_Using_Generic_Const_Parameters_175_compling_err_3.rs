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

pub struct Alter_Array_Size_Expression_Using_Generic_Const_Parameters_175;

impl Mutator for Alter_Array_Size_Expression_Using_Generic_Const_Parameters_175 {
    fn name(&self) -> &str {
        "Alter_Array_Size_Expression_Using_Generic_Const_Parameters_175"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ItemFn { sig, .. }) = item {
                if let Some(const_param) = sig.generics.params.iter().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param.ident.clone())
                    } else {
                        None
                    }
                }) {
                    for input in &mut sig.inputs {
                        if let FnArg::Typed(PatType { ty, .. }) = input {
                            if let Type::Array(type_array) = &mut **ty {
                                if let Expr::Lit(expr_lit) = &mut type_array.len {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        if lit_int.base10_parse::<usize>().is_ok() {
                                            *expr_lit = Expr::Lit(syn::ExprLit {
                                                attrs: Vec::new(),
                                                lit: syn::Lit::Verbatim(parse_quote!({ #const_param + 1 })),
                                            });
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
        "This mutation operator targets array type definitions in function parameters where the size is determined by a const expression involving a generic const parameter. It modifies the const expression to include additional arithmetic operations involving the generic const parameter, such as changing [u8; { N }] to [u8; { N + 1 }]. This helps in testing const evaluations and uncovering potential bugs in the compiler."
    }
}