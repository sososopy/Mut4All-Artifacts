use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Change_Safe_Fn_Pointer_To_Unsafe_221;

impl Mutator for Change_Safe_Fn_Pointer_To_Unsafe_221 {
    fn name(&self) -> &str {
        "Change_Safe_Fn_Pointer_To_Unsafe_221"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = FnPointerVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies variable bindings (static items and let bindings) with explicit safe function pointer type annotations (fn(...) -> ...) and changes them to unsafe function pointer types (unsafe fn(...) -> ...) while keeping the initializer unchanged. This transformation targets the compiler's handling of unsafe function pointer casts during MIR optimization, particularly when the initializer (closure or function pointer) lacks the same safety qualifier, potentially exposing type coercion bugs and safety checking issues."
    }
}

struct FnPointerVisitor;

impl VisitMut for FnPointerVisitor {
    fn visit_item_static_m2ut(&mut self, i: &mut syn::ItemStatic) {
        if let Type::BareFn(bare_fn) = &mut *i.ty {
            if bare_fn.unsafety.is_none() {
                bare_fn.unsafety = Some(token::Unsafe::default());
            }
        }
        syn::visit_mut::visit_item_static_mut(self, i);
    }
    
    fn visit_local_mut(&mut self, i: &mut syn::Local) {
        if let Pat::Type(pat_type) = &mut i.pat {
            if let Type::BareFn(bare_fn) = &mut *pat_type.ty {
                if bare_fn.unsafety.is_none() {
                    bare_fn.unsafety = Some(token::Unsafe::default());
                }
            }
        }
        syn::visit_mut::visit_local_mut(self, i);
    }
    
    fn visit_item_mut(&mut self, i: &mut syn::Item) {
        match i {
            Item::Static(item_static) => self.visit_item_static_mut(item_static),
            _ => syn::visit_mut::visit_item_mut(self, i),
        }
    }
}