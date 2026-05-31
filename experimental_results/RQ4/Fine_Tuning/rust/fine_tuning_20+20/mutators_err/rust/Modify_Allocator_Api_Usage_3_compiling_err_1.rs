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

pub struct Modify_Allocator_Api_Usage_3;

impl Mutator for Modify_Allocator_Api_Usage_3 {
    fn name(&self) -> &str {
        "Modify_Allocator_Api_Usage_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident != "init_slice" {
                    continue;
                }
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::For(for_stmt) = stmt {
                        if let Expr::MethodCall(method_call) = &*for_stmt.expr {
                            if method_call.method == "enumerate" {
                                let mut new_for_stmt = for_stmt.clone();
                                let new_for_expr: Expr = parse_quote! {
                                    s.iter_mut().enumerate()
                                };
                                new_for_stmt.expr = Box::new(new_for_expr);
                                let new_for_body: Block = parse_quote! {
                                    {
                                        if i % 2 == 0 {
                                            *a = MaybeUninit::new(f(i));
                                        } else {
                                            *a = MaybeUninit::new(default_value());
                                        }
                                        guard.len += 1;
                                    }
                                };
                                new_for_stmt.body = new_for_body;
                                new_stmts.push(Stmt::For(new_for_stmt));
                                continue;
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that initialize a slice with `MaybeUninit` elements, specifically the `init_slice` function. It modifies the initialization loop to conditionally initialize elements based on their index, introducing an alternative initialization path. This alteration stresses the compiler's handling of memory initialization sequences, particularly in conjunction with custom allocators, by creating non-standard initialization patterns that may reveal bugs in optimization or code generation."
    }
}