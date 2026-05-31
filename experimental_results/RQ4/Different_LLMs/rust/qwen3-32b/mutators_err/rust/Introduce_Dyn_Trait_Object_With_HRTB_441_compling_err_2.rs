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

pub struct Introduce_Dyn_Trait_Object_With_HRTB_441;

impl Mutator for Introduce_Dyn_Trait_Object_With_HRTB_441 {
    fn name(&self) -> &str {
        "Introduce_Dyn_Trait_Object_With_HRTB_441"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if Deref trait exists
        let mut has_deref = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "Deref" {
                    has_deref = true;
                    break;
                }
            }
        }
        if !has_deref {
            file.items.push(parse_quote! {
                trait Deref<T> {
                    fn deref(&self) -> &T;
                }
            });
        }

        // Add MyTrait with HRTB
        file.items.push(parse_quote! {
            trait MyTrait: for<T> Deref<Target=T> {}
        });

        // Add some_function expecting incompatible type
        file.items.push(parse_quote! {
            fn some_function(arg: *mut usize) {}
        });

        // Modify a function to create and use the trait object
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    // Manually construct Local instead of using parse_quote
                    let pat = Pat::Ident(PatType {
                        attrs: vec![],
                        pat: Box::new(Pat::Wild(Default::default())),
                        colon_token: None,
                        ty: None,
                    });
                    let ty = Type::TraitObject(syn::TypeTraitObject {
                        dyn_token: Some(Default::default()),
                        bounds: Punctuated::new(),
                        paren_token: None,
                    });
                    let expr = Expr::AddrOf(ExprAddrOf {
                        and_token: Default::default(),
                        expr: Box::new(Expr::Tuple(ExprTuple {
                            attrs: vec![],
                            paren_token: Default::default(),
                            elems: Punctuated::new(),
                        })),
                    });
                    let local = Local {
                        let_token: Default::default(),
                        pat: Box::new(pat),
                        init: Some((Default::default(), Box::new(expr))),
                        semi_token: Default::default(),
                        attrs: vec![],
                    };
                    func.block.stmts.insert(0, Stmt::Local(local));

                    // Insert function call
                    let expr_call = parse_quote! {
                        some_function(obj)
                    };
                    func.block.stmts.push(Stmt::Expr(expr_call, None));
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}