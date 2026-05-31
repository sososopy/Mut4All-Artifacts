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

pub struct Modify_Array_Length_In_Function_Signature_380;

impl Mutator for Modify_Array_Length_In_Function_Signature_380 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Function_Signature_380"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found = false;
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &*pat_type.ty {
                            if let Type::Array(type_array) = &*type_reference.elem {
                                if let Expr::Path(expr_path) = &type_array.len {
                                    if let Some(last_segment) = expr_path.path.segments.last() {
                                        if last_segment.ident != "NEW_SIZE" {
                                            found = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if found {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_reference) = &mut *pat_type.ty {
                                if let Type::Array(type_array) = &mut *type_reference.elem {
                                    type_array.len = Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(Ident::new(
                                            "NEW_SIZE",
                                            Span::call_site(),
                                        )),
                                    });
                                }
                            }
                        }
                    }
                    let new_const: Stmt = parse_quote! {
                        const NEW_SIZE: usize;
                    };
                    item_fn.block.stmts.insert(0, new_const);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with array parameters using const generics for length. It replaces the array length with a new unevaluated const generic `NEW_SIZE` and injects a corresponding const declaration inside the function body. This transformation exploits the Rust compiler's handling of unevaluated const generics, aiming to trigger ICEs related to type mismatches and const evaluation."
    }
}