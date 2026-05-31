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

pub struct Modify_Box_Allocator_Type_454;

impl Mutator for Modify_Box_Allocator_Type_454 {
    fn name(&self) -> &str {
        "Modify_Box_Allocator_Type_454"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut replace_box = |expr: &mut Expr| {
                    if let Expr::Call(ExprCall { func, args, .. }) = expr {
                        if let Expr::Path(ExprPath { path, .. }) = &**func {
                            if path.segments.len() == 1 && path.segments[0].ident == "Box" {
                                if let Some(PathArguments::AngleBracketed(args)) = &mut path.segments[0].arguments {
                                    if args.args.len() == 2 {
                                        args.args.pop();
                                        args.args.push(GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: parse_quote!(std::alloc::Global),
                                        })));
                                    }
                                }
                                if args.len() == 2 {
                                    args.pop();
                                }
                            }
                        }
                    }
                };
                
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(expr) | Stmt::Semi(expr, _) = stmt {
                        replace_box(expr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Box instantiations with custom allocators, replacing the allocator type with `std::alloc::Global` and adjusting the `new_in` method to `new`. This transformation tests the compiler's handling of allocator API changes and its ability to adapt to standard allocation paths, potentially revealing issues in allocator management and interaction with the debuginfo subsystem."
    }
}