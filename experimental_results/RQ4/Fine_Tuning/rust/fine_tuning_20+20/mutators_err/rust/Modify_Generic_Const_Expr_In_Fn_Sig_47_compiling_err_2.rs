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

pub struct Modify_Generic_Const_Expr_In_Fn_Sig_47;

impl Mutator for Modify_Generic_Const_Expr_In_Fn_Sig_47 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expr_In_Fn_Sig_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut const_idents = HashSet::new();
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_idents.insert(const_param.ident.to_string());
                    }
                }
                if const_idents.is_empty() {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Array(type_array) = &*pat_type.ty {
                            if let syn::Expr::Path(expr_path) = &*type_array.len {
                                if expr_path.qself.is_none()
                                    && expr_path.path.leading_colon.is_none()
                                    && expr_path.path.segments.len() == 1
                                {
                                    let segment = expr_path.path.segments.first().unwrap();
                                    if const_idents.contains(&segment.ident.to_string()) {
                                        pat_type.ty = Box::new(syn::Type::Array(syn::TypeArray {
                                            bracket_token: type_array.bracket_token,
                                            elem: type_array.elem.clone(),
                                            len: syn::Expr::Binary(syn::ExprBinary {
                                                attrs: Vec::new(),
                                                left: Box::new(type_array.len.clone()),
                                                op: syn::BinOp::Add,
                                                right: Box::new(type_array.len.clone()),
                                            }),
                                            semi_token: type_array.semi_token,
                                        }));
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
        "Identify functions with const generics and modify array length expressions in the signature to use arithmetic operations on the const parameters, such as doubling the size by changing `[u8; N]` to `[u8; N + N]`."
    }
}