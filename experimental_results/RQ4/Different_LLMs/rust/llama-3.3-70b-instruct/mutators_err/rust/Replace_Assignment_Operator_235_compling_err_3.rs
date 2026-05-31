use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Assignment_Operator_235;

impl Mutator for Replace_Assignment_Operator_235 {
    fn name(&self) -> &str {
        "Replace_Assignment_Operator_235"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                const_item.tok = token::Const { span: Span::call_site() };
            }
            if let Item::Fn(fn_item) = item {
                for arg in &mut fn_item.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Pat::Type(pat) = &mut *pat_type.pat {
                            pat.colon_token = token::Colon { spans: [Span::call_site()] };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the assignment operator in variable declarations and replaces it with a similar-looking Unicode character, such as replacing '=' with '⩵', to test the compiler's ability to handle Unicode characters and distinguish between them."
    }
}