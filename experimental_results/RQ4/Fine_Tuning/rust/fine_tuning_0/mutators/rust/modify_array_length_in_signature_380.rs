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

pub struct Modify_Array_Length_In_Signature_380;

impl Mutator for Modify_Array_Length_In_Signature_380 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Signature_380"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut modified = false;
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &mut *pat_type.ty {
                            if let Expr::Path(expr_path) = &type_array.len {
                                if expr_path.path.segments.len() == 1 {
                                    let new_const_ident = Ident::new("NEW_SIZE", Span::call_site());
                                    type_array.len = Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(new_const_ident.clone()),
                                    });
                                    modified = true;
                                }
                            }
                        }
                    }
                }
                if modified {
                    let new_const_stmt: Stmt = parse_quote! {
                        const NEW_SIZE: usize;
                    };
                    func.block.stmts.insert(0, new_const_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function signatures with array parameters by replacing the array length with a new unevaluated const generic `NEW_SIZE`. This transformation introduces potential type mismatches and tests the compiler's handling of unevaluated const generics, aiming to uncover ICEs related to const generic evaluation and type resolution."
    }
}