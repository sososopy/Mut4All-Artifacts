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

pub struct Modify_Return_Type_With_Lifetime_366;

impl Mutator for Modify_Return_Type_With_Lifetime_366 {
    fn name(&self) -> &str {
        "Modify_Return_Type_With_Lifetime_366"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut insert_struct = false;
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if type_path.path.is_ident("usize") {
                            insert_struct = true;
                            func.sig.generics.params.push(GenericParam::Lifetime(
                                LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: Lifetime::new("'a", Span::call_site()),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                },
                            ));
                            func.sig.output = parse_quote! { -> Complex<'a> };
                            func.block = parse_quote! {
                                {
                                    Complex("example")
                                }
                            };
                        }
                    }
                }
            }
        }
        if insert_struct {
            let complex_struct: ItemStruct = parse_quote! {
                struct Complex<'a>(&'a str);
            };
            file.items.insert(0, Item::Struct(complex_struct));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning `usize` and transforms them into returning a custom struct with a lifetime parameter. By injecting a new struct and altering the function's return type to involve lifetimes, it stresses the compiler's handling of lifetime inference, region constraints, and const evaluation, particularly in const contexts."
    }
}