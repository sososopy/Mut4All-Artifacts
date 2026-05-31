use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned, token,
    token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Add_Unused_Generic_Parameter_235;

impl Mutator for Add_Unused_Generic_Parameter_235 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Parameter_235"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    let unused_generic: GenericParam = parse_quote!(U: std::fmt::Debug);
                    generics.params.push(unused_generic);

                    // Modify the function body to include an unused variable of the new generic type
                    let unused_var: Stmt = parse_quote!(let _unused: U;);
                    func.block.stmts.insert(0, unused_var);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            let unused_generic: GenericParam = parse_quote!(U: std::fmt::Debug);
                            generics.params.push(unused_generic);

                            // Modify the method body to include an unused variable of the new generic type
                            let unused_var: Stmt = parse_quote!(let _unused: U;);
                            func.block.stmts.insert(0, unused_var);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused generic parameter `U: std::fmt::Debug` to non-main functions and methods. It also inserts an unused variable of type `U` at the beginning of the function body. This transformation introduces additional complexity and potential for type resolution issues, testing the compiler's handling of unused generics and their impact on type inference and trait bounds."
    }
}