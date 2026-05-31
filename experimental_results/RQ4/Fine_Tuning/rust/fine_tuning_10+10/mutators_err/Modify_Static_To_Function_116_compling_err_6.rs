use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Static_To_Function_116;

impl Mutator for Modify_Static_To_Function_116 {
    fn name(&self) -> &str {
        "Modify_Static_To_Function_116"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::TraitObject(type_trait_object) = &*item_static.ty {
                    let fn_name = item_static.ident.clone();
                    let fn_return_type = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(Box<dyn #type_trait_object>),
                    }));
                    let fn_block = parse_quote!({
                        Box::new(#item_static.expr)
                    });
                    let new_fn = ItemFn {
                        attrs: item_static.attrs.clone(),
                        vis: item_static.vis.clone(),
                        sig: Signature {
                            constness: None,
                            asyncness: None,
                            unsafety: None,
                            abi: None,
                            fn_token: token::Fn(item_static.static_token.span),
                            ident: fn_name,
                            generics: Default::default(),
                            paren_token: token::Paren(item_static.static_token.span),
                            inputs: Punctuated::new(),
                            variadic: None,
                            output: ReturnType::Type(token::RArrow(item_static.static_token.span), fn_return_type),
                        },
                        block: Box::new(fn_block),
                    };
                    *item = Item::Fn(new_fn);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies static variable declarations using dynamic traits within a function or module. It transforms such static variables into function definitions that return a boxed trait object. This involves changing the static keyword to fn, preserving the type signature within the function's return type, and ensuring the body of the static variable is used as the body of the new function. This mutation tests the handling of dynamic trait objects in function contexts, ensuring correct ownership semantics with boxed trait objects."
    }
}