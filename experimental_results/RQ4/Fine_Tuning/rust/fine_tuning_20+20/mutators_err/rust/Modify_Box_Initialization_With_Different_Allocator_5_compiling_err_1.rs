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

pub struct Modify_Box_Initialization_With_Different_Allocator_5;

impl Mutator for Modify_Box_Initialization_With_Different_Allocator_5 {
    fn name(&self) -> &str {
        "Modify_Box_Initialization_With_Different_Allocator_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_system = false;
        for item in &file.items {
            if let Item::Use(item_use) = item {
                if item_use.to_token_stream().to_string() == "use std :: alloc :: System" {
                    has_system = true;
                }
            }
        }
        if !has_system {
            let use_system: Item = parse_quote! { use std::alloc::System; };
            file.items.insert(0, use_system);
        }
        struct BoxVisitor {
            has_system: bool,
        }
        impl VisitMut for BoxVisitor {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                match node {
                    Expr::Call(expr_call) => {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                            if path.is_ident("Box") {
                                if let Some((_, Expr::Path(ExprPath { path, .. }))) =
                                    expr_call.args.last_mut()
                                {
                                    if !path.is_ident("System") {
                                        *path = parse_quote!(System);
                                    }
                                }
                            }
                        }
                    }
                    Expr::MethodCall(expr_method_call) => {
                        if expr_method_call.method == "new_in" {
                            if let Some((_, Expr::Path(ExprPath { path, .. }))) =
                                expr_method_call.args.last_mut()
                            {
                                if !path.is_ident("System") {
                                    *path = parse_quote!(System);
                                }
                            }
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_expr_mut(self, node);
            }
        }
        let mut visitor = BoxVisitor { has_system };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets Box initializations that use custom allocators by modifying them to use the System allocator. It ensures that the System allocator is imported and replaces the existing allocator in Box::new_in calls. This transformation is designed to test the compiler's handling of allocator changes and may reveal issues related to memory management and allocation strategies."
    }
}