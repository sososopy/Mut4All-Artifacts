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

pub struct Modify_Box_New_In_With_Mutable_Refs_451;

impl Mutator for Modify_Box_New_In_With_Mutable_Refs_451 {
    fn name(&self) -> &str {
        "Modify_Box_New_In_With_Mutable_Refs_451"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = BoxNewInVisitor;
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls using `Box::new_in` with an allocator argument. It introduces a mutable reference to the data being boxed, simulating potential allocator mismanagement. This transformation can expose issues with memory allocation and undefined behavior, challenging the compiler's handling of memory safety and allocation APIs."
    }
}

struct BoxNewInVisitor;

impl VisitMut for BoxNewInVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if path.segments.len() == 2 && path.segments[0].ident == "Box" && path.segments[1].ident == "new_in" {
                if node.args.len() == 2 {
                    let first_arg = &mut node.args[0];
                    *first_arg = parse_quote!(&mut #first_arg);
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}