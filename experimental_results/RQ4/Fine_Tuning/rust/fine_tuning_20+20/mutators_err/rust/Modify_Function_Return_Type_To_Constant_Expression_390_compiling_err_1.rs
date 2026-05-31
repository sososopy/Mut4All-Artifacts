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

pub struct Modify_Function_Return_Type_To_Constant_Expression_390;

impl Mutator for Modify_Function_Return_Type_To_Constant_Expression_390 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_To_Constant_Expression_390"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut constants = vec![];
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.is_ident("usize") {
                        constants.push(item_const.ident.clone());
                    }
                }
            }
        }
        let mut has_usize_constant = !constants.is_empty();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if type_path.path.segments.len() == 1
                            && type_path.path.segments[0].ident == "usize"
                        {
                            continue;
                        }
                    }
                    if has_usize_constant {
                        let constant = constants.choose(&mut thread_rng()).unwrap();
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow::default(),
                            Box::new(Type::Array(TypeArray {
                                bracket_token: Default::default(),
                                elem: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: Path::from(Ident::new("usize", Span::call_site())),
                                })),
                                len: Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: Path::from(constant.clone()),
                                }),
                            })),
                        );
                    } else {
                        let new_constant_ident = Ident::new(
                            &format!("__MUTATED_CONST_{}", thread_rng().gen::<u32>()),
                            Span::call_site(),
                        );
                        constants.push(new_constant_ident.clone());
                        has_usize_constant = true;
                        file.items.insert(
                            0,
                            Item::Const(ItemConst {
                                attrs: vec![],
                                vis: Visibility::Inherited,
                                const_token: token::Const::default(),
                                ident: new_constant_ident,
                                colon_token: token::Colon::default(),
                                ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: Path::from(Ident::new("usize", Span::call_site())),
                                })),
                                eq_token: token::Eq::default(),
                                expr: Box::new(Expr::Lit(ExprLit {
                                    attrs: vec![],
                                    lit: Lit::Int(LitInt::new("128", Span::call_site())),
                                })),
                                semi_token: token::Semi::default(),
                            }),
                        );
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow::default(),
                            Box::new(Type::Array(TypeArray {
                                bracket_token: Default::default(),
                                elem: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: Path::from(Ident::new("usize", Span::call_site())),
                                })),
                                len: Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: Path::from(new_constant_ident),
                                }),
                            })),
                        );
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with non-constant return types and modifies them to return a constant array type using existing usize constants. If no suitable constant is available, it introduces a new usize constant. This transformation stresses the compiler's constant evaluation and type checking mechanisms by enforcing a constant-dependent return type, potentially revealing bugs in constant folding, type inference, and array handling."
    }
}