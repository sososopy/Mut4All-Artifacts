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

pub struct Replace_Generic_Array_Size_With_Nested_Expr_47;

impl Mutator for Replace_Generic_Array_Size_With_Nested_Expr_47 {
    fn name(&self) -> &str {
        "Replace_Generic_Array_Size_With_Nested_Expr_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &mut pat_type.ty;
                        if let Type::Array(array) = &mut *ty {
                            if let Expr::Path(_) = &*array.len {
                                let original_len = array.len.clone();
                                let new_len = parse_quote! { (#original_len - 1) + 1 + #original_len };
                                array.len = Box::new(new_len);
                            }
                        }
                    }
                }
            } else if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Array(array) = &mut *field.ty {
                        if let Expr::Path(_) = &*array.len {
                            let original_len = array.len.clone();
                            let new_len = parse_quote! { (#original_len - 1) + 1 + #original_len };
                            array.len = Box::new(new_len);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces direct uses of const generic parameters in array sizes with nested arithmetic expressions combining the same parameter. This introduces overlapping sub-expressions that may confuse const evaluators, testing their ability to handle complex but semantically equivalent expressions during compile-time evaluation."
    }
}