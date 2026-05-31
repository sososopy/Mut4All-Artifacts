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

pub struct Nested_Closure_With_Lifetime_Binder_40;

impl Mutator for Nested_Closure_With_Lifetime_Binder_40 {
    fn name(&self) -> &str {
        "Nested_Closure_With_Lifetime_Binder_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureVisitor::new();
                visitor.visit_item_fn(item_fn);
                for closure_expr in visitor.closures {
                    let outer_lifetime: Lifetime = parse_quote! { 'a };
                    let inner_lifetime: Lifetime = parse_quote! { 'b };
                    let inner_closure: Expr = parse_quote! {
                        for #inner_lifetime |_: &#outer_lifetime i32| -> () {}
                    };
                    let inner_closure_call: Expr = parse_quote! { (42) };
                    let outer_closure: Expr = parse_quote! {
                        for #outer_lifetime || -> () {
                            let closure_inner = #inner_closure;
                            closure_inner #inner_closure_call;
                        }
                    };
                    *closure_expr = outer_closure;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets closures within functions, replacing each closure with a nested closure structure that uses explicit lifetime binders. The outer closure is transformed to include a lifetime parameter in its signature, and a nested closure is introduced that references this outer lifetime. This transformation is designed to stress the compiler's handling of closure lifetimes and binder normalization, potentially exposing issues related to nested closure lifetime resolution."
    }
}

struct ClosureVisitor {
    closures: Vec<Expr>,
}

impl ClosureVisitor {
    fn new() -> Self {
        Self {
            closures: Vec::new(),
        }
    }
}

impl<'ast> Visit<'ast> for ClosureVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Closure(_) = node {
            self.closures.push(node.clone());
        }
        syn::visit::visit_expr(self, node);
    }
}