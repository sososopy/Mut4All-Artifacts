use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Remove_Const_From_ImplTrait_Closure_Fn_464;

impl Mutator for Remove_Const_From_ImplTrait_Closure_Fn_464 {
    fn name(&self) -> &str {
        "Remove_Const_From_ImplTrait_Closure_Fn_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.constness.is_some() {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let mut visitor = ClosureFinder { found: false };
                        visitor.visit_block(&item_fn.block);
                        if visitor.found {
                            item_fn.sig.constness = None;
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if !method.sig.constness.is_some() {
                            continue;
                        }
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(_) = &**return_type {
                                let mut visitor = ClosureFinder { found: false };
                                visitor.visit_block(&method.block);
                                if visitor.found {
                                    method.sig.constness = None;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const functions that return an opaque impl Trait and contain an unboxed closure in their body. It removes the const qualifier from the function signature, transforming it into a regular non-const function. This mutation aims to expose bugs in the compiler's handling of const evaluation, closure type inference, and transmutability analysis, particularly with nightly features like effects and const trait implementations. By removing constness in contexts where it may be required, it can trigger internal compiler errors related to upvar_tys being called before capture types are inferred."
    }
}

struct ClosureFinder {
    found: bool,
}

impl<'ast> Visit<'ast> for ClosureFinder {
    fn visit_expr_closure(&mut self, _: &'ast syn::ExprClosure) {
        self.found = true;
    }
}