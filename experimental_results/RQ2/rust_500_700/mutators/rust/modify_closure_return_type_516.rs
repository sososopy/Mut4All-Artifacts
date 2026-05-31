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

pub struct Modify_Closure_Return_Type_516;

impl Mutator for Modify_Closure_Return_Type_516 {
    fn name(&self) -> &str {
        "Modify_Closure_Return_Type_516"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureReturnTypeVisitor;
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within function bodies, altering their return types by introducing type conversions. This transformation aims to test the compiler's handling of closure return type inference and compatibility with surrounding code."
    }
}

struct ClosureReturnTypeVisitor;

impl VisitMut for ClosureReturnTypeVisitor {
    fn visit_expr_closure_mut(&mut self, closure: &mut ExprClosure) {
        if let ReturnType::Type(_, output_type) = &closure.output {
            if let Type::Path(type_path) = &**output_type {
                if type_path.path.segments.last().unwrap().ident == "i32" {
                    closure.output = ReturnType::Type(Default::default(), Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(f64),
                    })));
                    if let Expr::Block(expr_block) = &mut *closure.body {
                        if let Some(Stmt::Expr(expr, _)) = expr_block.block.stmts.last_mut() {
                            *expr = parse_quote! { (#expr) as f64 };
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_closure_mut(self, closure);
    }
}