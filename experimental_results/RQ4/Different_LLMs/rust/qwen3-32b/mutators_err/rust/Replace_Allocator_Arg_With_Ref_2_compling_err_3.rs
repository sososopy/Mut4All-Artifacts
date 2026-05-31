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

pub struct Replace_Allocator_Arg_With_Ref_2;

impl Mutator for Replace_Allocator_Arg_With_Ref_2 {
    fn name(&self) -> &str {
        "Replace_Allocator_Arg_With_Ref_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AllocatorArgVisitor;
        impl VisitMut for AllocatorArgVisitor {
            fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
                // Check if the function is Box::new_in
                if let Expr::Path(path_expr) = &*expr_call.func {
                    let path = &path_expr.path;
                    if path.segments.len() == 2 {
                        if path.segments[0].ident == "Box" && path.segments[1].ident == "new_in" {
                            // Modify the second argument
                            if let Some(arg) = expr_call.args.get_mut(1) {
                                let ampersand = token::And::default();
                                let new_expr = syn::Expr::AddrOf(syn::ExprAddrOf {
                                    and_token: ampersand,
                                    expr: Box::new(arg.clone()),
                                });
                                *arg = Box::new(new_expr);
                            }
                        }
                    }
                }
                // Continue visiting children
                syn::visit_mut::visit_expr_call_mut(self, expr_call);
            }
        }
        let mut visitor = AllocatorArgVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the second argument of `Box::new_in` function calls, replacing an owned allocator argument with a reference to an existing allocator instance using the address-of operator. This transformation introduces type mismatches in allocator API usage, testing the compiler's handling of ownership and reference-to-value coercion in allocator contexts."
    }
}