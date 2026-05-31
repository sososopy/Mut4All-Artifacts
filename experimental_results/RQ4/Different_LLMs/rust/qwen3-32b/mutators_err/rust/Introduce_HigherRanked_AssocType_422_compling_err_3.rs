use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_HigherRanked_AssocType_422;

impl Mutator for Introduce_HigherRanked_AssocType_422 {
    fn name(&self) -> &str {
        "Introduce_HigherRanked_AssocType_422"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions_to_mutate: Vec<usize> = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ty) = &func.sig.output {
                    if let Type::Reference(_) = **ty {
                        functions_to_mutate.push(i);
                    }
                }
            }
        }

        for &i in functions_to_mutate.iter().rev() {
            let struct_s = parse_quote! { struct S; };
            let impl_s = parse_quote! { impl S { type P<'a> = &'a i32; } };
            let type_function = parse_quote! { type Function = for<'a> fn(&'a i32) -> S::P<'a>; };
            
            file.items.insert(i, Item::Struct(struct_s));
            file.items.insert(i + 1, Item::Impl(impl_s));
            file.items.insert(i + 2, Item::Type(type_function));
            
            if let Item::Fn(func) = &mut file.items[i + 3] {
                if let ReturnType::Type(_, ty) = &mut func.sig.output {
                    *ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(S::P<'_>),
                    }));
                }
                
                let mut has_local = false;
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(_) = stmt {
                        has_local = true;
                        break;
                    }
                }
                
                if !has_local {
                    func.block.stmts.insert(0, Stmt::Local(Local {
                        let_token: Default::default(),
                        pat: Box::new(Pat::Ident(PatIdent {
                            ident: Ident::new("x", Span::call_site()),
                            subpat: None,
                            mutability: None,
                        })),
                        init: Some((syn::token::Eq::default(), Box::new(Expr::Lit(ExprLit {
                            attrs: Vec::new(),
                            lit: Lit::Int(syn::LitInt::new("0", Span::call_site())),
                            span: Span::call_site(),
                        })))),
                        semicolon: Some(Default::default()),
                    }));
                }
                
                let mut local_index = None;
                for (idx, stmt) in func.block.stmts.iter_mut().enumerate() {
                    if let Stmt::Local(_) = stmt {
                        local_index = Some(idx);
                        break;
                    }
                }
                
                if let Some(idx) = local_index {
                    func.block.stmts.insert(idx + 1, Stmt::Local(Local {
                        let_token: Default::default(),
                        pat: Box::new(Pat::Ident(PatIdent {
                            ident: Ident::new("f", Span::call_site()),
                            subpat: None,
                            mutability: None,
                        })),
                        init: Some((syn::token::Eq::default(), Box::new(parse_quote! { |y| y }))),
                        semicolon: Some(Default::default()),
                    }));
                    
                    let mut return_index = None;
                    for (idx, stmt) in func.block.stmts.iter_mut().enumerate() {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Return(_) = &**expr {
                                return_index = Some(idx);
                                break;
                            }
                        }
                    }
                    
                    if let Some(idx) = return_index {
                        *func.block.stmts.get_mut(idx).unwrap() = Stmt::Expr(
                            Expr::Return(parse_quote! { return f(&x); }),
                            None
                        );
                    } else {
                        func.block.stmts.push(Stmt::Expr(
                            Expr::Return(parse_quote! { return f(&x); }),
                            None
                        ));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an inherent associated type with a higher-ranked lifetime in functions returning references. It replaces the return type with the associated type's reference, adds a closure with a for<'a> bound, and ensures the closure captures a local variable. This increases complexity in lifetime inference and exposes borrow-checker inconsistencies between NLL and Polonius."
    }
}