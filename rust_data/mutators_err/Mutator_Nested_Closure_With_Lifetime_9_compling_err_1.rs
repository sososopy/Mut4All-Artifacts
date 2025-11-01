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

pub struct Mutator_Nested_Closure_With_Lifetime_9;

impl Mutator for Mutator_Nested_Closure_With_Lifetime_9 {
    fn name(&self) -> &str {
        "Mutator_Nested_Closure_With_Lifetime_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let mut visitor = ClosureVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within functions. It introduces a nested closure inside the identified closure. The nested closure uses a lifetime parameter from the outer closure within its signature. The outer closure is modified to use explicit lifetime parameters in its signature. This mutation exposes potential issues related to lifetime handling in nested closures."
    }
}

struct ClosureVisitor;

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, closure: &mut ExprClosure) {
        if closure.inputs.is_empty() {
            let outer_lifetime: Lifetime = parse_quote!('a);
            let inner_lifetime: Lifetime = parse_quote!('b);

            closure.inputs = parse_quote!(for<#outer_lifetime> |_: &'a i32| -> ());
            let inner_closure: ExprClosure = parse_quote! {
                for<#inner_lifetime> |_: &'a i32| -> () {
                    // Inner closure body using outer lifetime 'a
                }
            };

            let inner_call: Stmt = parse_quote! {
                let closure_inner = #inner_closure;
                closure_inner(42);
            };

            if let Expr::Block(ref mut block) = *closure.body {
                block.block.stmts.insert(0, inner_call);
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, closure);
    }
}