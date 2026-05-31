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

pub struct Add_Const_Generics_293;

impl Mutator for Add_Const_Generics_293 {
    fn name(&self) -> &str {
        "Add_Const_Generics_293"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        let type_ident = type_path.path.segments.last().unwrap().ident.clone();
                        let const_generic: GenericParam = parse_quote! { const N: usize };
                        item_fn.sig.generics.params.push(const_generic);
                        let new_return_type: Type = parse_quote! { #type_ident };
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow {
                                spans: [Span::call_site()],
                            },
                            Box::new(new_return_type),
                        );
                        let new_block: Block = parse_quote! {
                            {
                                N
                            }
                        };
                        item_fn.block = Box::new(new_block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with simple return types by adding a generic constant parameter and modifying the function body to utilize this parameter. It transforms the function into a more complex form that depends on compile-time constant evaluation, challenging the compiler's handling of const generics, type inference, and return type consistency."
    }
}