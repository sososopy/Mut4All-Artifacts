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

pub struct Manipulate_Function_Return_Type_With_Impl_Trait_160;

impl Mutator for Manipulate_Function_Return_Type_With_Impl_Trait_160 {
    fn name(&self) -> &str {
        "Manipulate_Function_Return_Type_With_Impl_Trait_160"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let fn_name = &item_fn.sig.ident;
                        let mut has_self_call = false;
                        for stmt in &item_fn.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Call(expr_call) = expr {
                                    if let Expr::Path(expr_path) = &*expr_call.func {
                                        if expr_path.path.is_ident(fn_name) {
                                            has_self_call = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if !has_self_call {
                            let self_call: Expr = parse_quote! { #fn_name() };
                            let mut new_block = item_fn.block.clone();
                            new_block.stmts.push(Stmt::Expr(self_call, None));
                            item_fn.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types and ensures they include a recursive call to themselves. By injecting or preserving self-recursion, it stresses the compiler's handling of opaque return types in recursive contexts, potentially exposing issues in trait resolution, type inference, and code generation for `impl Trait` constructs."
    }
}