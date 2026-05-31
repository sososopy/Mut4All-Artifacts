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

pub struct Replace_Closure_Capture_44;

impl Mutator for Replace_Closure_Capture_44 {
    fn name(&self) -> &str {
        "Replace_Closure_Capture_44"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Closure(closure) = expr {
                            if let Some(capture) = &closure.capture {
                                let new_capture = match capture {
                                    syn::expr::ClosureCapture::ByRef(_) => syn::expr::ClosureCapture::ByValue(token::Move { span: Span::call_site() }),
                                    syn::expr::ClosureCapture::ByValue(token::Move { .. }) => syn::expr::ClosureCapture::ByRef(token::Ref { span: Span::call_site() }),
                                    syn::expr::ClosureCapture::ByValue(_) => syn::expr::ClosureCapture::ByValue(token::Move { span: Span::call_site() }),
                                };
                                closure.capture = Some(new_capture);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure expressions and replaces their capture type. This transformation can affect how variables are captured and used within the closure, potentially triggering issues related to the compiler's handling of closure captures."
    }
}