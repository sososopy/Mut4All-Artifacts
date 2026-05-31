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

pub struct Modify_Const_Generic_Function_Incompatible_Return_Type_72;

impl Mutator for Modify_Const_Generic_Function_Incompatible_Return_Type_72 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Function_Incompatible_Return_Type_72"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(const_generic) = func.sig.generics.params.iter().find(|param| matches!(param, GenericParam::Const(_))) {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::Array(_) = **return_type {
                            // Change the return type to a single u8
                            func.sig.output = ReturnType::Type(
                                token::RArrow::default(),
                                Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: parse_quote!(u8),
                                })),
                            );

                            // Modify the function body to add logic using the const generic
                            let block = &mut func.block;
                            block.stmts.insert(0, parse_quote! {
                                let _value = [42; #const_generic];
                            });
                            block.stmts.push(parse_quote! {
                                loop {
                                    break;
                                }
                            });
                            block.stmts.push(parse_quote! {
                                0
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions using const generics with a return type dependent on the const generic parameter. It modifies the return type to be incompatible with the intended const generic value, such as changing an array return type to a single value type. The function body is adjusted to simulate logic using the const generic parameter, while ensuring the return type remains incompatible, potentially causing compiler errors or unexpected behavior."
    }
}