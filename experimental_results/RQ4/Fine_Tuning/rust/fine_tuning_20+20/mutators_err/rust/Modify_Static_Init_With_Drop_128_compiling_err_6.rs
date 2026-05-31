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

pub struct Modify_Static_Init_With_Drop_128;

impl Mutator for Modify_Static_Init_With_Drop_128 {
    fn name(&self) -> &str {
        "Modify_Static_Init_With_Drop_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                let mut has_drop = false;
                {
                    let mut drop_checker = |ty: &Type| {
                        if let Type::Path(type_path) = ty {
                            let last_segment = type_path.path.segments.last();
                            if let Some(segment) = last_segment {
                                let ident_str = segment.ident.to_string();
                                if ident_str == "Vec" || ident_str == "String" {
                                    has_drop = true;
                                }
                            }
                        }
                    };
                    let ty = &item_static.ty;
                    drop_checker(ty);
                    let has_drop_copy = has_drop;
                    if !has_drop_copy {
                        if let Ok(ty) = syn::parse2::<Type>(quote! { #item_static }) {
                            drop_checker(&ty);
                        }
                    }
                }
                if has_drop {
                    let new_expr: Expr = parse_quote! {{
                        let test_vec: Vec<String>;
                        test_vec.push(String::from("Example"));
                    }};
                    item_static.expr = Box::new(new_expr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets static variables with initializers that include types implementing Drop, such as Vec or String. It replaces the initializer with a block that declares an uninitialized Vec<String> and pushes a new String into it. This transformation exploits the compiler's handling of destructors in static initializers, potentially triggering ICEs by creating partially initialized drop types."
    }
}