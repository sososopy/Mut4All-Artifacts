use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprClosure, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemConst, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    TraitItem, TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Const_Closure_Missing_Method_326;

impl Mutator for Const_Closure_Missing_Method_326 {
    fn name(&self) -> &str {
        "Const_Closure_Missing_Method_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs = Vec::new();
        let mut traits_with_methods = Vec::new();

        // First pass: collect structs and traits with methods
        for item in &mut file.items {
            if let Item::Struct(s) = item {
                structs.push(s);
            } else if let Item::Trait(trait_item) = item {
                let mut methods = Vec::new();
                for trait_item in &trait_item.items {
                    if let TraitItem::Method(method) = trait_item {
                        methods.push(method);
                    }
                }
                if !methods.is_empty() {
                    traits_with_methods.push((trait_item, methods));
                }
            }
        }

        // Second pass: process const items
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Block(block_expr) = &mut const_item.expr {
                    if let Some(block_expr) = &mut block_expr.expr {
                        if let Expr::Closure(closure) = block_expr {
                            if let Some(closure_body) = &mut closure.body {
                                if !structs.is_empty() && !traits_with_methods.is_empty() {
                                    let (trait_item, methods) = &traits_with_methods[0];
                                    let struct_item = &structs[0];
                                    let struct_name = &struct_item.ident;
                                    let trait_name = &trait_item.ident;
                                    let method_name = &methods[0].sig.ident;

                                    // Create the cast expression: &Struct as &dyn Trait
                                    let cast_expr = parse_quote! {
                                        & #struct_name as &dyn #trait_name
                                    };

                                    // Create the let statement
                                    let let_stmt = parse_quote! {
                                        let obj = #cast_expr;
                                    };

                                    // Create the method call: obj.method_name()
                                    let method_call = parse_quote! {
                                        obj.#method_name()
                                    };

                                    // Add the statements to the closure's body
                                    closure_body.stmts.push(Stmt::Local(Local {
                                        let_token: Default::default(),
                                        pat: Box::new(Pat::Ident(PatIdent {
                                            ident: Ident::new("obj", Span::call_site()),
                                            subpat: None,
                                            mutability: None,
                                        })),
                                        init: Some((token::Eq::default(), Box::new(cast_expr))),
                                        semi_token: Some(Default::default()),
                                    }));
                                    closure_body.stmts.push(Stmt::Expr(method_call, None));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms const closures by casting an existing type to a dyn trait object and invoking a method that is declared but not implemented in the const impl. This tests the compiler's handling of const trait methods and dynamic dispatch in const contexts, potentially exposing bugs in trait resolution or const evaluation."
    }
}