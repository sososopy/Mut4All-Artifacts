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

pub struct Manipulate_Function_Return_Type_With_Impl_Trait_160;

impl Mutator for Manipulate_Function_Return_Type_With_Impl_Trait_160 {
    fn name(&self) -> &str {
        "Manipulate_Function_Return_Type_With_Impl_Trait_160"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let func_name = &item_fn.sig.ident;
                        let mut found_recursive_call = false;

                        for stmt in &item_fn.block.stmts {
                            if let Stmt::Expr(Expr::Call(ExprCall { func, .. }), _) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &**func {
                                    if path.is_ident(func_name) {
                                        found_recursive_call = true;
                                        break;
                                    }
                                }
                            }
                        }

                        if !found_recursive_call {
                            let recursive_call: Stmt = parse_quote! {
                                #func_name();
                            };
                            item_fn.block.stmts.push(recursive_call);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types, adding a recursive call to the function within its body if not already present. This stresses the compiler's handling of recursive calls with `impl Trait`, potentially exposing issues in trait resolution and type inference in recursive contexts."
    }
}