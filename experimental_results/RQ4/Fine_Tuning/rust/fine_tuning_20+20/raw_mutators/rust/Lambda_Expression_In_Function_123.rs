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

pub struct Lambda_Expression_In_Function_123;

impl Mutator for Lambda_Expression_In_Function_123 {
    fn name(&self) -> &str {
        "Lambda_Expression_In_Function_123"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LambdaVisitor {
            mutated: bool,
        }
        impl VisitMut for LambdaVisitor {
            fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
                if self.mutated {
                    return;
                }
                if node.body.is_yield() {
                    node.body = parse_quote!(|| {});
                } else {
                    node.body = parse_quote!(yield);
                }
                self.mutated = true;
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = LambdaVisitor { mutated: false };
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions containing lambda expressions by modifying the lambda body to include a `yield` expression. If the lambda already contains a yield, it replaces it with a different expression. This transformation leverages the compiler's handling of generators and lambda expressions to explore potential vulnerabilities in type-checking and code generation."
    }
}