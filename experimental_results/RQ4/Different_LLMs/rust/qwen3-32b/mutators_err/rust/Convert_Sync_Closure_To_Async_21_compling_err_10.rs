use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Convert_Sync_Closure_To_Async_21;

impl Mutator for Convert_Sync_Closure_To_Async_21 {
    fn name(&self) -> &str {
        "Convert_Sync_Closure_To_Async_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut visitor = ClosureToAsyncVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ClosureToAsyncVisitor;

impl VisitMut for ClosureToAsyncVisitor {
    fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
        if let Some(syn::token::ClosureCapture::Move) = &i.capture {
            // Skip move closures
        } else {
            // Convert non-move closures to async
            i.asyncness = Some(token::Async::ne(Span::call_site()));
        }
        syn::visit_mut::visit_expr_closure_mut(self, i);
    }
}