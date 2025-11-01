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

pub struct Modify_Array_Length_181;

impl Mutator for Modify_Array_Length_181 {
    fn name(&self) -> &str {
        "Modify_Array_Length_181"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut modified = false;
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Array(type_array) = &**ty {
                            if let Expr::Path(ExprPath { path, .. }) = &type_array.len {
                                if path.segments.len() == 1 {
                                    let new_generic = Ident::new("NEW_SIZE", Span::call_site());
                                    type_array.len = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath::from(new_generic.clone()),
                                    });
                                    item_fn.block.stmts.insert(0, Stmt::Item(Item::Const(parse_quote! {
                                        const NEW_SIZE: usize;
                                    })));
                                    modified = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if modified {
                    break;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with array parameters using const generics. It modifies the array length to use a new const generic, NEW_SIZE, which is declared but not instantiated within the function. This leverages Rust's handling of unevaluated const generics to potentially trigger ICEs related to type mismatches."
    }
}